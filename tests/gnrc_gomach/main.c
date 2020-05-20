/*
 * Copyright (C) 2017  INRIA
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
 * @brief       Test application for testing the GoMacH implementation
 *
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
#include "net/gnrc/gomach/gomach.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

int main(void)
{
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_CP_DURATION_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_SUPERFRAME_DURATION_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_CP_RANDOM_END_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_CP_MIN_GAP_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_NO_TX_ISR_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_MAX_PREAM_INTERVAL_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_PREAMBLE_INTERVAL_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_BCAST_INTERVAL_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_VTDMA_SLOT_SIZE_US);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_TX_BUSY_THRESHOLD);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_CP_EXTEND_THRESHOLD);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_RX_DUPCHK_UNIT_LIFE);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_MAX_ALLOC_SENDER_NUM);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_REPHASELOCK_THRESHOLD);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_T2U_RETYR_THRESHOLD);
    SHOW_DEFINE(CONFIG_GNRC_GOMACH_MAX_T2U_RETYR_THRESHOLD);
    
    gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
                                                          gnrc_pktdump_pid);
    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
