APPLICATION=test_gnrc_pktbuf

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += gnrc
USEMODULE += gnrc_pktbuf

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

#CFLAGS += -DCONFIG_GNRC_PKTBUF_SIZE=5000

include $(RIOTBASE)/Makefile.include
