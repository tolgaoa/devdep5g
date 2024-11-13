#!/bin/bash

# Check if the number of requests was passed as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_requests>"
    exit 1
fi

# Total number of requests to distribute over a 1-minute window
TOTAL_REQUESTS=$1

DURATION_MINUTES=3

# Output files
amf_to_ausf_aka_outputFile="amf_to_ausf_aka_request_times.csv"
amf_nrf_disc_outputFile="amf_nrf_disc_request_times.csv"
amf_pdu_session_outputFile="amf_pdu_session_request_times.csv"
amf_smf_modify_outputFile="amf_smf_modify_request_times.csv"

# Get pod names
namespace="oai"
amfpod=$(kubectl get pods -n $namespace | grep amf | awk '{print $1}')

# Function to initialize output files
initialize_output_file() {
    if [ ! -f "$1" ]; then
        echo "Timestamp,Response Time (ms),Success" > "$1"
    fi
}

# Initialize output files
initialize_output_file "$amf_to_ausf_aka_outputFile"
initialize_output_file "$amf_nrf_disc_outputFile"
initialize_output_file "$amf_pdu_session_outputFile"
initialize_output_file "$amf_smf_modify_outputFile"

# Functions to make specific requests and log the response times
make_amf_ausf_aka_request() {
    local timestamp=$(date +%Y-%m-%dT%H:%M:%S)
    local response=$(kubectl exec $amfpod -n $namespace -c amf -- curl -X POST "http://oai-ausf10-svc:80/nausf-auth/v1/ue-authentications" \
    -d '{"servingNetworkName":"5G:mnc095.mcc208.3gppnetwork.org","supiOrSuci":"208950000000010"}' \
    -H "Content-Type: application/json" \
    -w '%{time_total}:%{http_code}' -o /dev/null -s)
    local time_ms=$(echo $response | cut -d: -f1 | awk '{print $1*1000}')
    local http_code=$(echo $response | cut -d: -f2)
    local success=0
    if [ "$http_code" -eq 200 ]; then
        success=1
    fi
    echo "$timestamp,$time_ms,$success" >> "$amf_to_ausf_aka_outputFile"
}

make_amf_nrf_disc_request() {
    local timestamp=$(date +%Y-%m-%dT%H:%M:%S)
    local response=$(kubectl exec $amfpod -n $namespace -c amf -- curl "http://oai-nrf10-svc:80/nnrf-disc/v1/nf-instances?target-nf-type=SMF&requester-nf-type=AMF" \
    -w '%{time_total}:%{http_code}' -o /dev/null -s)
    local time_ms=$(echo $response | cut -d: -f1 | awk '{print $1*1000}')
    local http_code=$(echo $response | cut -d: -f2)
    local success=0
    if [ "$http_code" -eq 200 ]; then
        success=1
    fi
    echo "$timestamp,$time_ms,$success" >> "$amf_nrf_disc_outputFile"
}

make_amf_pdu_session_request() {
    local timestamp=$(date +%Y-%m-%dT%H:%M:%S)
    local response=$(kubectl exec $amfpod -n $namespace -c amf -- curl -X POST "http://oai-smf10-svc:80/nsmf-pdusession/v1/sm-contexts" \
    -d '{"anType":"3GPP_ACCESS","dnn":"oai","gpsi":"msisdn-200000000001","n1MessageContainer":{"n1MessageClass":"SM","n1MessageContent":{"contentId":"n1SmMsg"}},"pduSessionId":1,"pei":"imei-200000000000001","requestType":"INITIAL_REQUEST","sNssai":{"sd":"123","sst":210},"servingNetwork":{"mcc":"208","mnc":"95"},"servingNfId":"servingNfId","smContextStatusUri":"http://10.42.0.35:80/nsmf-pdusession/callback/imsi-208950000000010/1","supi":"imsi-208950000000010"}' \
    -H "Content-Type: application/json" \
    -w '%{time_total}:%{http_code}' -o /dev/null -s)
    local time_ms=$(echo $response | cut -d: -f1 | awk '{print $1*1000}')
    local http_code=$(echo $response | cut -d: -f2)
    local success=0
    if [ "$http_code" -eq 200 ]; then
        success=1
    fi
    echo "$timestamp,$time_ms,$success" >> "$amf_pdu_session_outputFile"
}

make_amf_smf_modify_request() {
    local timestamp=$(date +%Y-%m-%dT%H:%M:%S)
    local response=$(kubectl exec $amfpod -n $namespace -c amf -- curl -X POST "http://oai-smf10-svc:80/nsmf-pdusession/v1/sm-contexts/1/modify" \
    -d '{"n2SmInfo":{"contentId":"n2msg"},"n2SmInfoType":"PDU_RES_SETUP_RSP"}' \
    -H "Content-Type: application/json" \
    -w '%{time_total}:%{http_code}' -o /dev/null -s)
    local time_ms=$(echo $response | cut -d: -f1 | awk '{print $1*1000}')
    local http_code=$(echo $response | cut -d: -f2)
    local success=0
    if [ "$http_code" -eq 200 ]; then
        success=1
    fi
    echo "$timestamp,$time_ms,$success" >> "$amf_smf_modify_outputFile"
}

# Function to execute requests for a minute
execute_requests_for_a_minute() {
    local interval=$(echo "scale=2; 60 / $TOTAL_REQUESTS" | bc) # Interval between requests to fit into 1 minute

    for (( i=0; i<TOTAL_REQUESTS; i++ )); do
        # Determine which request to make based on modulo of i
        case $((i % 4)) in
            0) make_amf_nrf_disc_request & ;;
            1) make_amf_nrf_disc_request & ;;
            2) make_amf_nrf_disc_request & ;;
            3) make_amf_nrf_disc_request & ;;
        esac

        sleep "$interval" # Control the rate of requests
    done
    wait # Wait for all background processes to complete
}

# Function to execute requests for the specified duration
execute_requests_for_duration() {
    for (( j=0; j<DURATION_MINUTES; j++ )); do
        execute_requests_for_a_minute
        echo "Cycle $((j+1)) of $DURATION_MINUTES completed."
        sleep 1 # Small delay before starting the next cycle
    done
}

# Execute the requests for the duration
execute_requests_for_duration

echo "All requests executed for $DURATION_MINUTES minutes."

