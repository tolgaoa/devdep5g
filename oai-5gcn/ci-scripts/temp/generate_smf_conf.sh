# prompt has been removed for easier Ctrl+C Ctrl+V
# please update the following information according to your configuration

INSTANCE=1
PREFIX='/openair-smf/etc'

declare -A SMF_CONF

SMF_CONF[@INSTANCE@]=$INSTANCE
SMF_CONF[@PID_DIRECTORY@]='/var/run'

SMF_CONF[@SMF_INTERFACE_NAME_FOR_N4@]='CI_N4_IF_NAME'
SMF_CONF[@SMF_INTERFACE_NAME_FOR_SBI@]='CI_SBI_IF_NAME'

SMF_CONF[@SMF_INTERFACE_PORT_FOR_SBI@]='80'
SMF_CONF[@SMF_INTERFACE_HTTP2_PORT_FOR_SBI@]='9090'
SMF_CONF[@SMF_API_VERSION@]='v1'

SMF_CONF[@UDM_IPV4_ADDRESS@]='127.0.0.1'
SMF_CONF[@UDM_PORT@]='80'
SMF_CONF[@UDM_API_VERSION@]='v2'

SMF_CONF[@AMF_IPV4_ADDRESS@]='CI_AMF_IP_ADDR'
SMF_CONF[@AMF_PORT@]='80'
SMF_CONF[@AMF_API_VERSION@]='v1'

SMF_CONF[@UPF_IPV4_ADDRESS@]='CI_UPF_IP_ADDR'

SMF_CONF[@DEFAULT_DNS_IPV4_ADDRESS@]='8.8.8.8'
SMF_CONF[@DEFAULT_DNS_SEC_IPV4_ADDRESS@]='4.4.4.4'

for K in "${!SMF_CONF[@]}"; do 
  egrep -lRZ "$K" $PREFIX/smf.conf | xargs -0 -l sed -i -e "s|$K|${SMF_CONF[$K]}|g"
  ret=$?;[[ ret -ne 0 ]] && echo "Tried to replace $K with ${SMF_CONF[$K]}"
done

echo "SMF Configuration Successful"
