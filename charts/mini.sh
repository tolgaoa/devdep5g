helm install mysql mysql/ -n oai
kubectl wait --for=condition=available --timeout=200s deployment/mysql -n oai
mysqlIP=$(kubectl get pods -n oai -o wide| grep mysql | awk '{print $6}');


helm install nrf oai-nrf/ -n oai
sleep 5
sed -i "/mySqlServer/c\  mySqlServer: \"$mysqlIP\"" oai-udr/values.yaml
helm install udr oai-udr/ -n oai 
sleep 5
helm install udm oai-udm/ -n oai 
sleep 5 
helm install ausf oai-ausf/ -n oai 
sleep 5
helm install amf oai-amf/ -n oai 
sleep 5
helm install smf oai-smf/ -n oai 
sleep 5
helm install upf oai-spgwu-tiny/ -n oai 

