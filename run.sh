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

#gnbsimim dnnim users slices iterationstest thrtesttype(1==hostlvl, 0==podlvl)
st=`date +%s`

/bin/bash ./deploy.sh zoomv1 zoomv1 10 8 10 1

et=`date +%s`
rt=$((et-st))
echo $rt >> ite1.txt

/bin/bash ./deploy.sh netflixv1 netflixv1 10 8 10 1
/bin/bash ./deploy.sh oculusv1 oculusv1 10 8 10 1
/bin/bash ./deploy.sh tiktokv1 tiktokv1 10 8 10 1
/bin/bash ./deploy.sh whatsappv1 whatsappv1 10 8 10 1
/bin/bash ./deploy.sh fortnitev2 fortnitev1 10 8 10 1

et=`date +%s`
rt=$((et-st))
echo $rt
echo $rt >> itetot.txt


echo "-------------------------------------------------"
echo "Experiment Finished for All Use Cases"
echo $rt
echo "-------------------------------------------------"
