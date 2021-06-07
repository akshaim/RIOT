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

#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

uint32_t SystemCoreClock  = 4000000UL; /*CPU1: M4 on MSI clock after startup (4MHz)*/
#define SUBGHZ_RFBUSY_LOOP_TIME    ((SystemCoreClock*24U)>>20U)

#define SX126X_SPI_SPEED    (SPI_CLK_5MHZ)
#define SX126X_SPI_MODE     (SPI_MODE_0)

static bool sx126x_radio_sleepstatus = false;

#ifdef ENABLE_DEBUG
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            DEBUG("%u", byte);
        }
    }
    DEBUG("\n");
}
#endif

static uint32_t IsActive_RFBUSYMS(void)
{
  return ((READ_BIT(PWR->SR2, PWR_SR2_RFBUSYMS) == (PWR_SR2_RFBUSYMS)) ? 1UL : 0UL);
}

static uint32_t IsActive_RFBUSYS(void)
{
  return ((READ_BIT(PWR->SR2, PWR_SR2_RFBUSYS) == (PWR_SR2_RFBUSYS)) ? 1UL : 0UL);
}

static uint32_t IsRFUnderReset(void)
{
  return ((READ_BIT(RCC->CSR, RCC_CSR_RFRSTF) == (RCC_CSR_RFRSTF)) ? 1UL : 0UL);
}

static void EXTI_EnableIT_32_63(uint32_t ExtiLine)
{
  EXTI->IMR2 |= ExtiLine;
}

static void PWR_SetRadioBusyTrigger(uint32_t RadioBusyTrigger)
{
  MODIFY_REG(PWR->CR3, PWR_CR3_EWRFBUSY, RadioBusyTrigger);
}

static uint32_t sx126x_radio_waitonbusy(void)
{
    uint32_t count;
    uint32_t mask;
    count  = 100 * SUBGHZ_RFBUSY_LOOP_TIME;

  /* Wait until Busy signal is set */
    do
    {
        mask = IsActive_RFBUSYMS();

        if (count == 0U)
        {
            DEBUG("[ERROR WaitOnBusy - Error] \n");
            return 1;
            break;
    }
    count--;
    } while ((IsActive_RFBUSYS()& mask) == 1UL);

    return 0;
}

static uint32_t sx126x_radio_checkdeviceready(void)
{
  // if (sx126x_radio_sleepstatus == true)
  // {
    DEBUG("[sx126x_radio_checkdeviceready] : Wakeup radio \n");
    PWR->SUBGHZSPICR &= ~PWR_SUBGHZSPICR_NSS; // PULL NSS LOW

    ztimer_sleep(ZTIMER_USEC, 1000);

    PWR->SUBGHZSPICR |= PWR_SUBGHZSPICR_NSS; //PULL NSS UP
  // }
  return (sx126x_radio_waitonbusy());
}

sx126x_hal_status_t sx126x_hal_write(const void *context,
                                     const uint8_t *command, const uint16_t command_length,
                                     const uint8_t *data, const uint16_t data_length)
{
    (void)data;
    (void)data_length;
    sx126x_t *dev = (sx126x_t *)context;
    // while (gpio_read(dev->params->busy_pin)) {}

    while(sx126x_radio_checkdeviceready()){}

    // while (PWR->SR2 & PWR_SR2_RFBUSYMS) {}
    // DEBUG("[sx126x_hal] Radio busy done\n");

    //check the register
    // printf("SUBGHZSPI->CR1: ");
    // printBits(sizeof((int)(SUBGHZSPI->CR1)), (int*)&(SUBGHZSPI->CR1));

    spi_acquire(dev->params->spi, SPI_CS_UNDEF, SX126X_SPI_MODE, SX126X_SPI_SPEED);

    // printf("aFTER aCQUIRE SUBGHZSPI->CR1: ");
    // printBits(sizeof((int)(SUBGHZSPI->CR1)), (int*)&(SUBGHZSPI->CR1));

    // clear the NSS bit
    // 2nd parm NSS Bit UNDEF
    if(command[0] == 0x84U || command[0] == 0x94U) {
      sx126x_radio_sleepstatus = true;
    }
    else {
      sx126x_radio_sleepstatus = false;
    }

    PWR->SUBGHZSPICR &= ~PWR_SUBGHZSPICR_NSS; // PULL NSS LOW

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
    DEBUG("[sx126x_hal] READ \n");
    sx126x_t *dev = (sx126x_t *)context;

    while(sx126x_radio_checkdeviceready()){}

    /* wait for the device to not be busy anymore */
    // while (gpio_read(dev->params->busy_pin)) {}

    spi_acquire(dev->params->spi, SPI_CS_UNDEF, SX126X_SPI_MODE, SX126X_SPI_SPEED);
    PWR->SUBGHZSPICR &= ~PWR_SUBGHZSPICR_NSS; // PULL NSS LOW

    spi_transfer_bytes(dev->params->spi, dev->params->nss_pin, true, command, NULL, command_length);
    spi_transfer_bytes(dev->params->spi, dev->params->nss_pin, false, NULL, data, data_length);

    PWR->SUBGHZSPICR |= PWR_SUBGHZSPICR_NSS; //PULL NSS UP
    spi_release(dev->params->spi);
    return 0;
}

sx126x_hal_status_t sx126x_hal_reset(const void *context)
{
    DEBUG("[sx126x_hal] RESET\n");

    sx126x_t *dev = (sx126x_t *)context;

    gpio_set(dev->params->reset_pin);
    gpio_clear(dev->params->reset_pin);
    /* it takes 100us for the radio to be ready after reset */
    // ztimer_sleep(ZTIMER_USEC, 100);
    gpio_set(dev->params->reset_pin);
    
// #if defined(CPU_FAM_STM32WL)
    RCC->CSR |= RCC_CSR_RFRST; //Reset Radio
    RCC->CSR &= ~RCC_CSR_RFRST; // Disable radio reset
    ztimer_sleep(ZTIMER_USEC, 100);
    
    /* Wait while reset is done */
    while (IsRFUnderReset() != 0UL){}

    /* Asserts the reset signal of the Radio peripheral */
    PWR->SUBGHZSPICR |= PWR_SUBGHZSPICR_NSS;

    /* Enable EXTI 44 : Radio IRQ ITs for CPU1 */
    EXTI_EnableIT_32_63(EXTI_IMR2_IM44);

    /* Enable wakeup signal of the Radio peripheral */
    PWR_SetRadioBusyTrigger(PWR_CR3_EWRFBUSY);

    /* Clear Pending Flag */
    PWR->SCR = PWR_SCR_CWRFBUSYF;

    sx126x_radio_sleepstatus = true;

    return 0;
}

sx126x_hal_status_t sx126x_hal_wakeup(const void *context)
{
    DEBUG("[sx126x_hal] WAKEUP\n");
    sx126x_t *dev = (sx126x_t *)context;

    spi_acquire(dev->params->spi, SPI_CS_UNDEF, SX126X_SPI_MODE, SX126X_SPI_SPEED);
    PWR->SUBGHZSPICR &= ~PWR_SUBGHZSPICR_NSS; // PULL NSS LOW
    gpio_clear(dev->params->nss_pin);

    PWR->SUBGHZSPICR |= PWR_SUBGHZSPICR_NSS; //PULL NSS UP
    gpio_set(dev->params->nss_pin);

    spi_release(dev->params->spi);

    /* it takes 500us for the radio device to be ready after waking up */
    ztimer_sleep(ZTIMER_USEC, 1000);
    return 0;
}
