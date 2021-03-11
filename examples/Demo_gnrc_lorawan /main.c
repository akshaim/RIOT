/*
 * Copyright (C) 2019 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 * @file
 * @brief       Test application for GNRC LoRaWAN
 *
 * @author      José Ignacio Alamos <jose.alamos@haw-hamburg.de>
 * @}
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thread.h"
#include "xtimer.h"
#include "shell.h"
#include "shell_commands.h"

#include "timex.h"
#include "fmt.h"
#include "dht.h"
#include "../../drivers/dht/include/dht_params.h"

#define DELAY           (10 * US_PER_SEC)
#define MODULE_DHT11

#define MSG           "{ '%s': [ { 'ts': %llu000, 'values':{'%s': %d, '%s': %d}}]}"

#include "board.h"

#include "net/gnrc/netapi.h"
#include "net/gnrc/netif.h"

#include "net/gnrc/pktbuf.h"
#include "net/gnrc/pktdump.h"
#include "net/gnrc/netreg.h"
#include "net/loramac.h"

#define LORAWAN_QUEUE_SIZE (4U)

// static void _usage(void)
// {
//     puts("usage: send <if_pid> <payload> [port]");
// }

int _send(const char *temp_s)
{
    gnrc_pktsnip_t *pkt;
    uint8_t port = CONFIG_LORAMAC_DEFAULT_TX_PORT; /* Default: 2 */
    int interface=3;

    pkt = gnrc_pktbuf_add(NULL, temp_s, strlen(temp_s), GNRC_NETTYPE_UNDEF);

    /* register for returned packet status */
    if (gnrc_neterr_reg(pkt) != 0) {
        puts("Can not register for error reporting");
        return 0;
    }


    gnrc_netapi_set(interface, NETOPT_LORAWAN_TX_PORT, 0, &port, sizeof(port));
    gnrc_netif_send(gnrc_netif_get_by_pid(interface), pkt);

    /* wait for packet status and check */
    msg_t msg;
    msg_receive(&msg);
    if ((msg.type != GNRC_NETERR_MSG_TYPE) ||
        (msg.content.value != GNRC_NETERR_SUCCESS)) {
        printf("Error sending packet: (status: %d\n)", (int) msg.content.value);
    }
    else {
        puts("Successfully sent packet");
    }

    return 0;
}

// static const shell_command_t shell_commands[] = {
//     { "send",       "Send LoRaWAN data",     tx_cmd },
//     { NULL, NULL, NULL }
// };

static int _measure(dht_t *dev, char *temp_s, char *hum_s)
{
    int16_t temp, hum;

    if (dht_read(dev, &temp, &hum) != DHT_OK) {
            puts("Error reading values");
            return -1;
        }
    
    size_t n = fmt_s16_dfp(temp_s, temp, -1);
    temp_s[n] = '\0';
    n = fmt_s16_dfp(hum_s, hum, -1);
    hum_s[n] = '\0';
    return 0;
}

int main(void)
{
    puts("LoRaWAN SAUL test application\n");

#ifdef MODULE_DHT11
    dht_t dev;
    char temp_s[10];
    char hum_s[10];

    /* initialize first configured sensor */
    printf("Initializing DHT sensor...\t");
    if (dht_init(&dev, &dht_params[0]) == DHT_OK) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }
#endif

    /* start the shell */
    // puts("Initialization successful - starting the shell now");
    gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(CONFIG_LORAMAC_DEFAULT_TX_PORT,
                                                          gnrc_pktdump_pid);

    gnrc_netreg_register(GNRC_NETTYPE_LORAWAN, &dump);

    // char line_buf[SHELL_DEFAULT_BUFSIZE];

    // shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* periodically read temp and humidity values */
    while (1) {
        _measure(&dev,temp_s,hum_s);
        printf("DHT values - temp: %s°C - relative humidity: %s%%\n",
            temp_s, hum_s);
        _send(temp_s);
         xtimer_usleep(DELAY);
    }

    return 0;
}
