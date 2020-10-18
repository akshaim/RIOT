/*
 * Copyright (c) 2020 Freie Universitaet Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include "net/emcute.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

int main(void)
{
    SHOW_DEFINE(CONFIG_EMCUTE_DEFAULT_PORT);
    SHOW_DEFINE(CONFIG_EMCUTE_BUFSIZE_EXP);
    SHOW_DEFINE(EMCUTE_BUFSIZE);
    SHOW_DEFINE(CONFIG_EMCUTE_TOPIC_MAXLEN);
    SHOW_DEFINE(CONFIG_EMCUTE_KEEPALIVE);
    SHOW_DEFINE(CONFIG_EMCUTE_T_RETRY);
    SHOW_DEFINE(CONFIG_EMCUTE_N_RETRY);
}
