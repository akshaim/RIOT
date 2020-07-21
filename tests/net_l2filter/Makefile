APPLICATION=test_net_l2filter

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += l2filter

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

# CFLAGS += -DCONFIG_L2FILTER_ADDR_MAXLEN=6
# CFLAGS += -DCONFIG_L2FILTER_LISTSIZE=6

include $(RIOTBASE)/Makefile.include
