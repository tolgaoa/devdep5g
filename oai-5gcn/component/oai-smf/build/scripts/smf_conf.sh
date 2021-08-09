# prompt has been removed for easier Ctrl+C Ctrl+V
# please update the following information according to your configuration

INSTANCE=1
PREFIX='/usr/local/etc/oai'
sudo mkdir -m 0777 -p $PREFIX
cp ../../etc/smf.conf  $PREFIX

declare -A SMF_CONF

SMF_CONF[@INSTANCE@]=$INSTANCE
SMF_CONF[@PREFIX@]=$PREFIX
SMF_CONF[@PID_DIRECTORY@]='/var/run'

SMF_CONF[@SMF_INTERFACE_NAME_FOR_N4@]='enx0050b6f4b8f0'
SMF_CONF[@SMF_INTERFACE_NAME_FOR_SBI@]='enx0050b6f4b8f0'

SMF_CONF[@SMF_INTERFACE_PORT_FOR_SBI@]='80'
SMF_CONF[@SMF_INTERFACE_HTTP2_PORT_FOR_SBI@]='9090'
SMF_CONF[@SMF_API_VERSION@]='v1'

SMF_CONF[@UDM_IPV4_ADDRESS@]='172.16.1.103'
SMF_CONF[@UDM_PORT@]='80'
SMF_CONF[@UDM_API_VERSION@]='v2'

SMF_CONF[@AMF_IPV4_ADDRESS@]='192.168.122.183'
SMF_CONF[@AMF_PORT@]='80'
SMF_CONF[@AMF_API_VERSION@]='v1'

SMF_CONF[@UPF_IPV4_ADDRESS@]='192.168.12.245'

SMF_CONF[@NRF_IPV4_ADDRESS@]='192.168.1.23'
SMF_CONF[@NRF_PORT@]='8080'
SMF_CONF[@NRF_API_VERSION@]='v1'
 
SMF_CONF[@DEFAULT_DNS_IPV4_ADDRESS@]='8.8.8.8'
SMF_CONF[@DEFAULT_DNS_SEC_IPV4_ADDRESS@]='4.4.4.4'

for K in "${!SMF_CONF[@]}"; do 
  egrep -lRZ "$K" $PREFIX | xargs -0 -l sed -i -e "s|$K|${SMF_CONF[$K]}|g"
  ret=$?;[[ ret -ne 0 ]] && echo "Tried to replace $K with ${SMF_CONF[$K]}"
done
