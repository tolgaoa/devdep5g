#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_requests>"
    exit 1
fi

n=$1

#amf_to_ausf_aka_outputFile="amf_to_ausf_aka_request_times.csv"
#amf_nrf_disc_outputFile="amf_nrf_disc_request_times.csv"
#amf_pdu_session_outputFile="amf_pdu_session_request_times.csv"
#amf_smf_modify_outputFile="amf_smf_modify_request_times.csv"

#initialize_output_file() {
#    if [ ! -f "$1" ]; then
#        touch "$1"
#    fi
#}

#initialize_output_file "$amf_to_ausf_aka_outputFile"
#initialize_output_file "$amf_nrf_disc_outputFile"
#initialize_output_file "$amf_pdu_session_outputFile"
#initialize_output_file "$amf_smf_modify_outputFile"


ausfpod=$(kubectl get pods -n oai  | grep ausf | awk '{print $1}')
amfpod=$(kubectl get pods -n oai  | grep amf | awk '{print $1}')
smfpod=$(kubectl get pods -n oai  | grep smf | awk '{print $1}')


make_amf_ausf_aka_request() {
    local pod_name=$1
    local namespace=$2
    local outputFile="amf_to_ausf_aka_request_times.csv"

    # Ensure the output file exists
    if [ ! -f "$outputFile" ]; then
        touch "$outputFile"
    fi

    local cmd="curl -X POST http://oai-ausf10-svc:80/nausf-auth/v1/ue-authentications \
    -d '{\"servingNetworkName\":\"5G:mnc095.mcc208.3gppnetwork.org\",\"supiOrSuci\":\"208950000000010\"}' \
    -H 'Content-Type: application/json' -w '%{time_total}' -o /dev/null -s"

    local time_ms=$(kubectl exec $pod_name -n $namespace -c amf -- bash -c "$cmd" | awk '{print $1*1000}')
    echo "$time_ms" >> "$outputFile"

    #echo "AMF to AUSF AKA request completed, took $time_ms ms"
}

make_5g_aka_confirmation_request() {
    local pod_name=$1
    local namespace=$2
    local outputFile="amf_aka_confirmation_request_times.csv"

    # Ensure the output file exists
    if [ ! -f "$outputFile" ]; then
        touch "$outputFile"
    fi

    local log_output1=$(kubectl logs -n oai $pod_name -c amf | grep resStar)
    local res_star=$(echo "$log_output1" | grep -oP '(?<="resStar":")[^"]*')
    local data="{\"resStar\":\"$res_star\"}"
   
	local log_output2=$(kubectl logs -n oai $pod_name -c amf | grep 'nausf-auth/v1/ue-authentications')
 	local href=$(echo "$log_output2" | grep -oP 'href":"[^"]+' | awk -F'/' '{print $7}' | head -n 1)

    local cmd="curl -X PUT http://oai-ausf10-svc:80/nausf-auth/v1/ue-authentications/$href/5g-aka-confirmation \
    -d '$data' \
    -H 'Content-Type: application/json' -w '%{time_total}' -o /dev/null -s"

    local time_ms=$(kubectl exec $pod_name -n $namespace -c amf -- bash -c "$cmd" | awk '{print $1*1000}')
    echo "$time_ms" >> "$outputFile"

    # Provide feedback to the user
    #echo "5G AKA Confirmation request completed, took $time_ms ms"
}

make_amf_nrf_disc_request() {
    local pod_name=$1
    local namespace=$2
    local outputFile="amf_nrf_disc_request_times.csv"

    if [ ! -f "$outputFile" ]; then
        touch "$outputFile"
    fi

    local cmd="curl 'http://oai-nrf10-svc:80/nnrf-disc/v1/nf-instances?target-nf-type=SMF&requester-nf-type=AMF' \
    -H 'Content-Type: application/json' -w '%{time_total}' -o /dev/null -s"

    local time_ms=$(kubectl exec $pod_name -n $namespace -c amf -- bash -c "$cmd" | awk '{print $1*1000}')
    echo "$time_ms" >> "$outputFile"

    #echo "AMF to NRF DISC request completed, took $time_ms ms"
}

make_amf_pdu_session_request() {
    local pod_name=$1
    local namespace=$2
    local outputFile="amf_pdu_session_request_times.csv"
	
	local amf_ip=$(kubectl get pod $pod_name -n oai -o jsonpath='{.status.podIP}')

    if [ ! -f "$outputFile" ]; then
        touch "$outputFile"
    fi

    local cmd="curl -X POST 'http://oai-smf10-svc:80/nsmf-pdusession/v1/sm-contexts' \
    -d '{\"anType\":\"3GPP_ACCESS\",\"dnn\":\"oai\",\"gpsi\":\"msisdn-200000000001\",\"n1MessageContainer\":{\"n1MessageClass\":\"SM\",\"n1MessageContent\":{\"contentId\":\"n1SmMsg\"}},\"pduSessionId\":1,\"pei\":\"imei-200000000000001\",\"requestType\":\"INITIAL_REQUEST\",\"sNssai\":{\"sd\":\"123\",\"sst\":210},\"servingNetwork\":{\"mcc\":\"208\",\"mnc\":\"95\"},\"servingNfId\":\"servingNfId\",\"smContextStatusUri\":\"http://$amf_ip:80/nsmf-pdusession/callback/imsi-208950000000010/1\",\"supi\":\"imsi-208950000000010\"}' \
    -H 'Content-Type: application/json' -w '%{time_total}' -o /dev/null -s"

    local time_ms=$(kubectl exec $pod_name -n $namespace -c amf -- bash -c "$cmd" | awk '{print $1*1000}')
    echo "$time_ms" >> "$outputFile"

    #echo "AMF to SMF PDU SESSION request completed, took $time_ms ms"
}

make_amf_smf_modify_request() {
    local pod_name=$1
    local namespace=$2
    local outputFile="amf_smf_modify_request_times.csv"

    if [ ! -f "$outputFile" ]; then
        touch "$outputFile"
    fi

    local cmd="curl -X POST 'http://oai-smf10-svc:80/nsmf-pdusession/v1/sm-contexts/1/modify' \
    -d '{\"n2SmInfo\":{\"contentId\":\"n2msg\"},\"n2SmInfoType\":\"PDU_RES_SETUP_RSP\"}' \
    -H 'Content-Type: application/json' -w '%{time_total}' -o /dev/null -s"

    local time_ms=$(kubectl exec $pod_name -n $namespace -c amf -- bash -c "$cmd" | awk '{print $1*1000}')
    echo "$time_ms" >> "$outputFile"

    #echo "AMF to SMF MODIFY request completed, took $time_ms ms"
}

sleep=0

for (( i=1; i<=n; i++ ))
do
    make_amf_ausf_aka_request "$amfpod" "oai"
    #echo "AMF to AUSF AKA request $i completed"
    sleep $sleep
    make_5g_aka_confirmation_request "$amfpod" "oai"
    #echo "AMF to AUSF AKA confirmation $i completed"
    sleep $sleep
    make_amf_nrf_disc_request "$amfpod" "oai"
    #echo "AMF to NRF DISC request $i completed"
    sleep $sleep
    make_amf_pdu_session_request "$amfpod" "oai"
    #echo "AMF to SMF PDU SESSION request $i completed"
    sleep $sleep
    make_amf_smf_modify_request "$amfpod" "oai"
    #echo "AMF to SMF MODIFY request $i completed"
    sleep $sleep
    echo "Complete iteration $i"
    
done

echo "All requests completed. Data written to respective files."
