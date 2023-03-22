#!/bin/bash

helm delete nrf udr udm ausf amf smf upf gnb -n oai
kubectl delete -k oai-dnn/
