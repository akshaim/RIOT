/*
 * Copyright (c) 2020 Freie Universitaet Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include "net/csma_sender.h"

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

int main(void)
{
    SHOW_DEFINE(CONFIG_CSMA_SENDER_MIN_BE_DEFAULT);
    SHOW_DEFINE(CONFIG_CSMA_SENDER_MAX_BE_DEFAULT);
    SHOW_DEFINE(CONFIG_CSMA_SENDER_MAX_BACKOFFS_DEFAULT);
    SHOW_DEFINE(CONFIG_CSMA_SENDER_BACKOFF_PERIOD_UNIT);
}
