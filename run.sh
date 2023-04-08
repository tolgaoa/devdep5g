#!/bin/bash

#---------------------------------------------------------------
#---------------------------------------------------------------
GREEN='\x1b[32m'
BLUE='\x1b[34m'
RED='\x1B[31m'
NC='\033[0m'

bold=$(tput bold)
NORMAL=$(tput sgr0)
#---------------------------------------------------------------
#---------------------------------------------------------------

st=`date +%s`

#/bin/bash ./deploy.sh fortnitev2 fortnitev1 41
#/bin/bash ./deploy.sh netflixv1 netflixv1 31
#/bin/bash ./deploy.sh oculusv1 oculusv1 31
#/bin/bash ./deploy.sh tiktokv1 tiktokv1 31
#/bin/bash ./deploy.sh whatsappv1 whatsappv1 31
/bin/bash ./deploy.sh n4kul 4kaggrdu 31

et=`date +%s`
rt=$((et-st))
echo $rt


echo "-------------------------------------------------"
echo "Experiment Finished for All Use Cases"
echo $rt
echo "-------------------------------------------------"
