#!/bin/bash
#set -eo pipefail

STATUS=0
tunnel_interface_status=$(ls -lrth /sys/class/net/ | grep oaitun_ue1)

if [[ $tunnel_interface_status ]]; then
    route del default
    route add default gw 12.1.1.1 oaitun_ue1
    STATUS=0
else
    echo "oaitun_ue1 interface is not up"
    STATUS=1
fi

exit $STATUS