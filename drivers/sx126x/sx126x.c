/*
 * Copyright (C) 2021 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_sx126x
 * @{
 *
 * @file
 * @brief       Device driver implementation for the SX1261/2/8 and LLCC68 LoRa radio driver
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <errno.h>

#include "sx126x_netdev.h"

#include "net/lora.h"
#include "periph/spi.h"

#include "sx126x_driver.h"
#include "sx126x.h"
#include "sx126x_params.h"

#define ENABLE_DEBUG 1
#include "debug.h"

#ifndef CONFIG_SX126X_PKT_TYPE_DEFAULT
#define CONFIG_SX126X_PKT_TYPE_DEFAULT          (SX126X_PKT_TYPE_LORA)
#endif

#ifndef CONFIG_SX126X_CHANNEL_DEFAULT
#define CONFIG_SX126X_CHANNEL_DEFAULT           (868300000UL)   /* in Hz */
#endif

#ifndef CONFIG_SX126X_TX_POWER_DEFAULT
#define CONFIG_SX126X_TX_POWER_DEFAULT          (0U)           /* in dBm */
#endif

#ifndef CONFIG_SX126X_RAMP_TIME_DEFAULT
#define CONFIG_SX126X_RAMP_TIME_DEFAULT         (SX126X_RAMP_200_US)
#endif

#ifndef SX126X_CAL_DEFAULT
#define SX126X_CAL_DEFAULT                      (0x7F)
#endif

#define REG_XTA_TRIM 0x0911

void sx126x_setup(sx126x_t *dev, const sx126x_params_t *params, uint8_t index)
{
    DEBUG("[sx126x.c] sx126x_setup \n");
    netdev_t *netdev = (netdev_t *)dev;

    netdev->driver = &sx126x_driver;
    dev->params = (sx126x_params_t *)params;
    netdev_register(&dev->netdev, NETDEV_SX126X, index);
}

static const uint16_t _bw_khz[3] = {
    [LORA_BW_125_KHZ] = 125,
    [LORA_BW_250_KHZ] = 250,
    [LORA_BW_500_KHZ] = 500,
};

static uint8_t _compute_ldro(sx126x_t *dev)
{
    DEBUG("[sx126x.c] _compute_ldro \n");
    uint32_t symbol_len = (uint32_t)(1 << dev->mod_params.sf) / _bw_khz[dev->mod_params.bw - SX126X_LORA_BW_125];
    if (symbol_len >= 16) {
        return 0x01;
    }

    return 0x00;
}

static void sx126x_init_default_config(sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_init_default_config \n");

    /* Configure PA optimal settings for maximum output power
     * Values used here comes from the datasheet, section 13.1.14 SetPaConfig
     * and are optimal for a TX output power of 14dBm.
     */
    if (IS_USED(MODULE_LLCC68) || IS_USED(MODULE_SX1262)) {
        sx126x_pa_cfg_params_t pa_cfg = {
            .pa_duty_cycle = 0x04,
            .hp_max = 0x00,
            .device_sel = 0x01,
            .pa_lut = 0x01
        };
        sx126x_set_pa_cfg(dev, &pa_cfg);
    }
    else if (IS_USED(MODULE_SX1268)) {
        sx126x_pa_cfg_params_t pa_cfg = {
            .pa_duty_cycle = 0x04,
            .hp_max = 0x06,
            .device_sel = 0x00,
            .pa_lut = 0x01
        };
        sx126x_set_pa_cfg(dev, &pa_cfg);
    }
    else { /* IS_USED(MODULE_SX1261) */
        sx126x_pa_cfg_params_t pa_cfg = {
            .pa_duty_cycle = 0x04,
            .hp_max = 0x00,
            .device_sel = 0x01,
            .pa_lut = 0x01
        };
        sx126x_set_pa_cfg(dev, &pa_cfg);
    }
    sx126x_set_ocp_value(dev, 0x18);

    uint8_t reg_values = 0;
    sx126x_read_register(dev, 0x08E7, &reg_values, 1 );
    DEBUG("OCP Value %x",reg_values);

    sx126x_set_tx_params(dev, CONFIG_SX126X_TX_POWER_DEFAULT, CONFIG_SX126X_RAMP_TIME_DEFAULT);

    /* packet type must be set first */
    sx126x_set_pkt_type(dev, SX126X_PKT_TYPE_GFSK);

    sx126x_set_channel(dev, CONFIG_SX126X_CHANNEL_DEFAULT);

    dev->mod_params.bw = (sx126x_lora_bw_t)(CONFIG_LORA_BW_DEFAULT + SX126X_LORA_BW_125);
    dev->mod_params.sf = (sx126x_lora_sf_t)CONFIG_LORA_SF_DEFAULT;
    dev->mod_params.cr = (sx126x_lora_cr_t)CONFIG_LORA_CR_DEFAULT;
    dev->mod_params.ldro = _compute_ldro(dev);
    sx126x_set_lora_mod_params(dev, &dev->mod_params);

    dev->pkt_params.pld_len_in_bytes = 0;
    dev->pkt_params.crc_is_on = LORA_PAYLOAD_CRC_ON_DEFAULT;
    dev->pkt_params.header_type = (
        IS_ACTIVE(CONFIG_LORA_FIXED_HEADER_LEN_MODE_DEFAULT) ? true : false
        );
    dev->pkt_params.preamble_len_in_symb = CONFIG_LORA_PREAMBLE_LENGTH_DEFAULT;
    dev->pkt_params.invert_iq_is_on = (
        IS_ACTIVE(CONFIG_LORA_IQ_INVERTED_DEFAULT) ? true : false
        );
    sx126x_set_lora_pkt_params(dev, &dev->pkt_params);
}

static void _dio1_isr(void *arg)
{
    DEBUG("[sx126x.c] _dio1_isr \n");
    netdev_trigger_event_isr((netdev_t *)arg);
}

int sx126x_init(sx126x_t *dev) //check
{
    DEBUG("[sx126x.c] sx126x_init \n");
    int res = spi_init_cs(dev->params->spi, SPI_CS_UNDEF); //Check
    if (res != SPI_OK) {
        DEBUG("[sx126x] error: failed to initialize SPI_%i device (code %i)\n",
              dev->params->spi, res);
        return -1;
    }

    DEBUG("[sx126x] init: SPI_%i initialized with success\n", dev->params->spi);

    gpio_init(dev->params->reset_pin, GPIO_OUT);
    gpio_init(dev->params->busy_pin, GPIO_IN_PD);

    /* Initialize DIOs */
    if (gpio_is_valid(dev->params->dio1_pin)) {
        res = gpio_init_int(dev->params->dio1_pin, GPIO_IN, GPIO_RISING, _dio1_isr, dev);
        if (res < 0) {
            DEBUG("[sx126x] error: failed to initialize DIO1 pin\n");
            return res;
        }
    }
    else {
        DEBUG("[sx126x] error: no DIO1 pin defined\n");
        return -EIO;
    }

    /* Reset the device */
    DEBUG("[sx126x_init] : Reset the device\n");
    sx126x_reset(dev);

    sx126x_set_reg_mode(dev, dev->params->regulator);

    sx126x_set_standby(dev, SX126X_STANDBY_CFG_RC);

    sx126x_set_buffer_base_address(dev, 0x00, 0x00);
    uint8_t test = 0xFF;
    DEBUG("[sx126x_init] Write buffer : %x\n", test);
    sx126x_write_buffer(dev, 0x00, &test, 1);
    test = 0x03; // some random value
    sx126x_read_buffer(dev, 0x00, &test, 1);
    DEBUG("[sx126x_init] Read buffer : %x\n", test);

    // uint8_t reg_value = 0;
    // sx126x_read_register( dev, REG_XTA_TRIM, &reg_value, 1 );
    // DEBUG("Trim value before reset : %x\n", reg_value);
    // sx126x_write_register(dev, REG_XTA_TRIM, ( const uint8_t[] ){ 0x00 },1);
    // sx126x_read_register( dev, REG_XTA_TRIM, &reg_value, 1 );
    // DEBUG("Trim value after reset : %x\n", reg_value);
    while (1){}

    /* Configure the power regulator mode */
    sx126x_set_reg_mode(dev, dev->params->regulator);
    // sx126x_read_register()

    sx126x_set_dio3_as_tcxo_ctrl(dev, SX126X_TCXO_CTRL_1_7V, 10 << 6 );

    sx126x_cal(dev, SX126X_CAL_DEFAULT);

    sx126x_set_buffer_base_address(dev, 0x00, 0x00);

    /* Initialize radio with the default parameters */
    sx126x_init_default_config(dev);

    // /* Configure available IRQs */
    // const uint16_t irq_mask = (
    //     SX126X_IRQ_TX_DONE |
    //     SX126X_IRQ_RX_DONE |
    //     SX126X_IRQ_PREAMBLE_DETECTED |
    //     SX126X_IRQ_HEADER_VALID |
    //     SX126X_IRQ_HEADER_ERROR |
    //     SX126X_IRQ_CRC_ERROR |
    //     SX126X_IRQ_CAD_DONE |
    //     SX126X_IRQ_CAD_DETECTED |
    //     SX126X_IRQ_TIMEOUT
    //     );

    sx126x_set_dio_irq_params(dev, 0xFFFF, 0xFFFF, 0, 0);

    if (IS_ACTIVE(ENABLE_DEBUG)) {
        sx126x_pkt_type_t pkt_type;
        sx126x_get_pkt_type(dev, &pkt_type);
        DEBUG("[sx126x] init radio: pkt type: %d\n", pkt_type);

        sx126x_chip_status_t radio_status;
        sx126x_get_status(dev, &radio_status);
        DEBUG("[sx126x] init: chip mode %d\n", radio_status.chip_mode);
        DEBUG("[sx126x] init: cmd status %d\n", radio_status.cmd_status);
    }
    sx126x_set_lora_sync_word(dev, 0x34); //public nw
    return res;
}

uint8_t sx126x_get_radio_status(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_radio_status \n");
    return dev->mod_params.bw - SX126X_LORA_BW_125;
}

uint32_t sx126x_get_channel(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_channel \n");
    return dev->channel;
}

void sx126x_set_channel(sx126x_t *dev, uint32_t freq)
{
    DEBUG("[sx126x.c] sx126x_set_channel \n");
    dev->channel = freq;
    sx126x_set_rf_freq(dev, dev->channel);
}

uint8_t sx126x_get_bandwidth(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_bandwidth \n");
    return dev->mod_params.bw - SX126X_LORA_BW_125;
}

void sx126x_set_bandwidth(sx126x_t *dev, uint8_t bandwidth)
{
    DEBUG("[sx126x.c] sx126x_set_bandwidth \n");
    dev->mod_params.bw = bandwidth + SX126X_LORA_BW_125;
    dev->mod_params.ldro = _compute_ldro(dev);
    sx126x_set_lora_mod_params(dev, &dev->mod_params);
}

uint8_t sx126x_get_spreading_factor(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_spreading_factor \n");
    return dev->mod_params.sf;
}

void sx126x_set_spreading_factor(sx126x_t *dev, uint8_t sf)
{
    DEBUG("[sx126x.c] sx126x_set_spreading_factor \n");
    dev->mod_params.sf = (sx126x_lora_sf_t)sf;
    dev->mod_params.ldro = _compute_ldro(dev);
    sx126x_set_lora_mod_params(dev, &dev->mod_params);
}

uint8_t sx126x_get_coding_rate(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_coding_rate \n");
    return dev->mod_params.cr;
}

void sx126x_set_coding_rate(sx126x_t *dev, uint8_t cr)
{
    DEBUG("[sx126x.c] sx126x_set_coding_rate \n");
    dev->mod_params.cr = (sx126x_lora_cr_t)cr;
    sx126x_set_lora_mod_params(dev, &dev->mod_params);
}

uint8_t sx126x_get_lora_payload_length(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_lora_payload_length \n");
    sx126x_rx_buffer_status_t rx_buffer_status;

    sx126x_get_rx_buffer_status(dev, &rx_buffer_status);
    return rx_buffer_status.pld_len_in_bytes;
}

void sx126x_set_lora_payload_length(sx126x_t *dev, uint8_t len)
{
    DEBUG("[sx126x.c] sx126x_set_lora_payload_length \n");
    dev->pkt_params.pld_len_in_bytes = len;
    sx126x_set_lora_pkt_params(dev, &dev->pkt_params);
}

bool sx126x_get_lora_crc(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_lora_crc \n");
    return dev->pkt_params.crc_is_on;
}

void sx126x_set_lora_crc(sx126x_t *dev, bool crc)
{
    DEBUG("[sx126x.c] sx126x_set_lora_crc \n");
    dev->pkt_params.crc_is_on = crc;
    sx126x_set_lora_pkt_params(dev, &dev->pkt_params);
}

bool sx126x_get_lora_implicit_header(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_lora_implicit_header \n");
    return dev->pkt_params.header_type == SX126X_LORA_PKT_IMPLICIT;
}

void sx126x_set_lora_implicit_header(sx126x_t *dev, bool mode)
{
    DEBUG("[sx126x.c] sx126x_set_lora_implicit_header \n");
    dev->pkt_params.header_type = (mode ? SX126X_LORA_PKT_IMPLICIT : SX126X_LORA_PKT_EXPLICIT);
    sx126x_set_lora_pkt_params(dev, &dev->pkt_params);
}

uint16_t sx126x_get_lora_preamble_length(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_lora_preamble_length \n");
    return dev->pkt_params.preamble_len_in_symb;
}

void sx126x_set_lora_preamble_length(sx126x_t *dev, uint16_t preamble)
{
    DEBUG("[sx126x.c] sx126x_set_lora_preamble_length \n");
    dev->pkt_params.preamble_len_in_symb = preamble;
    sx126x_set_lora_pkt_params(dev, &dev->pkt_params);
}

bool sx126x_get_lora_iq_invert(const sx126x_t *dev)
{
    DEBUG("[sx126x.c] sx126x_get_lora_iq_invert \n");
    return dev->pkt_params.invert_iq_is_on;
}

void sx126x_set_lora_iq_invert(sx126x_t *dev, bool iq_invert)
{
    DEBUG("[sx126x.c] sx126x_set_lora_iq_invert \n");
    dev->pkt_params.invert_iq_is_on = iq_invert;
    sx126x_set_lora_pkt_params(dev, &dev->pkt_params);
}
