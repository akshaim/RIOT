APPLICATION=test_net_sock

# BOARD = samr21-xpro
BOARD = native

RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += gnrc_ipv6_default
USEMODULE += tinydtls_sock_dtls
USEMODULE += sock_util
USEMODULE += prng_sha1prng

# Chage this this to see if macros can be configures using CFLAGS
# Use make menuconfig command alternatively to access GUI.

# CFLAGS += -DCONFIG_DTLS_HANDSHAKE_BUFSIZE_EXP=10

include $(RIOTBASE)/Makefile.include
