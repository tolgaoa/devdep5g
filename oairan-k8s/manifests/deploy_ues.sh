#!/bin/bash
count=1
#for count in 3
#do
#-------------------------------UE Modifications--------------------------------------------------------

sed -i "/192.168.18.19/c\                        \"address\": \"192.168.18.19$count/24\"," oai-ue1/04_multus.yaml
sed -i "/tolgaomeratalay/c\        image: tolgaomeratalay/oai-nrue:2.0.$count" oai-ue1/05_deployment.yaml
sed -i "4s/.*/  name: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "6s/.*/    app: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "11s/.*/      app: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "17s/.*/        app: oai-ue$count/" oai-ue1/05_deployment.yaml
sed -i "61s/.*/            name: oai-ran$count/" oai-ue1/05_deployment.yaml
sed -i "68s/.*/        - name: oai-ran$count/" oai-ue1/05_deployment.yaml
sed -i "70s/.*/            claimName: oai-ran-claim$count/" oai-ue1/05_deployment.yaml

kubectl apply -k oai-ue1/

#done
