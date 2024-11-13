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
#ADJUSTING THE NUMBER OF USERS AND SLICES
#-------Inputs to the deploy.sh script below
#Input $1: gnbsim image -- DO NOT CHANGE
#Input $2: data network node image -- DO NOT CHANGE
#Input $3: number of users
#Input $4: number of slices
#Input $5: number of measurement reps -- only necessary when there is traffic flow
#Input $6: logging -- not used


/bin/bash ./deploy.sh zoomv3 zoomv3 1 1 1 1

#/bin/bash ./deploy.sh netflixv3 netflixv3 10 8 20 1
#/bin/bash ./deploy.sh oculusv3 oculusv3 10 8 20 1
#/bin/bash ./deploy.sh tiktokv3 tiktokv3 10 8 20 1
#/bin/bash ./deploy.sh whatsappv3 whatsappv3 10 8 20 1
#/bin/bash ./deploy.sh fortnitev3 fortnitev3 10 8 20 1


echo "-------------------------------------------------"
echo "Experiment Finished for All Use Cases"
echo $rt
echo "-------------------------------------------------"
