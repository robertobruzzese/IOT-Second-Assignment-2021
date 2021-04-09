# name of your application
APPLICATION = Task02

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f401re

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..


# Default to using ethos for providing the uplink when not on native
UPLINK ?= ethos
# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
# Interrupt handler module
USEMODULE += periph_gpio_irq

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += periph_adc
USEMODULE += xtimer
USEMODULE += analog_util
# Include MQTT-SN
USEMODULE += emcute
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6
# For testing we also include the ping6 command and some stats
USEMODULE += gnrc_icmpv6_echo
# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single
USEMODULE += timex
USEMODULE += stdio_ethos gnrc_uhcpc

USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp


 # Modules to include:
USEMODULE += gnrc_dhcpv6

ETHOS_BAUDRATE ?= 115200
TAP ?= tap0
USE_DHCPV6 ?= 0
IPV6_PREFIX ?= 2001:0db8::/64 #?
SERVER_ADDR = 2001:0db8::5
SERVER_PORT = 1885
MQTT_TOPIC_OUT = localgateway_to_awsiot
MQTT_TOPIC_IN = awsiot_to_localgateway
CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)
CFLAGS += -DMQTT_TOPIC_OUT='"$(MQTT_TOPIC_OUT)"'
CFLAGS += -DMQTT_TOPIC_IN='"$(MQTT_TOPIC_IN)"'

# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif


ifneq (,$(filter $(BOARD),$(BOARD_PROVIDES_NETIF)))
  # Use modules for networking
  # gnrc is a meta module including all required, basic gnrc networking modules
  USEMODULE += gnrc
  # use the default network interface for the board
  USEMODULE += gnrc_netdev_default
  # automatically initialize the network interface
  USEMODULE += auto_init_gnrc_netif
  # shell command to send L2 packets with a simple string
  USEMODULE += gnrc_txtsnd
  # the application dumps received packets to stdout
  USEMODULE += gnrc_pktdump
endif
QUIET ?=1
include $(CURDIR)/Makefile.ethos.conf
include $(RIOTBASE)/Makefile.include

.PHONY: host-tools

host-tools:
	$(Q)env -u CC -u CFLAGS $(MAKE) -C $(RIOTTOOLS)
