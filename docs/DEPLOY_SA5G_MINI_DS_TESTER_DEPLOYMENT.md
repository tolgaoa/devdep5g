<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Minimalist Deployment using Docker-Compose</font></b>
    </td>
  </tr>
</table>


![SA dsTest Demo](./images/docker-compose/5gCN-mini.jpg)


**OVERVIEW**

This tutorial will help in understanding how to deploy a `minimalist` OAI core network using docker-compose.

* In this tutorial we have used [dsTest](https://www.developingsolutions.com/products/dstest-5g-core-network-testing/), a commercial paid gNB emulator.
* Though, instead of this, readers can also use `gNBsim` (an opensource gNB emulator), you can follow another tutorial for [this](./DEPLOY_SA5G_WITH_GNBSIM.md).
* Readers could also replace the RAN emulator by a real RAN, it means the `so-called dsTest Host` could be a real `gNB`.

Please follow the tutorial step by step to create a stable working testbed. 

**Reading time: ~20mins**

**Tutorial replication time: ~1hr**

**TABLE OF CONTENTS**

1.  [Minimalist Deployment Flavours](#1-minimalist-deployment-flavours)
2.  [Pre-requisites](#2-pre-requisites)
3.  [Network Function Container Images](#2-network-function-container-images)
4.  [Configuring Host Machines](#4-configuring-host-machines)
5.  [Configuring OAI 5G Core Network Functions](#5-configuring-the-oai-5g-core-network-functions)
6.  [Configuring dsTest Scenario](#6-configuring-dstester-scenario)
7.  [Deploying OAI 5G Core Network](#7-deploying-oai-5g-core-network)
8.  [Executing dsTest Scenario](#8-executing-the-dstest-scenario)
9.  [Analysing Scenario Results](#9-analysing-the-scenario-results)
10. [Demo Video](#10-demo-video)
11. [Notes](#11-notes)

## 1. Minimalist Deployment Flavours ##

The `Minimalist` functional 5g core network can be deployed into 2 scenarios:

    - Scenario I:  AMF, SMF, UPF (SPGWU), NRF, MYSQL
    - Scenario II:  AMF, SMF, UPF (SPGWU), MYSQL

## 2. Pre-requisites ##

The container images are built using `docker build` command on Ubuntu 18.04 host machine. The base image for all the containers is Ubuntu 18.04. 

The requried softwares and their respected versions are listed below. To replicate the testbed use these versions. 


| Software                   | Version                         |
|:-------------------------- |:------------------------------- |
| docker engine              | 19.03.6, build 369ce74a3c       |
| docker-compose             | 1.27.4, build 40524192          |
| Host operating system      | Ubuntu 18.04.4 LTS              |
| Container operating system | Ubuntu 18.04                    |
| dsTest (Licensed)          | 5.5                             |
| tshark                     | 3.4.4 (Git commit c33f6306cbb2) |
| wireshark                  | 3.4.4 (Git commit c33f6306cbb2) |

### 2.1. Wireshark ###

The new version of `wireshark` may not be available in the ubuntu repository. 

- So it is better to build it from source.

You may also use the developer PPA:

```bash
sudo add-apt-repository ppa:wireshark-dev/stable
sudo apt update
sudo apt install wireshark

wireshark --version
Wireshark 3.4.7 (Git v3.4.7 packaged as 3.4.7-1~ubuntu18.04.0+wiresharkdevstable1)
```

### 2.2. Networking considerations ###

Most of the times the `docker-compose-host` machine is not configured with packet forwarding then it can be done using below command (if you have already done it in any other section then don't repeat).

**This is the most important step towards end-to-end connectivity.**

```bash
(docker-compose-host)$ sudo sysctl net.ipv4.conf.all.forwarding=1
(docker-compose-host)$ sudo iptables -P FORWARD ACCEPT
```

To know how to configure the machine with the above requirements vist [pre-requisites](./DEPLOY_PRE_REQUESITES.md) page.

## 3. Network Function Container Images ##

- In this demo the network function branch and tags which were used are listed below, follow the [Retrieving images](./RETRIEVE_OFFICIAL_IMAGES.md) or the [Building images](./BUILD_IMAGES.md) to build images with below tags.

| CNF Name    | Branch Name | Tag      | Ubuntu 18.04 | RHEL8 (UBI8)    |
| ----------- | ----------- | -------- | ------------ | ----------------|
| AMF         | `master`    | `v1.2.0` | X            | X               |
| SMF         | `master`    | `v1.2.0` | X            | X               |
| NRF         | `master`    | `v1.2.0` | X            | X               |
| SPGW-U-TINY | `master`    | `v1.1.3` | X            | X               |

- In case readers are interested in making images using different branch then **they have to build images from scratch they can't use the docker-hub images**.

## 4. Configuring Host Machines ##

All the network functions are connected using `demo-oai` bridge.

There are two ways to create this bridge either manually or automatically using docker-compose.

* The manual version will allow packet capturing while network functions are getting deployed. So the initial tested setup packets can be captured for debugging purposes or checking if network functions registered properly to NRF. 
* The second option of automatic deployment is good when initial packet capture is not important.

**NOTE** This tutorial needs that the bridge is created manually to analyse NRF packet exchange. 

### 4.1 Creating bridge manually

Since this is not the `default` behavior, you **SHALL** edit the docker-compose file.

- The bottom section of [docker-compose file](../docker-compose/docker-compose-mini-nrf.yaml) SHALL look like this:

```
    networks:
          public_net:
              external:
                  name: demo-oai-public-net
        # public_net:
        #     driver: bridge
        #     name: demo-oai-public-net
        #     ipam:
        #         config:
        #             - subnet: 192.168.70.128/26
        #     driver_opts:
        #         com.docker.network.bridge.name: "demo-oai"
```

- The `docker-compose-host` machine needs to be configured with `demo-oai` bridge before deploying core network components. To capture initial message exchange between smf<-->nrf<-->upf.

    ```bash
    (docker-compose-host)$ docker network create \
      --driver=bridge \
      --subnet=192.168.70.128/26 \
      -o "com.docker.network.bridge.name"="demo-oai" \
      demo-oai-public-net
    455631b3749ccd6f10a366cd1c49d5a66cf976d176884252d5d88a1e54049bc5
    (docker-compose-host)$ ifconfig demo-oai
    demo-oai: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
            inet 192.168.70.129  netmask 255.255.255.192  broadcast 192.168.70.191
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
    (docker-compose-host)$ docker network ls
    NETWORK ID          NAME                  DRIVER              SCOPE
    d2d34e05bb2d        bridge                bridge              local
    455631b3749c        demo-oai-public-net   bridge              local
    ```

### 4.2 Create bridge automatically  

- Though the bridge can be automatically created using docker-compose file if there is no need to capture initial packets.

This is the `default` version in the [docker-compose-mini-nrf.yaml](../docker-compose/docker-compose-mini-nrf.yaml) or `docker-compose-mini-nonrf.yaml`.

The bottom section SHALL look like this:

    ```
    networks:
        # public_net:
        #     external:
        #         name: demo-oai-public-net
          public_net:
              driver: bridge
              name: demo-oai-public-net
              ipam:
                  config:
                      - subnet: 192.168.70.128/26
              driver_opts:
                  com.docker.network.bridge.name: "demo-oai"
    ```

### 4.3 In case you forgot. True for manual or automatic network creation.

- If the `docker-compose-host` machine is not configured with packet forwarding then it can be done using below command (**important step**), 

    ```bash
    (docker-compose-host)$ sudo sysctl net.ipv4.conf.all.forwarding=1
    (docker-compose-host)$ sudo iptables -P FORWARD ACCEPT
    ```

- The `dsTest-host` needs to configured with a route to reach `docker-compose-host`. Assuming `dsTest-host` physical interface which is connected with `docker-compose-host` is NIC1 and the ip-address of this interface is IP_ADDR_NIC1 then,

    ```bash
    (dsTest-host)$ sudo ip route add route 192.168.70.128/26 \
                           via IP_ADDR_NIC1\
                           dev NIC1_NAME
    ```

- To verify ping the ip-address of the `docker-compose-host` interface connected to demo-oai bridge, if possible also ping amf from the dsTest-host machine.

    ```bash
    (dsTest-host)$ ping 192.168.70.129
    PING 192.168.70.129 (192.168.70.129) 56(84) bytes of data.
    64 bytes from 192.168.70.129: icmp_seq=1 ttl=64 time=0.260 ms
    64 bytes from 192.168.70.129: icmp_seq=2 ttl=64 time=0.147 ms
    64 bytes from 192.168.70.129: icmp_seq=3 ttl=64 time=0.187 ms
    64 bytes from 192.168.70.129: icmp_seq=4 ttl=64 time=0.187 ms
    64 bytes from 192.168.70.129: icmp_seq=5 ttl=64 time=0.181 ms
    ^C
    --- 192.168.70.129 ping statistics ---
    5 packets transmitted, 5 received, 0% packet loss, time 108ms
    rtt min/avg/max/mdev = 0.147/0.192/0.260/0.038 ms    
    ```
    
## 5. Configuring the OAI-5G Core Network Functions ##

### 5.1. Core Network Configuration ###

The docker-compose file has configuration parameters of each core network component. The file is pre-configured with parameters related to this scenario. The table contains the location of the configuration files. These files contains allowed configurable parameters. **Keep checking this file it is possible that we will add new parameters for new features.**  

| File Name   | Repository                                   | Location        |
| ----------- | -------------------------------------------- | --------------- |
| amf.conf    | (Gitlab) cn5g/oai-cn5g-amf                   | [etc/amf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-amf/-/blob/master/etc/amf.conf)    |
| smf.conf    | (Gitlab) cn5g/oai-cn5g-smf                   | [etc/smf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-smf/-/blob/master/etc/smf.conf)    |
| nrf.conf    | (Gilab) cn5g/oai-cn5g-nrf                    | [etc/nrf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-nrf/-/blob/master/etc/nrf.conf)   |
| spgw_u.conf | (Github) OPENAIRINTERFACE/openair-spgwu-tiny | [etc/spgw_u.conf](https://github.com/OPENAIRINTERFACE/openair-spgwu-tiny/blob/master/etc/spgw_u.conf) |

### 5.2. User Subscprition Profile ###

The dsTest UE which will request for a PDU session will have this user profile. Verify that this entry is present in the oai_db1.sql file located in [docker-compose/oai-db1.sql](../docker-compose/oai_db1.sql).  

    ```
    IMSI - 208950000000031
    IMEI - 55000000000001
    Secret Key (K) - 0x0C0A34601D4F07677303652C0462535B
    OPc - 0x63bfa50ee6523365ff14c1f45f88737d
    ``` 

- **Optional**: Incase, the user subscription entry is missing from oai_db.sql file then it can be added using below commands,

    ```bash
    #Login to mysql container once the container is running
    (docker-compose-host)$ docker exec -it mysql /bin/bash
    (mysql-container)$ mysql -uroot -plinux -D oai_db
    mysql> INSERT INTO users VALUES
    ('208950000000031','380561234567','55000000000001',NULL,'PURGED',50,40000000,100000000,47,0000000000,1,0x0C0A34601D4F07677303652C0462535B,0,0,0x40,'ebd07771ace8677a',0x63bfa50ee6523365ff14c1f45f88737d);
    ```

## 6. Configuring DsTester Scenario ##

- **User Subscription Profile**: The user profile used for dsTest scenario is below. Verify that this entry is present in the oai_db1.sql file located in docker-compose/oai_db1.sql.  

    ```
    IMSI - 208950000000031
    IMEI - 55000000000001
    Secret Key (K) - 0x0C0A34601D4F07677303652C0462535B
    OPc - 0x63bfa50ee6523365ff14c1f45f88737d
    ```

- **gNB Parameters for dsTest**: 

    ```
    TAC - 0xa000
    MCC - 208
    MNC - 95
    NSSAI SST - 222
    NSSAI SD - 123
    ```

- [SmartEvents State Machine](https://www.developingsolutions.com/Help/Topics/SmartFlow-SmartEvents-State-Machines.htm) used for this dsTest scenario is below, the number on each arrow between different states depicts transition interval in milli seconds. 

![SmartEvent State Machine](./images/docker-compose/smartevent.png)



## 7. Deploying OAI 5g Core Network ##

- The core network is deployed using a [python script](../docker-compose/core-network.py) which is a wrapper around `docker-compose` and `docker` command. 
- The script informs the user when the core-network is correctly configured by checking health status of containers and connectivity between different core network components.
- In case if there is a problem in using the script then use docker-compose manually read the [notes section](#11-notes)
- If the script is executed without any arguments then the helper menu is visible 

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
    (docker-compose-host)$ sudo tshark -i demo-oai \
         -f "not arp and not port 53 and not host archive.ubuntu.com and not host security.ubuntu.com" \
         -w /tmp/5gcn-mini-deployment-nrf.pcap
    ```

- Explanation on the capture filter:
   *  `not arp` : Not capturing ARP traffic
   *  `not port 53` : Not capturing DNS traffic
   *  `not host archive.ubuntu.com and not host security.ubuntu.com` : Not capturing traffic from `oai-ext-dn` container when installing tools
- Starting the core network components, 

    ```bash
    (docker-compose-host)$ $ python3 ./core-network.py --type start-mini --fqdn no --scenario 1
    [2021-09-14 16:43:45,980] root:DEBUG:  Starting 5gcn components... Please wait....
    Creating oai-nrf ... done
    Creating mysql   ... done
    Creating oai-amf ... done
    Creating oai-smf ... done
    Creating oai-spgwu ... done
    Creating oai-ext-dn ... done
    
    [2021-09-14 16:44:10,098] root:DEBUG:  OAI 5G Core network started, checking the health status of the containers... takes few secs....
    [2021-09-14 16:44:47,025] root:DEBUG:  All components are healthy, please see below for more details....
    Name                 Command                  State                  Ports            
    -----------------------------------------------------------------------------------------
    mysql        docker-entrypoint.sh mysqld      Up (healthy)   3306/tcp, 33060/tcp         
    oai-amf      /bin/bash /openair-amf/bin ...   Up (healthy)   38412/sctp, 80/tcp, 9090/tcp
    oai-ext-dn   /bin/bash -c  apt update;  ...   Up                                         
    oai-nrf      /bin/bash /openair-nrf/bin ...   Up (healthy)   80/tcp, 9090/tcp            
    oai-smf      /bin/bash /openair-smf/bin ...   Up (healthy)   80/tcp, 8805/udp, 9090/tcp  
    oai-spgwu    /openair-spgwu-tiny/bin/en ...   Up (healthy)   2152/udp, 8805/udp
    [2021-09-14 16:44:47,025] root:DEBUG:  Checking if the containers are configured....
    [2021-09-14 16:44:47,025] root:DEBUG:  Checking if SMF and UPF registered with nrf core network....
    [2021-09-14 16:44:47,059] root:DEBUG:  For example: oai-smf Registration with oai-nrf can be checked on this url /nnrf-nfm/v1/nf-instances?nf-type="SMF" {"_links":{"item":[{"href":"192.168.70.133"}],"self":""}}....
    [2021-09-14 16:44:47,059] root:DEBUG:  SMF and UPF are registered to NRF....
    [2021-09-14 16:44:47,059] root:DEBUG:  Checking if SMF is able to connect with UPF....
    [2021-09-14 16:44:47,176] root:DEBUG:  UPF receiving heathbeats from SMF....
    [2021-09-14 16:44:47,176] root:DEBUG:  OAI 5G Core network is configured and healthy....
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
    (docker-compose-host)$ docker logs oai-amf > amf.log
    (docker-compose-host)$ docker logs oai-smf > smf.log
    (docker-compose-host)$ docker logs oai-nrf > nrf.log
    (docker-compose-host)$ docker logs oai-spgwu > spgwu.log  
    (docker-compose-host)$ python3 ./core-network.py --type stop-mini
    [2021-09-14 16:46:45,137] root:DEBUG:  UnDeploying OAI 5G core components....
    Stopping oai-ext-dn ... done
    Stopping oai-spgwu  ... done
    Stopping oai-smf    ... done
    Stopping oai-amf    ... done
    Stopping mysql      ... done
    Stopping oai-nrf    ... done
    Removing oai-ext-dn ... done
    Removing oai-spgwu  ... done
    Removing oai-smf    ... done
    Removing oai-amf    ... done
    Removing mysql      ... done
    Removing oai-nrf    ... done
    Network demo-oai-public-net is external, skipping
    
    [2021-09-14 16:47:44,070] root:DEBUG:  OAI 5G core components are UnDeployed....
    ```

- If you have chosen the `manual` networking option, do not forget to remove the network as well:

    ```bash
    (docker-compose-host)$ docker network rm demo-oai-public-net 
    demo-oai-public-net
    ```

## 9. Analysing the Scenario Results ##

This section is subdivided in two parts:

- the first part for analysing the message exchange between core network components at the time of deployment.
- the second part, for analysing the dsTest stimuli (ie gNB and UE connections).

| Container     | Ip-address     |
| ------------- |:-------------- |
| mysql         | 192.168.70.131 |
| oai-amf       | 192.168.70.132 |
| oai-smf       | 192.168.70.133 |
| oai-nrf       | 192.168.70.130 |
| oai-spgwu     | 192.168.70.134 |
| oai-ext-dn    | 192.168.70.135 |
| Host Machine  | 192.168.70.129 |
| dsTest gNB/UE | 192.168.18.184 |

We are provided both scenarios (with and without `NRF`) as reference but we will analyze only the with `NRF` scenario.

| PCAP/LOG files for Mini w/ NRF                                                            | PCAP/LOG files for Mini w/o NRF |
| ----------------------------------------------------------------------------------------- | ------------------------------- |
| [5gcn-mini-deployment-nrf.pcap](./results/dsTest/mini-nrf/5gcn-mini-deployment-nrf.pcap)  | [5gcn-mini-deployment-no-nrf.pcap](./results/dsTest/mini-no-nrf/5gcn-mini-deployment-no-nrf.pcap) |
| [amf.log](./results/dsTest/mini-nrf/amf.log)                                              | [amf.log](./results/dsTest/mini-no-nrf/amf.log) |
| [smf.log](./results/dsTest/mini-nrf/smf.log)                                              | [smf.log](./results/dsTest/mini-no-nrf/smf.log) |
| [nrf.log](./results/dsTest/mini-nrf/nrf.log)                                              | |
| [spgwu.log](./results/dsTest/mini-nrf/spgwu.log)                                          | [spgwu.log](./results/dsTest/mini-no-nrf/spgwu.log) |


### 9.1. Analysing initial message exchange ###

Using wireshark, open `5gcn-mini-deployment-nrf.pcap` and use the filter `http || pfcp`

- SMF request to NRF for subscribing UPF registration/de-registration events: Packet `37`, `POST` request 
- SMF registration with NRF: Packet `47`, `PUT` request
- UPF(SPGWU) registration with NRF: Packet `60`, `PUT` request
- NRF notification to SMF for UPF registration: Packet `65`, `POST` request
- SMF <--> UPF PFCP Association Setup request and response: Packets `67`, `73`
- Message exchange between SMF, NRF and UPF can be seen in nrf.log but the name of the network function is replaced with a unique identifier (UUID). 

![Analysing initial message exchange](./images/docker-compose/start.png)


### Analysing scenario execution

Using wireshark, open `5gcn-mini-deployment-nrf.pcap` and use the filter `ngap || http || pfcp || gtp`

- NG Setup request reponse: Packets `311`, `313`
- Initial UE Message registration request: Packet `315`
- Authentication and security processL: Packets `339-372`
- Registration complete: Packet `373`
- PDU session establishment request: Packet `396`
- AMF requesting NRF for SMF address: Packet `384` & `386` GET Request
- AMF <--> SMF PDU session request: Packet `394` & `397` POST Request
- SMF <--> UPF PFCP session establishment: Packet `396` & `402`
- Allocated UE IP-address can be seen in Packet `396`, `413`, `420`

![Scenario execution 1](./images/docker-compose/scenario-1.png)

- UE PDU session release request: Packet `495`
- AMF <--> SMF PDU session release request: Packet `500`
- NGreset : Packet `552`

![Scenario execution 2](./images/docker-compose/scenario-2.png)


## 10. Demo Video ##

- Here is the link to the [youtube video](https://www.youtube.com/watch?v=ENQiwl2EYl8) 

## 11. Notes ##

- The `oai-ext-dn` container is optional and is only required if the user wants to ping the dsTest UE. In general this container is not required except for testing purposes. 
- This tutorial can be taken as reference to test the OAI 5G core with a COTS UE. The configuration files has to be changed according to the gNB and COTS UE information should be present in the mysql database. 
- Generally, in a COTS UE two PDN sessions are created by default so configure the IMS in SMF properly. Currently some parameters can not be configured via [docker-compose-mini-nrf.yaml](../docker-compose/docker-compose-mini-nrf.yaml). We recommend you directly configure them in the conf file and mount the file in the docker during run time. 
- It is not necessary to use [core-network.py](../docker-compose/core-network.py) python script, it is possible to directly deploy using `docker-compose` command
- In case you want to deploy debuggers/developers core network environment with more logs please follow [this tutorial](./DEBUG_5G_CORE.md)

    ```
    #To start the containers 
    docker-compose -f <file-name> -p <project-name> up -d
    #To check their health status
    docker-compose -f <file-name> -p <project-name> ps -a
    #To stop the containers 
    docker-compose -f <file-name> -p <project-name> down
    ```
