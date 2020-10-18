APPLICATION=test_net_emcute

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

#USEMODULE += gnrc_netdev_default
#USEMODULE += auto_init_gnrc_netif
#USEMODULE += gnrc_sock_udp
USEMODULE += gnrc_ipv6_default
USEMODULE += emcute

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

# CFLAGS += -DCONFIG_EMCUTE_DEFAULT_PORT=1990
# CFLAGS += -DCONFIG_EMCUTE_BUFSIZE_EXP=8
# CFLAGS += -DCONFIG_EMCUTE_TOPIC_MAXLEN=200
# CFLAGS += -DCONFIG_EMCUTE_KEEPALIVE=300
# CFLAGS += -DCONFIG_EMCUTE_T_RETRY=10
# CFLAGS += -DCONFIG_EMCUTE_N_RETRY=4

include $(RIOTBASE)/Makefile.include
