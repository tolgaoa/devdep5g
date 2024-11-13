#!/bin/bash

# Check if at least one VNF was passed as an argument
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <vnf1> [<vnf2> ...]"
    echo "Available VNFs: nrf, amf, smf, ausf, udm, udr"
    exit 1
fi

# VNF list to monitor
VNFS=("$@")

# Namespace and general variables
NAMESPACE="oai"
DURATION="10 minute 0 seconds"
END=$(date -ud "$DURATION" +%s)

# Function to get pod name based on VNF type
get_pod_name() {
    local vnf="$1"
    kubectl get pods -n "$NAMESPACE" | grep "${vnf}" | awk '{print $1}'
}

# Map VNFs to their output files and initialize files with headers
declare -A OUTPUT_FILES
for vnf in "${VNFS[@]}"; do
    OUTPUT_FILE="${vnf}_cpu_mem_usage.csv"
    OUTPUT_FILES[$vnf]=$OUTPUT_FILE
    echo "Pod Name,Container Name,Timestamp,CPU Usage (millicores),Memory Usage (bytes)" > "$OUTPUT_FILE"
done

# Start data collection
while [[ $(date -u +%s) -lt $END ]]; do
  for vnf in "${VNFS[@]}"; do
    POD_NAME=$(get_pod_name "$vnf")
    CONTAINER_NAME="$vnf" # Assuming container names match VNF names
    
    if [ -n "$POD_NAME" ]; then
        # Fetch CPU and Memory usage
        METRICS_JSON=$(kubectl get --raw "/apis/metrics.k8s.io/v1beta1/namespaces/$NAMESPACE/pods/$POD_NAME")
        CPU_USAGE_NANO=$(echo "$METRICS_JSON" | jq -r ".containers[] | select(.name==\"$CONTAINER_NAME\") | .usage.cpu" | sed 's/[^0-9]*//g')
        MEMORY_USAGE=$(echo "$METRICS_JSON" | jq -r ".containers[] | select(.name==\"$CONTAINER_NAME\") | .usage.memory")
        CPU_USAGE_MILLI=$(bc <<< "scale=3; $CPU_USAGE_NANO / 1000000")
        TIMESTAMP=$(date +%Y-%m-%d\ %H:%M:%S)
        
        # Append data to the respective output file
        echo "$POD_NAME,$CONTAINER_NAME,$TIMESTAMP,${CPU_USAGE_MILLI},${MEMORY_USAGE}" >> "${OUTPUT_FILES[$vnf]}"
    else
        echo "Warning: Pod for $vnf not found in namespace $NAMESPACE."
    fi
  done
  sleep 2
done

echo "Data collection completed. Output written to individual files for each selected VNF."

