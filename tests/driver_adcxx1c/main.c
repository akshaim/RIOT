/*
 * Copyright (C) 2017 OTA keys S.A.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test application for the ADCXX1C ADC driver
 *
 * @author      Vincent Dupont <vincent@otakeys.com>
 * @}
 */

#include <stdio.h>

#include "xtimer.h"
#include "timex.h"
#include "adcxx1c.h"
#include "adcxx1c_params.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))
#define SIZE_ARR(x)  (sizeof(x) / sizeof((x)[0]))

#define SLEEP_USEC  (100 * US_PER_MS)

static adcxx1c_t dev;

static void alert_cb(void *arg)
{
    (void)arg;
    puts("[Alert]\n");
}

int main(void)
{
    int16_t data;

    SHOW_DEFINE(CONFIG_ADCXX1C_I2C_ADDRESS);
    SHOW_DEFINE(CONFIG_ADCXX1C_PARAM_CYCLE);
    SHOW_DEFINE(CONFIG_ADCXX1C_PARAM_LOW_LIMIT);
    SHOW_DEFINE(CONFIG_ADCXX1C_PARAM_HIGH_LIMIT);
    SHOW_DEFINE(CONFIG_ADCXX1C_PARAM_HYSTERESIS);

    puts("ADCXX1C analog to digital driver test application\n");
    printf("Initializing ADCXX1C analog to digital at I2C_DEV(%i)... ",
           adcxx1c_params->i2c);

    if (adcxx1c_init(&dev, adcxx1c_params) == ADCXX1C_OK) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return -1;
    }

    puts("Enabling alert interrupt: ");
    if (adcxx1c_enable_alert(&dev, alert_cb, NULL) == ADCXX1C_OK) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return -1;
    }

    while (1) {
        adcxx1c_read_raw(&dev, &data);
        printf("Raw analog value: %d\n", data);
        xtimer_usleep(SLEEP_USEC);
    }

    return 0;
}
