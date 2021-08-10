------------------------------------------------------------------------------

# OAI 5GC and RAN Deployment in K8s 
 
This is the implementation of the Openairinterface 5G core network and RAN in a Kubernetes cluster. The instructions given below are a debugged and environment specific variant of the deployment instructions provided at the official gitlab of [openairinterface5g](https://gitlab.eurecom.fr/oai) cn5g and openairinterface5g subgroups.

The steps are given below from start to finish in order to replicate a sample deployment where the 5G core network is deployed. Following this an OAI-gNB is connected to the AMF followed by a OAI-nrUE registration.

------------------------------------------------------------------------------

## Step 1: Setting up the K8s Cluster

In this deployment, the K8s cluster is set up using the Ranchers Kubernetes Engine (RKE) which can be installed following this [link](https://rancher.com/docs/rke/latest/en/installation/).

Once the RKE binary is setup, the [cluster.yaml](cluster.yml) file is used in order to setup the K8s cluster. The current file is used to set up a single node cluster, however additional nodes can be added by uncommenting the specified lines in the cluster.yaml file. To proceed with the installation, the following are the required steps: 
1. A workstation with an ssh key-pair
2. A minimum of one target node with sufficient capacity to install the cluster accessible from the workstation (we have deployed on an ESXi VM Ubuntu 18.04 with 32GB RAM, 8 vCPUs and 180 GB disk space). Note that if you choose to create a cluster with multiple nodes you'll have to change the way the K8s volumes are created because the files will need to be shared across multiple nodes, which will require a network file system. (Coming Soon)
3. Docker installed on the target node
4. On the target node, set "AllowTCPForwarding" and "PermitRootLogin" to **yes** from /etc/ssh/sshd.conf  
5. On the target node, add current user, which will be used by the RKE to access the docker daemon to the docker group 
```
$ sudo usermod -aG docker $USER
```
6. Copy the public key of your workstation to the authorized keys of your target node by running the following on your workstation
```
$ ssh-copy-id -i ~/.ssh/your_key user@hostIP
```
7. Make the necessary modifications to the cluster.yaml (i.e. change the host name(s), host IP(s) and the private key)
 From the directory that contains the cluster.yaml file, run
```
$ rke up
```
## Step 2: Preparing the Cluster
Now that the cluster is deployed, it requires a network plugin for the OAI core and RAN to run on it, which is called Multus. 
1. Set up kubectl on your workstation by following this [link](https://kubernetes.io/docs/tasks/tools/install-kubectl-linux/).
2. The directory that you executed item 7 in Step 1 now contains the "kube_config_cluster.yml file", which is the configuration file of the cluster. Set up the access with
```
$ mkdir -p ~/.kube \
  cp kube_config_cluster.yml ~/.kube/config.oaicnran \
  export KUBECONFIG=~/.kube/config.oaicnran 
```
3. Install the multus networking plugin from [here](https://github.com/k8snetworkplumbingwg/multus-cni) required for the OAI core network and RAN components. You can execute this from the workstation which has kubectl access to the target node where the cluster was set up.
4. Make sure the required container networking plugins are installed on the target node. The installation of these plugins first requires go. 
```
$ wget https://dl.google.com/go/go1.13.3.linux-amd64.tar.gz \ 
  sudo tar -xvf go1.13.3.linux-amd64.tar.gz
  mv go /usr/local 
  export GOROOT=/usr/local/go
  export GOPATH=$HOME/Projects/Proj1
  export PATH=$GOPATH/bin:$GOROOT/bin:$PATH
```
After go is installed,
```
$ git clone https://github.com/containernetworking/plugins.git \
cd plugins \
./build_linux.sh
```
to build the binaries. Once they are built, from the /bin folder inside the /plugins folder, copy {bandwith, bridge, dhcp, host-device, ipvlan, macvlan, ptp, static, vlan, portmap, sbr, tuning} into /opt/cni/bin on your target node.

## Step 3: Install Helm on your Workstation
```
$ wget https://get.helm.sh/helm-v3.5.2-linux-amd64.tar.gz \
tar -zxvf helm-v3.5.2-linux-amd64.tar.gz \
sudo mv linux-amd64/helm /usr/local/bin/helm
```
## Step 4: Deploying the Core Network
From the oai-5gcn folder execute the following,

1. Create the persistent volume required for the mysql database which will be holding the subscriber information of the users. Make sure to change the path of where the volume will be created on the host in the file [create_mysql_volume.yaml](create_mysql_volume.yaml)
```
$ kubectl apply -f create_mysql_volume.yaml
```
2. Create the persistent volume and the persistent volume claim required for the SPGWU. Make sure to change the path of where the volume will be created on the host in the file [create_upf_volume.yaml](create_upf_volume.yaml)
```
$ kubectl apply -f create_upf_volume.yaml
$ kubectl apply -f create_upf_volume_claim.yaml
```
3. Before deploying the components, modify the multus files in the core network helm charts of each of the core network components except for mysql (i.e. /oai-5gcn/charts/_name_of_component_/templates/multus.yaml) by changing the name of the "master" network interface that multus will be using on your target node. (Currently this value has been set to "ens160".)
 
4. Create the "oai" namespace in the cluster 
```
$ kubectl create namespace oai
```
5. Deploy the core network charts in the given order using the newly created namespace. Wait for each deployment to complete before proceeding with the next. From the directory /oai5gcnRAN/oai-5gcn/charts
```
$ helm install <name-of-mysql-deployment> mysql/ -n oai
$ helm install <name-of-nrf-deployment> oai-nrf/ -n oai
$ helm install <name-of-amf-deployment> oai-amf/ -n oai
$ helm install <name-of-smf-deployment> oai-smf/ -n oai
$ helm install <name-of-spgwu-deployment> oai-spgwu-tiny/ -n oai
```
## Step 5: Deploying the RAN 
1. Go into /oairan-k8s/manifests folder. Perform the following modifications in the /oai-gnb
  - In [04_persistentvolume.yaml](oairan-k8s/manifests/oai-gnb/04_persistentvolume.yaml) modify the path of the volume to your environment
  - In [06_multus.yaml](oairan-k8s/manifests/oai-gnb/06_multus.yaml) change the master network interfaces for both the multus networks to the network interface of your taget node
2. Deploy the OAI-GNB. From inside the /oai-gnb folder 
```
$ kubectl apply -k .
```
3. Go into /oairan-k8s/manifests folder. Perform the following modifications in the /oai-ue
  - In [04_multus.yaml](oairan-k8s/manifests/oai-ue/04_multus.yaml) change the master network interfaces for the multus networks to the network interface of your target node
4. Deploy the OAI-nrUE. From inside the /oai-ue folder 
```
$ kubectl apply -k .
```
## Step 6: Running the gNB and nrUE
We'll be running the gNB and UE simulators in the Standalone (SA) mode so that they may connect with our 5G core network instead of the LTE Evolved Packet Core (EPC). 
1. Go into the gNB pod from your workstation
```
$ kubectl exec -it -n oai <pod name> -- bash
```
2. In the /opt/oai-ran/etc folder modify the "gnb.band78.sa.fr1.106PRB.usrpb210.conf" file, make the following modifications. The values given below are obtained from the [values.yaml](oai-5gcn/charts/oai-amf/values.yaml) in the AMF helm charts. If you changed those values before deploying the charts replace the values given below with ones you selected (it should be noted that we have updated the operator key inside the AMF helm charts to match the keys of the UEs hard-coded in the source code)
  - tracking_area_code = 0xa000
  - mcc = 208 , this value comes from the first three digits of the IMSI value hardcoded into the UE modem build in the openairinterface5g source code found [here](https://gitlab.eurecom.fr/oai/openairinterface5g/-/blob/96997d1ecadf3279db64be1b3e97ec56fdb5dee2/openair3/UICC/usim_interface.c)
  - mnc = 99 , this value comes from the 4th, 5th digits of the IMSI value hardcoded into the UE modem build in the openairinterface5g source code 
  - in the snssaiList 
     - first set of sst = 1, sd = 11
     - second set of sst = 2, sd = 12
  - in the amf_ip_address
     - ipv4 = 192.168.18.177
  - in the NETWORK_INTERFACES;=
     - GNB_INTERFACE_NAME_FOR_NG_AMF = net1
     - GNB_IPV4_ADDRESS_FOR_NG_AMF = 192.168.18.177
     - GNB_INTERFACE_NAME_FOR_NGU = net1
     - GNB_IPV4_ADDRESS_FOR_NGU = 192.168.18.179
 
3. Inside the gNB pod from /opt/oai-ran/rfsim folder run 
```
$ RFSIMULATOR=server ../bin/nr-softmodem.Rel15 -O ../etc/gnb.band78.sa.fr1.106PRB.usrpb210.conf --parallel-config PARALLEL_SINGLE_THREAD --rfsim --sa
```
4. From inside the UE pod from /opt/oai-ran/phy folder run 
```
$ RFSIMULATOR=192.168.18.110 ../bin/nr-uesoftmodem.Rel15 --rfsim --sa --rrc_config_path .
```
 
 
 
 


 



