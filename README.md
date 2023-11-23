# OAI 5GC and RAN Deployment in K8s

This branch includes the latest deployment integrated with 5G-STREAM. Enabling "proxy" from the helm charts of the VNFs will enable them to peak with the 5G-STREAM Service Communication Proxy. When disabled, 5G deployment will proceed as usual. 

## Deploy
From within the /charts directory, navigate to the helm templates of each of the following components -- "oai-nrf" , "oai-amf" , "oai-smf" , "oai-spgwu-tiny" -- and change the multus interfaces to the one on your local machine. 

Create the oai namespace.
```
kubectl create ns oai
```

From the oai-5gcn/charts folder execute the following,
```
$ helm install <name-of-mysql-deployment> mysql/ -n oai
```
which will first deploy the MYSQL pod. Once this has finished deploying change the MYSQL server IP parameter in AMF [values.yaml](oai-5gcn/charts/oai-amf/values.yaml). Next there are 8 different scripts that will automate the deployment of various slicing configurations. 
```
$ ./run.sh
```
The run.sh script will deploy the 5G core. Using the inputs to the script (detailed within the script), the number of slices and users can be adjusted.
