#!/bin/bash


for ((i=2;i<=127;i++));
do
	kubectl exec -n oai oai-dnn-6bb4997b54-bc9g7 -- ping -c 500 12.1.1.$i &
	sleep 2s
done
