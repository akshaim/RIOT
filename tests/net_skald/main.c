/*
 * Copyright (c) 2020 Freie Universitaet Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include <stdio.h>
#include "net/skald.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))
#define SIZE_ARR(x)  (sizeof(x) / sizeof((x)[0]))

int main(void)
{
    static uint8_t len;
    static const uint8_t _ad_chan[] = SKALD_ADV_CHAN;
    SHOW_DEFINE(CONFIG_SKALD_INTERVAL);
    printf("%d \n" , SIZE_ARR(_ad_chan));
    for(len = 0; len < SIZE_ARR(_ad_chan); len++)
        printf("%d \n", _ad_chan[len]);
}
