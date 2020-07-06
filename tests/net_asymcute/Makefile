APPLICATION=test_net_asymcute

#BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

#USEMODULE += gnrc_netdev_default
#USEMODULE += auto_init_gnrc_netif
#USEMODULE += gnrc_sock_udp
USEMODULE += gnrc_ipv6_default
USEMODULE += asymcute

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

# CFLAGS += -DCONFIG_ASYMCUTE_BUFSIZE_EXP=6
# CFLAGS += -DCONFIG_ASYMCUTE_TOPIC_MAXLEN=16
# CFLAGS += -DCONFIG_ASYMCUTE_KEEPALIVE=300
# CFLAGS += -DCONFIG_ASYMCUTE_KEEPALIVE_PING=200
# CFLAGS += -DCONFIG_ASYMCUTE_T_RETRY=11
# CFLAGS += -DCONFIG_ASYMCUTE_N_RETRY=4

include $(RIOTBASE)/Makefile.include
