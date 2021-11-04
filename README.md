# OAI 5GC and RAN Deployment in K8s 
 
This is the implementation of the OpenAirInterface 5G core network and gNBSIM RAN in a Kubernetes cluster. 

The steps are given below from start to finish in order to replicate a sample deployment.

------------------------------------------------------------------------------

## Step 1: Setting up the K8s Cluster

In this deployment, the K8s cluster is set up using the Ranchers Kubernetes Engine (RKE) which can be installed following this [link](https://rancher.com/docs/rke/latest/en/installation/). Certain RKE versions can only setup certain versions of Kubernetes. We used rke 1.2.7 with Kubernetes 1.19.

Once the RKE binary is setup, the [cluster.yaml](cluster.yml) file is used in order to setup the K8s cluster. The current file is used to set up a single node cluster, however additional nodes can be specified using the same format. To proceed with the installation, the following are the required steps: 
1. A minimum of one target node with sufficient capacity to install the cluster accessible from the workstation (our cluster runs on 16 Openstack VMs with a total of 220GB RAM and 80 vCPUs, however ). Note that if you choose to create a cluster with multiple nodes you'll have to change the way the K8s volumes are created because the files will need to be shared across multiple nodes, which will require a network file system. 
Start by copying this repo to the workstation.
```
$ git clone https://github.com/chateauxvt/oai5gdep.git
```
2. Docker installed on the target node
3. On the target node, set "AllowTCPForwarding" and "PermitRootLogin" to **yes** from /etc/ssh/sshd.conf  
4. On the target node, add current user, which will be used by the RKE to access the docker daemon to the docker group 
```
$ sudo usermod -aG docker $USER
```
5. Make the necessary modifications to the cluster.yaml (i.e. change the host name(s), host IP(s) and the private key)
 From the directory that contains the cluster.yaml file, run
```
$ rke up
```
## Step 2: Preparing the Cluster
Now that the cluster is deployed, it requires a network plugin for the OAI core and gNBSIM RAN to run on it, which is called Multus. 
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
This version of Kubernetes doesn't support the latest helm version. We used helm v3.5.2 for this reason.
```
$ wget https://get.helm.sh/helm-v3.5.2-linux-amd64.tar.gz \
tar -zxvf helm-v3.5.2-linux-amd64.tar.gz \
sudo mv linux-amd64/helm /usr/local/bin/helm
```
## Step 4: Deploy
From the oai-5gcn folder execute the following,
```
$ helm install <name-of-mysql-deployment> mysql/ -n oai
```
which will first deploy the MYSQL pod. Once this has finished deploying. Change the MYSQL server IP parameter in AMF [values.yaml](oai-5gcn/charts/oai-amf/values.yaml). Next there are five different scripts that will automate the deployment of various slicing configurations. 
 

$ helm install <name-of-nrf-deployment> oai-nrf/ -n oai
$ helm install <name-of-amf-deployment> oai-amf/ -n oai
$ helm install <name-of-smf-deployment> oai-smf/ -n oai
$ helm install <name-of-spgwu-deployment> oai-spgwu-tiny/ -n oai 


 
 
 
 


 



