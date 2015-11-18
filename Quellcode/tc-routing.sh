#! /bin/sh
# generic config used in /usr/local/etc/network/if-up.d/split-access-routing and /usr/local/bin/traffic-shaper.sh

# used programs
BASH=/bin/bash
IP=/bin/ip
IPT=/sbin/iptables
TC=/sbin/tc
TCNG=/usr/bin/tcng

# external Interfaces
EDEV1=eth1
EDEV2=eth2
EDEVLIST="eth1 eth2"

# routing-tables
T1=one
T2=two

# IPs
EDEV1_IP=10.1.0.1
EDEV2_IP=10.2.0.1

# next-hop-router
EDEV1_R=10.1.0.2
EDEV2_R=10.2.0.2

# balancing (*@\label{lst:tc-routing.sh.balancing}@*)
EDEV1_WEIGHT=1
EDEV2_WEIGHT=1

