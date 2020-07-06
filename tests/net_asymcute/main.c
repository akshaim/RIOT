/*
 * Copyright (c) 2020 Freie Universitaet Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include "net/asymcute.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

int main(void)
{
    SHOW_DEFINE(CONFIG_ASYMCUTE_BUFSIZE_EXP);
    SHOW_DEFINE(ASYMCUTE_BUFSIZE);
    SHOW_DEFINE(CONFIG_ASYMCUTE_TOPIC_MAXLEN);
    SHOW_DEFINE(CONFIG_ASYMCUTE_KEEPALIVE);
    SHOW_DEFINE(CONFIG_ASYMCUTE_KEEPALIVE_PING);
    SHOW_DEFINE(CONFIG_ASYMCUTE_T_RETRY);
    SHOW_DEFINE(CONFIG_ASYMCUTE_N_RETRY);
}
