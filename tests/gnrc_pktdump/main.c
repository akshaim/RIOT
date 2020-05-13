/*
 * Copyright (c) 2020 Freie Universitaet Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include "net/gnrc/pktdump.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

int main(void)
{
    SHOW_DEFINE(CONFIG_GNRC_PKTDUMP_MSG_QUEUE_SIZE_EXP);
    SHOW_DEFINE(GNRC_PKTDUMP_MSG_QUEUE_SIZE);
}
