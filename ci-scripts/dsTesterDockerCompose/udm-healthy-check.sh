#!/bin/bash

STATUS=0

RESULT=$(ps aux | grep -v nohup || true)
SUB='/openair-udm/bin/oai_udm -c /openair-udm/etc/udm.conf -o'
if [[ $RESULT =~ $SUB ]]; then
    STATUS=0
else
   STATUS=-1
fi

exit $STATUS
