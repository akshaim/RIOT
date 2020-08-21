APPLICATION=test_net_skald

#BOARD = samr21-xpro
#BOARD = native
BOARD ?= nrf52dk

RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += skald

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

# CFLAGS += -DCONFIG_SKALD_INTERVAL=1500000
# CFLAGS += -DSKALD_ADV_CHAN={37,38}

include $(RIOTBASE)/Makefile.include
