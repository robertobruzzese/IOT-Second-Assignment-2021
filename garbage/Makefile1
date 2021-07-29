APPLICATION = Application

BOARD ?= iotlab-m3

RIOTBASE ?= $(CURDIR)/../../RIOT


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += xtimer
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


# Add the required modules here
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
SERVER_ADDR = 2001:660:3207:400::33
SERVER_PORT = 1885



ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif



#FEATURES_OPTIONAL += periph_rtc

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Activate ICMPv6 error messages
USEMODULE += gnrc_icmpv6_error
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
# This application dumps received packets to STDIO using the pktdump module
USEMODULE += gnrc_pktdump
# Additional networking mdules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo

 
ifneq (,$(filter gnrc_sock,$(USEMODULE))) 
   USEMODULE_INCLUDES += $(RIOTBASE)/sys/net/gnrc/sock/include 
endif

ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif


 
RIOTBASE ?= $(CURDIR)/../../RIOT

# if NUCLEO = 1 => an stm32 nucleo board is being used, else => an M3 node is being used
NUCLEO = 0
CFLAGS += -DNUCLEO=$(NUCLEO)

#include $(CURDIR)/Makefile.net.conf
include $(RIOTBASE)/Makefile.include

