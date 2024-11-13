#!/bin/bash

NAMESPACE="oai"
POD_NAMES=("oai-udr10-cc7c4f88f-7gscs" "oai-udm10-6585bcffd-r5v87" "oai-ausf10-6bb9dddd4c-lzsmf" "oai-amf10-578678f77d-rxcj4" "oai-smf10-7bbd68cf79-gmd8z")
CONTAINER_NAME="proxy"
OUTPUT_CSV="network_functions_cpu_mem_usage.csv"

# Header for CSV file
echo "Pod Name,Container Name,Timestamp,CPU Usage (millicores),Memory Usage (bytes)" > "$OUTPUT_CSV"

# Adjust the script to run for x minutes y seconds
END=$(date -ud "10 minute 0 seconds" +%s)

while [[ $(date -u +%s) -lt $END ]]; do
  for POD_NAME in "${POD_NAMES[@]}"; do
    METRICS_JSON=$(kubectl get --raw "/apis/metrics.k8s.io/v1beta1/namespaces/$NAMESPACE/pods/$POD_NAME")
    CPU_USAGE_NANO=$(echo $METRICS_JSON | jq -r ".containers[] | select(.name==\"$CONTAINER_NAME\") | .usage.cpu" | sed 's/[^0-9]*//g')
    MEMORY_USAGE=$(echo $METRICS_JSON | jq -r ".containers[] | select(.name==\"$CONTAINER_NAME\") | .usage.memory")
    CPU_USAGE_MILLI=$(bc <<< "scale=3; $CPU_USAGE_NANO / 1000000")
    TIMESTAMP=$(date +%Y-%m-%d\ %H:%M:%S)
    echo "$POD_NAME,$CONTAINER_NAME,$TIMESTAMP,${CPU_USAGE_MILLI},${MEMORY_USAGE}" >> "$OUTPUT_CSV"
  done
  sleep 2
done

echo "Data collection completed. Output written to $OUTPUT_CSV."

