#!/bin/bash

STATUS=0

RESULT=$(ps aux | grep -v nohup || true)
SUB='/openair-udr/bin/oai_udr -c /openair-udr/etc/udr.conf -o'
if [[ $RESULT =~ $SUB ]]; then
    STATUS=0
else
   STATUS=-1
fi

exit $STATUS
