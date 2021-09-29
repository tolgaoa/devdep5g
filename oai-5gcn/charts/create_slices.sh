#!/bin/bash


./slice_conf5.sh
sleep 30
./undeploy_all.sh
sleep 30
./slice_conf6.sh
sleep 30
./undeploy_all.sh
sleep 30
./slice_conf7.sh
sleep
./undeploy_all.sh
