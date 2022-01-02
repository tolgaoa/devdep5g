<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Basic Deployment with FQDN enabled using Docker-Compose</font></b>
    </td>
  </tr>
</table>


![SA dsTest Demo](./images/docker-compose/5gCN-basic.jpg)

**OVERVIEW**

This tutorial will help in understanding how to deploy a `basic` OAI core network using docker-compose.

* In this tutorial we have used [dsTest](https://www.developingsolutions.com/products/dstest-5g-core-network-testing/), a commercial paid gNB emulator.
* Though, instead of this, readers can also use `gNBsim` (an opensource gNB emulator), you can follow another tutorial for [this](./DEPLOY_SA5G_WITH_GNBSIM.md).
* Readers could also replace the RAN emulator by a real RAN, it means the `so-called dsTest Host` could be a real `gNB`.

Please follow the tutorial step by step to create a stable working testbed. 

**Reading time: ~20mins**

**Tutorial replication time: ~1hr**

Note: In case readers are interested in deploying debuggers/developers core network environment with more logs please follow [this tutorial](./DEBUG_5G_CORE.md)

**TABLE OF CONTENTS**

1.  [Basic Deployment Flavours](#1-basic-deployment-flavours)
2.  [Pre-requisites](#2-pre-requisites)
3.  [Network Function Container Images](#3-network-function-container-images)
4.  [Configuring Host Machines](#4-configuring-host-machines)
5.  [Configuring OAI 5G Core Network Functions](#5-configuring-the-oai-5g-core-network-functions)
6.  [Configuring dsTest Scenario](#6-configuring-dstester-scenario)
7.  [Deploying OAI 5G Core Network](#7-deploying-oai-5g-core-network)
8.  [Executing dsTest Scenario](#8-executing-the-dstest-scenario)
9.  [Reference Logs](#9-reference-logs)

## 1. Basic FQDN Deployment Flavours ##

In this tutorial we use FQDN (Fully Qualified Domain Name) feature.

The Basic functional 5g core network supports the following scenarios:

    - Scenario I:  AMF, SMF, UPF (SPGWU), NRF, UDM, UDR, AUSF, MYSQL
    - Scenario II:  AMF, SMF, UPF (SPGWU), UDM, UDR, AUSF, MYSQL

## 2. Pre-requisites ##

This section is similar to the [Section 2 in the basic-deployment](./DEPLOY_SA5G_BASIC_DS_TESTER_DEPLOYMENT.md#2-pre-requisites).

## 3. Network Function Container Images ##

This section is similar to the [Section 3 in the basic-deployment](./DEPLOY_SA5G_BASIC_DS_TESTER_DEPLOYMENT.md#3-network-function-container-images).

## 4. Configuring Host Machines ##

This section is similar to the [Section 4 in the minimalist-deployment](./DEPLOY_SA5G_MINI_DS_TESTER_DEPLOYMENT.md#4-configuring-host-machines).

## 5. Configuring the OAI-5G Core Network Functions ##

### 5.1. Core Network Configuration ###

This section is similar to the [Section 5.1 in the minimalist-deployment](./DEPLOY_SA5G_MINI_DS_TESTER_DEPLOYMENT.md#51-core-network-configuration).

### 5.2. User Subscprition Profile ###

This section is similar to the [Section 5.2 in the minimalist-deployment](./DEPLOY_SA5G_MINI_DS_TESTER_DEPLOYMENT.md#52-user-subscprition-profile).

## 6. Configuring DsTester Scenario ##

This section is similar to the [Section 6 in the minimalist-deployment](./DEPLOY_SA5G_MINI_DS_TESTER_DEPLOYMENT.md#6-configuring-dstester-scenario).

## 7. Deploying OAI 5g Core Network ##

- The core network is deployed using a [python script](../docker-compose/core-network.py) which is a wrapper around `docker-compose` and `docker` command. 
- The script informs the user when the core-network is correctly configured by checking health status of containers and connectivity between different core network components.
- To know how to use the script look for the helper menu as shown below.
- There are three parameters, which can be provided 
  - `--type` mandatory option to start/stop the 5g core components with minimum/basic functional architecture. 
  - `--fqdn` and `--scenario` are optional and if not provided by default it is considered as use fqdn feature with nrf component.
- In case if there is a problem in using the script then use docker-compose manually read the [notes section](#10-notes) 

    ```bash
    (docker-compose-host)$ pwd
    /home/<docker-compose-host>/oai/oai-cn-fed/docker-compose
    (docker-compose-host)$ python3 core-network.py --help

    usage: core-network.py [-h] --type {start-mini,start-basic,stop-mini,stop-basic} [--fqdn {yes,no}] [--scenario {1,2}]

    OAI 5G CORE NETWORK DEPLOY

    optional arguments:
    -h, --help            show this help message and exit
    --type {start-mini,start-basic,stop-mini,stop-basic}, -t {start-mini,start-basic,stop-mini,stop-basic}
                            Functional type of 5g core network ("start-mini"|"start-basic"|"stop-mini"|"stop-basic")
    --fqdn {yes,no}, -fq {yes,no}
                            Deployment scenario with FQDN ("yes"|"no")
    --scenario {1,2}, -s {1,2}
                            Scenario with NRF ("1") and without NRF ("2")

    example:
            python3 core-network.py --type start-mini
            python3 core-network.py --type start-basic
            python3 core-network.py --type stop-mini
            python3 core-network.py --type start-mini --fqdn no --scenario 2
            python3 core-network.py --type start-basic --fqdn no --scenario 2
    ```
- Before executing the script it is better to start capturing packets to see the message flow between smf <--> nrf <--> upf. The packets will be captured on **demo-oai** bridge which should be configured on the `docker-compose-host` machine. 

    ```bash
    (docker-compose-host)$ sudo tshark -i demo-oai 
         -f "not arp and not port 53 and not host archive.ubuntu.com and not host security.ubuntu.com" \
         -w /tmp/5gcn-basic-deployment-fqdn.pcap
    ```

- Explanation on the capture filter:
   *  `not arp` : Not capturing ARP traffic
   *  `not port 53` : Not capturing DNS traffic
   *  `not host archive.ubuntu.com and not host security.ubuntu.com` : Not capturing traffic from `oai-ext-dn` container when installing tools
- Starting the core network components, 

    ```bash
    (docker-compose-host)$ python3 core-network.py --type start-basic --fqdn yes --scenario 1
    [2021-09-20 14:14:50,537] root:DEBUG:  Starting 5gcn components... Please wait....
    Creating mysql   ... done
    Creating oai-nrf ... done
    Creating oai-udr ... done
    Creating oai-udm ... done
    Creating oai-ausf ... done
    Creating oai-amf  ... done
    Creating oai-smf  ... done
    Creating oai-spgwu ... done
    Creating oai-ext-dn ... done
    
    [2021-09-20 14:15:25,368] root:DEBUG:  OAI 5G Core network started, checking the health status of the containers... takes few secs....
    [2021-09-20 14:16:02,726] root:DEBUG:  All components are healthy, please see below for more details....
    Name                 Command                  State                  Ports            
    -----------------------------------------------------------------------------------------
    mysql        docker-entrypoint.sh mysqld      Up (healthy)   3306/tcp, 33060/tcp         
    oai-amf      /bin/bash /openair-amf/bin ...   Up (healthy)   38412/sctp, 80/tcp, 9090/tcp
    oai-ausf     /bin/bash /openair-ausf/bi ...   Up (healthy)   80/tcp                      
    oai-ext-dn   /bin/bash -c  apt update;  ...   Up                                         
    oai-nrf      /bin/bash /openair-nrf/bin ...   Up (healthy)   80/tcp, 9090/tcp            
    oai-smf      /bin/bash /openair-smf/bin ...   Up (healthy)   80/tcp, 8805/udp, 9090/tcp  
    oai-spgwu    /openair-spgwu-tiny/bin/en ...   Up (healthy)   2152/udp, 8805/udp          
    oai-udm      /bin/bash /openair-udm/bin ...   Up (healthy)   80/tcp                      
    oai-udr      /bin/bash /openair-udr/bin ...   Up (healthy)   80/tcp
    [2021-09-20 14:16:02,727] root:DEBUG:  Checking if the containers are configured....
    [2021-09-20 14:16:02,727] root:DEBUG:  Checking if SMF and UPF registered with nrf core network....
    [2021-09-20 14:16:02,762] root:DEBUG:  For example: oai-smf Registration with oai-nrf can be checked on this url /nnrf-nfm/v1/nf-instances?nf-type="SMF" {"_links":{"item":[{"href":"192.168.70.133"}],"self":""}}....
    [2021-09-20 14:16:02,763] root:DEBUG:  SMF and UPF are registered to NRF....
    [2021-09-20 14:16:02,763] root:DEBUG:  Checking if SMF is able to connect with UPF....
    [2021-09-20 14:16:02,878] root:DEBUG:  UPF receiving heathbeats from SMF....
    [2021-09-20 14:16:02,878] root:DEBUG:  OAI 5G Core network is configured and healthy....
    ```

## 8. Executing the dsTest Scenario ##

- **Scenario Execution**: On the dsTest host run the scenario either using the dsClient GUI or command line. Below are the commands to run it using the command line.

    ```bash
    (dsTest-host)$ dsClient -d 127.0.0.1 -c "source dsTestScenario.xml"
    ```
- **Verify PDN session establishment**: To check if a PDN session is properly estabilished there is an extra external data network container only for this demo purpose. The dsTest UE can be reached using this container to validate the PDN session establishment. To understand the packet flow read the next analysis section. In our settings the UE network is 12.1.1.0/24 the configuration can be seen in smf.conf and spgw_u.conf. The allocated IP address to dsTest UE can be seen in smf logs. Generally, if there is a single UE then the allocated ip address will be 12.1.1.2.

    ```bash
    (docker-compose-host)$ docker exec -it oai-ext-dn ping 12.1.1.2
    64 bytes from 12.1.1.2: icmp_seq=3 ttl=63 time=0.565 ms
    64 bytes from 12.1.1.2: icmp_seq=4 ttl=63 time=0.629 ms
    64 bytes from 12.1.1.2: icmp_seq=5 ttl=63 time=0.542 ms
    64 bytes from 12.1.1.2: icmp_seq=6 ttl=63 time=0.559 ms
    ^c
    ```

- **Stop PCAP collection**: Stop the wireshark or tshark process on the docker-compose-host.

- **Undeploy the core network**: Before undeploying collect all the logs from each component for analysis. 

    ```bash
    (docker-compose-host)$ docker logs oai-amf > amf.log 2>&1
    (docker-compose-host)$ docker logs oai-smf > smf.log 2>&1
    (docker-compose-host)$ docker logs oai-nrf > nrf.log 2>&1
    (docker-compose-host)$ docker logs oai-spgwu > spgwu.log   2>&1
    (docker-compose-host)$ docker logs oai-udr > udr.log 2>&1
    (docker-compose-host)$ docker logs oai-udm > udm.log 2>&1
    (docker-compose-host)$ docker logs oai-ausf > ausf.log 2>&1
    (docker-compose-host)$ python3 core-network.py --type stop-basic --fqdn yes --scenario 1
    [2021-09-20 14:18:26,489] root:DEBUG:  UnDeploying OAI 5G core components....
    Stopping oai-ext-dn ... done
    Stopping oai-spgwu  ... done
    Stopping oai-smf    ... done
    Stopping oai-amf    ... done
    Stopping oai-ausf   ... done
    Stopping oai-udm    ... done
    Stopping oai-udr    ... done
    Stopping oai-nrf    ... done
    Stopping mysql      ... done
    Removing oai-ext-dn ... done
    Removing oai-spgwu  ... done
    Removing oai-smf    ... done
    Removing oai-amf    ... done
    Removing oai-ausf   ... done
    Removing oai-udm    ... done
    Removing oai-udr    ... done
    Removing oai-nrf    ... done
    Removing mysql      ... done
    Network demo-oai-public-net is external, skipping
    
    [2021-09-20 14:19:52,044] root:DEBUG:  OAI 5G core components are UnDeployed....
    ```

## 9. Reference logs ##


| PCAP/LOG files for Basic w/ NRF                                                                |
|:---------------------------------------------------------------------------------------------- |
| [5gcn-basic-deployment-fqdn.pcap](./results/dsTest/basic-fqdn/5gcn-basic-deployment-fqdn.pcap) |
| [amf.log](./results/dsTest/basic-fqdn/amf.log)                                                 |
| [ausf.log](./results/dsTest/basic-fqdn/ausf.log)                                               |
| [smf.log](./results/dsTest/basic-fqdn/smf.log)                                                 |
| [nrf.log](./results/dsTest/basic-fqdn/nrf.log)                                                 |
| [spgwu.log](./results/dsTest/basic-fqdn/spgwu.log)                                             |
| [udm.log](./results/dsTest/basic-fqdn/udm.log)                                                 |
| [udr.log](./results/dsTest/basic-fqdn/udr.log)                                                 |

