#!/bin/bash

STATUS=0
NB_UNREPLACED_AT=`cat /openair-smf/etc/*.conf | grep -v contact@openairinterface.org | grep -c @ || true`
NOT_FOUND=`ldd /openair-smf/bin/oai_smf | grep -c "not found" || true`
if [[ $NB_UNREPLACED_AT -ne 0 || $NOT_FOUND -ne 0 ]]
then
  STATUS=-1
fi

exit $STATUS
