APPLICATION=test_net_cord

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

# USEMODULE += cord
# USEMODULE += cord_common
USEMODULE += cord_ep
# USEMODULE += cord_epsim

# USEMODULE += gnrc_netdev_default
# USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
# USEMODULE += gnrc_icmpv6_echo

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

# CFLAGS += -DCONFIG_CORD_LT=5000
# CFLAGS += -DCONFIG_CORD_UPDATE_INTERVAL=3000
# CFLAGS += -DCONFIG_CORD_EP=\"Newname\"

include $(RIOTBASE)/Makefile.include
