APPLICATION=test_gnrc_pktdump

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += gnrc
USEMODULE += gnrc_pktdump

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

#CFLAGS += -DGNRC_PKTDUMP_MSG_QUEUE_SIZE=32

include $(RIOTBASE)/Makefile.include
