#!/bin/bash

set -e

export ASN1C_PREFIX=Ngap_

asn1c \
    -pdu=all \
    -fcompound-names \
    -fno-include-deps \
    -findirect-choice \
    -gen-PER \
    -D ies \
    ./scripts/asn.1/*.asn1

