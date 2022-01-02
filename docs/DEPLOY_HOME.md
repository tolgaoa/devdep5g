<a href="https://openairinterface.org/">
    <img src="./images/oai_final_logo.png" alt="Openairinterface logo" title="Openairinterface" align="right" height="60" />
</a>

# OpenAirInterface 5G Core Network Deployment

Welcome to the tutorial home page of OAI 5g Core project. Here you can find lot of tutorials and help manuals. We regularly update these documents depending on the new feature set. 

**2021/09/06 Update: you have now the choice to either pull images or build your-self.**

## Table of content

- [Pre-requisites](./DEPLOY_PRE_REQUESITES.md)
- How to get the container images
    - [Pull the container images](./RETRIEVE_OFFICIAL_IMAGES.md)
    - [Build the container images](./BUILD_IMAGES.md)
- [Configuring the Containers](./CONFIGURE_CONTAINERS.md)
- 5G Core Network Deployment
    - [Using Docker-Compose, perform a `minimalist` deployment](./DEPLOY_SA5G_MINI_DS_TESTER_DEPLOYMENT.md)
    - [Using Docker-Compose, perform a `basic` deployment](./DEPLOY_SA5G_BASIC_DS_TESTER_DEPLOYMENT.md)
    - [Using Docker-Compose, perform a `basic` deployment with FQDN feature](./DEPLOY_SA5G_BASIC_FQDN_DEPLOYMENT.md)
    - [Using Docker-Compose, perform a `basic-vpp` deployment with VPP-implementation of UPF](./DEPLOY_SA5G_WITH_VPP_UPF.md)
    - [Using Helm Chart](./DEPLOY_SA5G_HC.md)
    - [Using Virtual Machine, deploy and test with a Commercial UE](./DEPLOY_SA5G_VM_COTSUE.md)
    - [Using Docker-Compose, perform a `minimalist` deployment and test with `gnbsim`](./DEPLOY_SA5G_WITH_GNBSIM.md)
    - [Using Docker-Compose, perform a `minimalist` deployment and test with OAI RF simulator](https://gitlab.eurecom.fr/oai/openairinterface5g/-/tree/develop/ci-scripts/yaml_files/5g_rfsimulator)
- The Developer Corner
    - [How to Deploy Developers Core Network and Basic Debugging](./DEBUG_5G_CORE.md)
