/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <cpu.h>
#include <string.h>
#include <stdlib.h>
#include "periph/spi.h"

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

int main(void)
{

    printf("RCC->CFGR: ");
    printBits(sizeof((int)(RCC->CFGR)), (int*)&(RCC->CFGR));

    printf("RCC->CR: ");
    printBits(sizeof((int)(RCC->CR)), (int*)&(RCC->CR));

    printf("RCC->EXTCFGR: ");
    printBits(sizeof((int)(RCC->EXTCFGR)), (int*)&(RCC->EXTCFGR));

    printf("RCC->APB3RSTR: ");
    printBits(sizeof((int)(RCC->APB3RSTR)), (int*)&(RCC->APB3RSTR));

    printf("RCC->APB3ENR: ");
    printBits(sizeof((int)(RCC->APB3ENR)), (int*)&(RCC->APB3ENR));

    printf("GPIOA->AFRL: ");
    printBits(sizeof((int)(GPIOA->AFR[0])), (int*)&(GPIOA->AFR[0]));
    
    while(1) {
    PWR->SUBGHZSPICR |= PWR_SUBGHZSPICR_NSS;
    printf("PWR->SUBGHZSPICR: ");
    printBits(sizeof((int)(PWR->SUBGHZSPICR)), (int*)&(PWR->SUBGHZSPICR));
    PWR->SUBGHZSPICR &= ~PWR_SUBGHZSPICR_NSS;
    printf("PWR->SUBGHZSPICR: ");
    printBits(sizeof((int)(PWR->SUBGHZSPICR)), (int*)&(PWR->SUBGHZSPICR));
    }
    return 0;
}
