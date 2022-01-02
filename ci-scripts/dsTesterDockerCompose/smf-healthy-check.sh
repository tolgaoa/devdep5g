#!/bin/bash

STATUS=0

RESULT=$(ps aux | grep -v nohup || true)
SUB='/openair-smf/bin/oai_smf -c /openair-smf/etc/smf.conf -o'
if [[ $RESULT =~ $SUB ]]; then
    STATUS=0
else
   STATUS=-1
fi

exit $STATUS
