#!/bin/bash

# Check if the number of requests and VNF type were provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <number_of_requests> <vnf_type>"
    echo "Available VNF types: smf, nrf, ausf, amf"
    exit 1
fi

n=$1
vnf_type=$2

# Retrieve pod names for VNFs
ausfpod=$(kubectl get pods -n oai | grep ausf | awk '{print $1}')
amfpod=$(kubectl get pods -n oai | grep amf | awk '{print $1}')
smfpod=$(kubectl get pods -n oai | grep smf | awk '{print $1}')

# Function to make AMF to AUSF AKA request
make_amf_ausf_aka_request() {
    local pod_name=$1
    local namespace=$2
    local outputFile="amf_to_ausf_aka_request_times.csv"
gnbsim10-869986db47-drc9r
    if [ ! -f "$outputFile" ]; then
        touch "$outputFile"
    fi

    local cmd="curl -X POST http://oai-ausf10-svc:80/nausf-auth/v1/ue-authentications \
    -d '{\"servingNetworkName\":\"5G:mnc095.mcc208.3gppnetwork.org\",\"supiOrSuci\":\"208950000000010\"}' \
    -H 'Content-Type: application/json' -w '%{time_total}' -o /dev/null -s"

    local time_ms=$(kubectl exec $pod_name -n $namespace -c amf -- bash -c "$cmd" | awk '{print $1*1000}')
    echo "$time_ms" >> "$outputFile"
}

# Function to make 5G AKA Confirmation request
make_5g_aka_confirmation_request() {
    local pod_name=$1
    local namespace=$2
    local outputFile="amf_aka_confirmation_request_times.csv"

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
}

# Function to make AMF to NRF DISC request
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
}

# Function to make AMF to SMF PDU SESSION request
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
}

# Function to make AMF to SMF MODIFY request
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
}

# Map VNF type to corresponding pod and requests
case $vnf_type in
    "smf")
        pod_name="$smfpod"
        requests=("make_amf_pdu_session_request" "make_amf_smf_modify_request")
        ;;
    "nrf")
        pod_name="$amfpod"
        requests=("make_amf_nrf_disc_request")
        ;;
    "ausf")
        pod_name="$ausfpod"
        requests=("make_amf_ausf_aka_request")
        ;;
    "amf")
        pod_name="$amfpod"
        requests=("make_amf_ausf_aka_request" "make_amf_pdu_session_request" "make_amf_smf_modify_request")
        ;;
    *)
        echo "Invalid VNF type specified. Exiting."
        exit 1
        ;;
esac

# Execute selected requests
for (( i=1; i<=n; i++ )); do
    for request in "${requests[@]}"; do
        $request "$pod_name" "oai"
        sleep 1  # Adjust sleep time if needed
    done
    echo "Complete iteration $i"
done

echo "All requests for $vnf_type completed. Data written to respective files."

