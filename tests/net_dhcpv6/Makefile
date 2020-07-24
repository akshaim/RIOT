APPLICATION=test_net_dhcpv6

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += dhcpv6

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI..

# CFLAGS += -DCONFIG_DHCPV6_CLIENT_PFX_LEASE_MAX=2

include $(RIOTBASE)/Makefile.include
