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

/bin/bash ./deploy.sh zoomv3 zoomv3 10 8 20 1

et=`date +%s`
rt=$((et-st))
echo $rt >> ite1.txt

/bin/bash ./deploy.sh netflixv3 netflixv3 10 8 20 1
/bin/bash ./deploy.sh oculusv3 oculusv3 10 8 20 1
/bin/bash ./deploy.sh tiktokv3 tiktokv3 10 8 20 1
#/bin/bash ./deploy.sh whatsappv3 whatsappv3 10 8 20 1
/bin/bash ./deploy.sh fortnitev3 fortnitev3 10 8 20 1

et=`date +%s`
rt=$((et-st))
echo $rt
echo $rt >> itetot.txt


echo "-------------------------------------------------"
echo "Experiment Finished for All Use Cases"
echo $rt
echo "-------------------------------------------------"
