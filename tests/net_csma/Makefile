APPLICATION=test_net_csma

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += csma_sender

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

# CFLAGS += -DCONFIG_CSMA_SENDER_MIN_BE_DEFAULT=2
# CFLAGS += -DCONFIG_CSMA_SENDER_MAX_BE_DEFAULT=4
# CFLAGS += -DCONFIG_CSMA_SENDER_MAX_BACKOFFS_DEFAULT=3
# CFLAGS += -DCONFIG_CSMA_SENDER_BACKOFF_PERIOD_UNIT=160

include $(RIOTBASE)/Makefile.include
