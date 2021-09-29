#!/bin/bash


for ((j=1;j<=3;j++));
do
	for ((i=10;i<=249;i++));
	do
		if [ $i -le 99 ]
		then
			((insval=$i+332)) #must equal the last line of the insertion table currently = 343
			sed -i "${insval}i\ \ \ \ INSERT INTO \`users\` VALUES (\'20895${j}0000000$i\',\'380561234567\',\'55000000000001\',NULL,\'PURGED\',50,40000000,100000000,47,0000000000,1,0x0C${j}A34601D4F07677303652C046250$i,0,0,0x40,\'ebd07771ace8677a\',0x63bfa50ee6523365ff14c1f45f88737d);" mysql/values.yaml
		else
			((insval=$i+332)) #must equal the last line of the insertion table currently = 343
			sed -i "${insval}i\ \ \ \ INSERT INTO \`users\` VALUES (\'20895${j}000000$i\',\'380561234567\',\'55000000000001\',NULL,\'PURGED\',50,40000000,100000000,47,0000000000,1,0x0C${j}A34601D4F07677303652C04625$i,0,0,0x40,\'ebd07771ace8677a\',0x63bfa50ee6523365ff14c1f45f88737d);" mysql/values.yaml
		fi
	done
done

