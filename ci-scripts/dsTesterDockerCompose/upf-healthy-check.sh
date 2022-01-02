#!/bin/bash

STATUS=0

RESULT=$(ps aux | grep -v nohup || true)
SUB='/openair-spgwu-tiny/bin/oai_spgwu -c /openair-spgwu-tiny/etc/spgw_u.conf -o'
if [[ $RESULT =~ $SUB ]]; then
    STATUS=0
else
   STATUS=-1
fi

exit $STATUS
