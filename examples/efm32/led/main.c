#include <stdio.h>
#include <stdint.h>

#include "board.h"
#include "periph_conf.h"

#ifdef CLOCK_CORECLOCK
#define DELAY_SHORT         (CLOCK_CORECLOCK / 50)
#else
#define DELAY_SHORT         (500000UL)
#endif
#define DELAY_LONG          (DELAY_SHORT * 4)

void dumb_delay(uint32_t delay)
{
    for (uint32_t i = 0; i < delay; i++) {
        __asm__("nop");
    }
}

int main(void)
{
    puts("Board should start blinking now");
    int numof = 0;

    /* get the number of available LED's and turn them all off*/
#ifdef LED0_ON
    ++numof;
    LED0_OFF;
#endif
#ifdef LED1_ON
    ++numof;
    LED1_OFF;
#endif
#ifdef LED2_ON
    ++numof;
    LED2_OFF;
#endif

    puts("On-board LED test\n");
    /* cppcheck-suppress knownConditionTrueFalse
     * (reason: board-dependent ifdefs) */
    if (numof == 0) {
        puts("NO LEDs AVAILABLE");
    }
    else {
        printf("Available LEDs: %i\n\n", numof);
        puts("Will now light up each LED once short and twice long in a loop");
    }


    while (1) {
#ifdef LED0_ON
        LED0_ON;
        dumb_delay(DELAY_SHORT);
        LED0_OFF;
        dumb_delay(DELAY_SHORT);
#endif
#ifdef LED1_ON
        LED1_ON;
        dumb_delay(DELAY_SHORT);
        LED1_OFF;
        dumb_delay(DELAY_SHORT);

#endif
#ifdef LED2_ON
        LED2_ON;
        dumb_delay(DELAY_SHORT);
        LED2_OFF;
        dumb_delay(DELAY_SHORT);
;
#endif
    }

    return 0;
}
