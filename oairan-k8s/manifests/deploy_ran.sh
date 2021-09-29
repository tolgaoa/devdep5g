#!/bin/bash

#count=1
AMF_BASE=180
UPF_BASE=120

for count in 1 2
do 
AMF_IP=$(( AMF_BASE + count ))
UPF_IP=$(( UPF_BASE + count ))
#----------------------------------gNB Modifications-----------------------------------------------------
# 2- configMap mount    	
sed -i "/name: gnb-configmapmount/c\  name: gnb-configmapmount-$count" oai-gnb1/02_configmapmount.yaml
sed -i "/Active_gNBs/c\                Active_gNBs = ( \"gNB-VTKryptowire-$count\");" oai-gnb1/02_configmapmount.yaml
sed -i "/gNB_ID/c\                    gNB_ID    =  0xe0$count;" oai-gnb1/02_configmapmount.yaml
sed -i "/gNB_name/c\                    gNB_name  =  \"gNB-VTKryptowire-$count\"; " oai-gnb1/02_configmapmount.yaml
sed -i "/amf_ip_address/c\                    amf_ip_address      = ( { ipv4       = \"192.168.18.177\" " oai-gnb1/02_configmapmount.yaml
sed -i "/GNB_IPV4_ADDRESS_FOR_NG_AMF/c\                        GNB_IPV4_ADDRESS_FOR_NG_AMF              = \"192.168.18.177\";" oai-gnb1/02_configmapmount.yaml
sed -i "/GNB_IPV4_ADDRESS_FOR_NGU/c\                        GNB_IPV4_ADDRESS_FOR_NGU                 = \"192.168.18.179\";" oai-gnb1/02_configmapmount.yaml
# 3- services
# 4- persistent volumes
sed -i "/name/c\  name: oai-ran$count" oai-gnb1/04_persistentvolume.yaml
sed -i "/io.kompose.service/c\    io.kompose.service: oai-ran$count" oai-gnb1/04_persistentvolume.yaml
sed -i "/path/c\    path: \"/k8s-data/sharedpvgnb$count/\"" oai-gnb1/04_persistentvolume.yaml
# 5- persistent volume claims
sed -i "/io.kompose.service/c\    io.kompose.service: oai-ran-claim$count" oai-gnb1/04_persistentvolume.yaml
sed -i "/name/c\  name: oai-ran-claim$count" oai-gnb1/05_persistentvolumeclaim.yaml
sed -i "/volumeName/c\  volumeName: oai-ran$count" oai-gnb1/05_persistentvolumeclaim.yaml
# 6- multus interface	
sed -i "/192.168.18.11/c\                       \"address\": \"192.168.18.11$count/24\"," oai-gnb1/06_multus.yaml
sed -i "/172.16.2.11/c\                        \"address\": \"172.16.2.11$count/24\"," oai-gnb1/06_multus.yaml
# 7- deployment
sed -i "6s/.*/    app: oai-gnb$count/" oai-gnb1/07_deployment.yaml
sed -i "4s/.*/  name: oai-gnb$count/" oai-gnb1/07_deployment.yaml
sed -i "11s/.*/      app: oai-gnb$count/" oai-gnb1/07_deployment.yaml
sed -i "17s/.*/        app: oai-gnb$count/" oai-gnb1/07_deployment.yaml
sed -i "36s/.*/          name: oai-gnb$count/" oai-gnb1/07_deployment.yaml
sed -i "121s/.*/            name: oai-ran$count/" oai-gnb1/07_deployment.yaml
sed -i "135s/.*/        - name: oai-ran$count/" oai-gnb1/07_deployment.yaml
sed -i "/claimName: oai-ran-claim/c\            claimName: oai-ran-claim$count" oai-gnb1/07_deployment.yaml
sed -i "/name: gnb-configmapmount/c\            name: gnb-configmapmount-$count" oai-gnb1/07_deployment.yaml

kubectl apply -k oai-gnb1/


#-------------------------------UE Modifications--------------------------------------------------------
 
sed -i "/192.168.18.19/c\                        \"address\": \"192.168.18.19$count/24\"," oai-gnb1/04_persistentvolume.yaml
sed -i "4s/.*/  name: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "6s/.*/    app: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "11s/.*/      app: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "17s/.*/        app: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "61s/.*/            name: oai-ran$count/" oai-ue1/05_deployment.yaml
sed -i "68s/.*/        - name: oai-ran$count/" oai-ue1/05_deployment.yaml
sed -i "70s/.*/            claimName: oai-ran-claim$count/" oai-ue1/05_deployment.yaml

done
