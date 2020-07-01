#include ../../Makefile.tests_common

# name of your application
APPLICATION = efm32-led

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../..

# Some boards do not initialize LED0 by default
CFLAGS=-DAUTO_INIT_LED0

include $(RIOTBASE)/Makefile.include