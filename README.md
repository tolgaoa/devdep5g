# OAI 5G Core Traffic Emulation with Real Life User Patterns
 
This is the implementation of the OpenAirInterface 5G core network and gNBSIM RAN in a Kubernetes cluster at a large-scale in order emulate various user traffic patterns pertaining to real-life use cases. [Globecom 2022] 

------------------------------------------------------------------------------
## Infrastructure Setup

The infrastructure that we used for the large-scale deployments in given below.

![Alt text](infra.png?raw=true)

Our testbed is comprised of a PowerEdge T640 Server with 2 x Intel Xeon Gold 6240R 2.4G CPUs, 768 GB RAM, 2TB disk space and two Precision 7920 Tower servers with 2 x Intel Xeon Gold 5218R 2.1GHz CPUs, 512GB RAM, 1TB disk space. All the hosts (VMs and baremetal are running Ubuntu 20.04). 

------------------------------------------------------------------------------
## Feature Set




## Deploying the Cluster and Running the Users

### Step 1: Setting up the K8s Cluster

In this deployment, the K8s cluster is set up using the Ranchers Kubernetes Engine (RKE) which can be installed following this [link](https://rancher.com/docs/rke/latest/en/installation/). Certain RKE versions can only setup certain versions of Kubernetes. We used rke 1.2.11 with Kubernetes 1.19.

------------------------------------------------------------------------------
**Warning**
Using Ubuntu 22.04 might cause issues with the SSH server used during the RKE cluster setup process
------------------------------------------------------------------------------

Once the RKE binary is setup, the [cluster.yaml](cluster.yml) file is used in order to setup the K8s cluster. The given file shows how to configure multiple nodes to be used in the same cluster. In order to adapt the yaml to one's own environment, change the following parameters:

 
1. Start by copying this repo to the workstation.

2. Docker installed on the target node
3. On the target node, set "AllowTCPForwarding" and "PermitRootLogin" to **yes** from /etc/ssh/sshd.conf  
4. On the target node, add current user, which will be used by the RKE to access the docker daemon to the docker group 
```
$ sudo usermod -aG docker $USER
```
5. Make the necessary modifications to the cluster.yaml
a) IP addresses of the target nodes
b) If using a network abstraction with multiple subnets (as is the case with most OpenStack setups), change the "internal_address" variable for the pertaining subnet
c) The ssh key path to the key that is copied on all the hosts
d) The host usernames

 From the directory that contains the cluster.yaml file, run
```
$ rke up
```
### Step 2: Preparing the Cluster
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
  sudo tar -xvf go1.17.linux-amd64.tar.gz
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

### Step 3: Install Helm on your Workstation
This version of Kubernetes doesn't support the latest helm version. We used helm v3.5.2 for this reason.
```
$ wget https://get.helm.sh/helm-v3.5.2-linux-amd64.tar.gz \
tar -zxvf helm-v3.5.2-linux-amd64.tar.gz \
sudo mv linux-amd64/helm /usr/local/bin/helm
```
### Step 4: Deploy
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
$ ./slice_conf<x>.sh
```
 


 
 
 
 


 



