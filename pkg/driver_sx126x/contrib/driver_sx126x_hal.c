/*
 * Copyright (C) 2021 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     pkg_driver_sx126x
 * @{
 *
 * @file
 * @brief       HAL implementation for the SX1261/2 LoRa radio driver
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include "ztimer.h"

#include "periph/gpio.h"
#include "periph/spi.h"

#include "sx126x.h"
#include "sx126x_hal.h"

#define ENABLE_DEBUG 1
#include "debug.h"
#include "cpu.h"

#define SX126X_SPI_SPEED    (SPI_CLK_1MHZ)
#define SX126X_SPI_MODE     (SPI_MODE_0)

sx126x_hal_status_t sx126x_hal_write(const void *context,
                                     const uint8_t *command, const uint16_t command_length,
                                     const uint8_t *data, const uint16_t data_length)
{
    (void)data;
    (void)data_length;
    sx126x_t *dev = (sx126x_t *)context;
    // DEBUG("Txinsdsdsdsd \n");

    // DEBUG("Pulling SPINSS Low \n");
    // PWR->SUBGHZSPICR &= ~PWR->SUBGHZSPICR;

    // DEBUG("Pulling SPINSS High \n");
    // PWR->SUBGHZSPICR |= ~PWR->SUBGHZSPICR;

    /* wait for the device to not be busy anymore */
    // while (gpio_read(dev->params->busy_pin)) {}

    // while (PWR->SR2 & PWR_SR2_RFBUSYMS) {}

    // DEBUG("Not busy \n");

    //check the register
    spi_acquire(dev->params->spi, SPI_CS_UNDEF, SX126X_SPI_MODE, SX126X_SPI_SPEED);

    PWR->SUBGHZSPICR &= ~PWR_SUBGHZSPICR_NSS; // PULL NSS LOW

    // clear the NSS bit
    // 2nd parm NSS Bit UNDEF
    spi_transfer_bytes(dev->params->spi, SPI_CS_UNDEF, data_length != 0, command, NULL,
                       command_length);
    if (data_length) {
        spi_transfer_bytes(dev->params->spi, SPI_CS_UNDEF, false, data, NULL, data_length);
    }
    PWR->SUBGHZSPICR |= PWR_SUBGHZSPICR_NSS; //PULL NSS UP
    spi_release(dev->params->spi);
    return 0;
}

sx126x_hal_status_t sx126x_hal_read(const void *context,
                                    const uint8_t *command, const uint16_t command_length,
                                    uint8_t *data, const uint16_t data_length)
{
    sx126x_t *dev = (sx126x_t *)context;

    /* wait for the device to not be busy anymore */
    while (gpio_read(dev->params->busy_pin)) {}

    spi_acquire(dev->params->spi, SPI_CS_UNDEF, SX126X_SPI_MODE, SX126X_SPI_SPEED);
    spi_transfer_bytes(dev->params->spi, dev->params->nss_pin, true, command, NULL, command_length);
    spi_transfer_bytes(dev->params->spi, dev->params->nss_pin, false, NULL, data, data_length);
    spi_release(dev->params->spi);
    return 0;
}

sx126x_hal_status_t sx126x_hal_reset(const void *context)
{
    DEBUG("[sx126x_hal] reset\n");
    sx126x_t *dev = (sx126x_t *)context;

    gpio_set(dev->params->reset_pin);
    gpio_clear(dev->params->reset_pin);
    /* it takes 100us for the radio to be ready after reset */
    ztimer_sleep(ZTIMER_USEC, 100);
    gpio_set(dev->params->reset_pin);
    return 0;
}

sx126x_hal_status_t sx126x_hal_wakeup(const void *context)
{
    DEBUG("[sx126x_hal] wakeup\n");
    sx126x_t *dev = (sx126x_t *)context;

    spi_acquire(dev->params->spi, SPI_CS_UNDEF, SX126X_SPI_MODE, SX126X_SPI_SPEED);
    gpio_clear(dev->params->nss_pin);
    gpio_set(dev->params->nss_pin);
    spi_release(dev->params->spi);

    /* it takes 500us for the radio device to be ready after waking up */
    ztimer_sleep(ZTIMER_USEC, 500);
    return 0;
}
