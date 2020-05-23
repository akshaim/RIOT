/*
 * Copyright (C) 2008, 2009, 2010 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2013 INRIA
 * Copyright (C) 2013 Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Test application for testing the LWMAC implementation
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 * @author      Shuguo Zhuo <shuguo.zhuo@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "shell.h"
#include "shell_commands.h"

#include "net/gnrc/pktdump.h"
#include "net/gnrc.h"
#include "net/gnrc/lwmac/lwmac.h"
#include "net/gnrc/lwmac/types.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

int main(void)
{
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_WAKEUP_INTERVAL_US);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_TIME_BETWEEN_WR_US);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_WR_PREPARATION_US);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_DATA_DELAY_US);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_DATA_CSMA_RETRIES);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_MAX_DATA_TX_RETRIES);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_MAX_RX_EXTENSION_NUM);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_BROADCAST_CSMA_RETRIES);
    SHOW_DEFINE(CONFIG_GNRC_LWMAC_TIMEOUT_COUNT);
    puts("LWMAC test application");

    gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
                                                          gnrc_pktdump_pid);
    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
