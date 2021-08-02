# name of your application
APPLICATION = application

# If no BOARD is found in the environment, use this default:
BOARD ?= iotlab-m3

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /senslab/users/rbruzzes/RIOT
RIOTMAKE ?= /senslab/users/rbruzzes/RIOT/makefiles
# Default to using ethos for providing the uplink when not on native
UPLINK ?= ethos
# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1
# Modules to include:
USEMODULE += xtimer

USEMODULE += lps331ap
USEMODULE += lsm303dlhc
USEMODULE += isl29020
USEMODULE += l3g4200d

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Activate ICMPv6 error messages
USEMODULE += gnrc_icmpv6_error
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
USEMODULE += gnrc_ipv6
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl


# This application dumps received packets to STDIO using the pktdump module
USEMODULE += gnrc_pktdump


# Additional networking mdules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo

USEMODULE += gnrc_ipv6_default
USEMODULE += emcute
#SERVER_ADDR = 2001:660:3207:400::61
#SERVER_PORT = 1885

# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl
MQTT_TOPIC = localgateway_to_awsiot
MQTT_TOPIC_IN = awsiot_to_localgateway
CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)

CFLAGS += -DMQTT_TOPIC='"$(MQTT_TOPIC)"'
CFLAGS += -DMQTT_TOPIC_IN='"$(MQTT_TOPIC_IN)"'

ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk
# if NUCLEO = 1 => an stm32 nucleo board is being used, else => an M3 node is bei$

NUCLEO = 0
CFLAGS += -DNUCLEO=$(NUCLEO)

#inizio moduli opzionali  
# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single

# Modules to include:
USEMODULE += gnrc_dhcpv6
ETHOS_BAUDRATE ?= 115200
TAP ?= tap0
USE_DHCPV6 ?= 0
IPV6_PREFIX ?= 2001:660:3207:4c1::/64 #?
SERVER_ADDR = 22001:0660:3207:0400::64
SERVER_PORT = 1885
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

USEMODULE += timex
USEMODULE += stdio_ethos gnrc_uhcpc

include $(CURDIR)/Makefile.ethos.conf
#include $(CURDIR)/Makefile.net.conf
include $(RIOTBASE)/Makefile.include
