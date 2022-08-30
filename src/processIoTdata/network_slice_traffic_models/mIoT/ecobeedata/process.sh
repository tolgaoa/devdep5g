#!/bin/bash

rm processedin.txt
rm processedout.txt

touch processedin.txt
touch processedout.txt

function abs_diff {
        echo $(($1 >= $2 ? $1 - $2 : $2 - $1))
}

function ms_diff {
        prev=$1
        curr=$2

        mil1val=$((1*(10#${curr:11:1}-10#${prev:11:1})))
        mil10val=$((10*(10#${curr:10:1}-10#${prev:10:1})))
        mil100val=$((100*(10#${curr:9:1}-10#${prev:9:1})))
        secval=$((1000*(10#${curr:6:2}-10#${prev:6:2})))
        minval=$((60000*(10#${curr:3:2}-10#${prev:3:2})))
        hrval=$((360000*(10#${curr:0:2}-10#${prev:0:2})))

        totaldiff=$(($hrval+$minval+$secval+$mil100val+$mil10val+$mil1val))

        echo $totaldiff
}

IFS=$'\n'

file=$(cat $1)

SRC_DST="idt.ecobee.com.8190"

curIUin=0
curIUout=0

dataUsageIn=0
dataUsageOut=0

prevT=$(echo $file | awk 'FNR == 1 {print $1}')

re='^[0-9]+$'

for i in $file
do
	currentSRC=$(echo $i | awk '{print $3}' )
	currentDST=$(echo $i | awk '{print $5}' )
	curT=$(echo $i | awk '{print $1}' )
	curU=$(echo $i | awk '{print $NF}')

	if [[ $currentSRC  == *$SRC_DST* ]];
	then
		if ! [[ $curU =~ $re ]] ;
		then
			echo "OpenWRT packet. Skipping parse"
			#echo ""

		else
			curIUin=$(($curIUin+$curU))
		fi
	fi

        if [[ $currentDST  == *$SRC_DST* ]];
        then
                if ! [[ $curU =~ $re ]] ;
                then
                        echo "OpenWRT packet. Skipping parse"
                        #echo ""

                else
                        curIUout=$(($curIUout+$curU))
                fi
        fi

	if [ ${curT:11:1} -ne ${prevT:11:1} ];
	then

		timePassed=$(ms_diff $prevT $curT)

		echo $timePassed

		datausageIn=$(($curIUin))
		datausageOut=$(($curIUout))
		echo $datausageIn >> processedin.txt
		echo $datausageOut >> processedout.txt
		curIUin=0
		curIUout=0

		for ((j=1;j<=timePassed;j++))
		do
			echo $curIUin >> processedin.txt
			echo $curIUout >> processedout.txt
		done
	fi
	prevT=$curT

done

