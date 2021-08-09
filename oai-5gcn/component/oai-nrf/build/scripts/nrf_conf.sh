# prompt has been removed for easier Ctrl+C Ctrl+V
# please update the following information according to your configuration

INSTANCE=1
PREFIX='/usr/local/etc/oai'
sudo mkdir -m 0777 -p $PREFIX
cp ../../etc/nrf.conf  $PREFIX

declare -A NRF_CONF

NRF_CONF[@INSTANCE@]=$INSTANCE
NRF_CONF[@PREFIX@]=$PREFIX
NRF_CONF[@PID_DIRECTORY@]='/var/run'

NRF_CONF[@NRF_INTERFACE_NAME_FOR_SBI@]='wlo1'

NRF_CONF[@NRF_INTERFACE_PORT_FOR_SBI@]='8080'
NRF_CONF[@NRF_INTERFACE_HTTP2_PORT_FOR_SBI@]='9090'
NRF_CONF[@NRF_API_VERSION@]='v1'


for K in "${!NRF_CONF[@]}"; do 
  egrep -lRZ "$K" $PREFIX | xargs -0 -l sed -i -e "s|$K|${NRF_CONF[$K]}|g"
  ret=$?;[[ ret -ne 0 ]] && echo "Tried to replace $K with ${NRF_CONF[$K]}"
done
