<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Docker Deployment : Configure Containers</font></b>
    </td>
  </tr>
</table>


**TABLE OF CONTENTS**

1.  [Configure](#1-configure-the-containers)
2.  [Deploy](#2-deploy-the-containers)
3.  [Networking](#3-networking)

# 1. Configure the containers #
- **Core Network Configuration**: The [docker-compose](../docker-compose/docker-compose.yaml) file has configuration parameters of all the core network components. The file is pre-configured with parameters related to an [example scenario](./DEPLOY_SA5G_WITH_DS_TESTER.md). The table contains the location of the configuration files. These files contains allowed configurable parameters. **Keep checking this file it is possible that we will add new parameters for new features.**  

    | File Name   | Repository                                   | Location        |
    |:----------- |:-------------------------------------------- |:--------------- |
    | amf.conf    | (Gitlab) cn5g/oai-cn5g-amf                   | [etc/amf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-amf/-/blob/develop/etc/amf.conf)    |
    | smf.conf    | (Gitlab) cn5g/oai-cn5g-smf                   | [etc/smf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-smf/-/blob/develop/etc/smf.conf)    |
    | nrf.conf    | (Gilab) cn5g/oai-cn5g-nrf                    | [etc/nrf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-nrf/-/blob/develop/etc/nrf.conf)   |
    | spgw_u.conf | (Github) OPENAIRINTERFACE/openair-spgwu-tiny | [etc/spgw_u.conf](https://github.com/OPENAIRINTERFACE/openair-spgwu-tiny/blob/gtp_extension_header/etc/spgw_u.conf) |

- The PLMN, TAC, Operator Key should be properly configured in amf.conf according to gNB and UE
- In case of a **COTS UE** by default there are two PDN sessions so check the smf.conf to configure the right names for the IMS. Currently some parameters can not be configured via docker-compose.yaml. We recommend you directly configure them in the conf file and mount the file in the docker during run time. 
    ```
    ## Configure here
        DNN_LIST = (
       # IPV4_POOL, IPV6_POOL are index in IPV4_LIST, IPV6_LIST, PDU_SESSION_TYPE choice in {IPv4, IPv6, IPv4v6}
      {DNN_NI = <name-1>; PDU_SESSION_TYPE = "IPv4"; IPV4_POOL  = 0; IPV6_POOL = -1},
      {DNN_NI = <name-2>; PDU_SESSION_TYPE = "IPv4"; IPV4_POOL = 1; IPV6_POOL = -1},
      {DNN_NI = <name-3>; PDU_SESSION_TYPE = "IPv4"; IPV4_POOL = 2; IPV6_POOL = -1}
    );
    ## Configure here
        LOCAL_CONFIGURATION :
    {
      SESSION_MANAGEMENT_SUBSCRIPTION_LIST = (
         { NSSAI_SST = 222, NSSAI_SD = "123", DNN = "<name-1>", DEFAULT_SESSION_TYPE = "IPV4", DEFAULT_SSC_MODE = 1, 
           QOS_PROFILE_5QI = 6, QOS_PROFILE_PRIORITY_LEVEL = 1, QOS_PROFILE_ARP_PRIORITY_LEVEL = 1, QOS_PROFILE_ARP_PREEMPTCAP = "NOT_PREEMPT", 
           QOS_PROFILE_ARP_PREEMPTVULN = "NOT_PREEMPTABLE", SESSION_AMBR_UL = "20Mbps", SESSION_AMBR_DL = "22Mbps"},
         { NSSAI_SST = 111; NSSAI_SD = "124", DNN = "<name-2>", DEFAULT_SESSION_TYPE = "IPV4", DEFAULT_SSC_MODE = 1, 
           QOS_PROFILE_5QI = 7, QOS_PROFILE_PRIORITY_LEVEL = 1, QOS_PROFILE_ARP_PRIORITY_LEVEL = 1, QOS_PROFILE_ARP_PREEMPTCAP = "NOT_PREEMPT", 
           QOS_PROFILE_ARP_PREEMPTVULN = "NOT_PREEMPTABLE", SESSION_AMBR_UL = "10Mbps", SESSION_AMBR_DL = "11Mbps"}
        );                 
    };  
    ```
- Incase NRF is not needed then it can be disabled in amf.conf,smf.conf and spgw_u.conf. 

    ```
    #smf.conf
    REGISTER_NRF = "no";  # Set to yes if SMF resgisters to an NRF
    DISCOVER_UPF = "no";  # Set to yes to enable UPF discovery and selection
    
    #amf.conf
    NF_REGISTRATION = "no";  # Set to yes if AMF resgisters to an NRF
    SMF_SELECTION   = "no";  # Set to yes to enable SMF discovery and selection
     
    #spgw_u.conf
    REGISTER_NRF = "no";
    ```
- An new user subscription information should present in mysql database before trying to connect the UE . This can be done by adding the UE information in the oai_db.sql file

  ```
  replace the values with your values
  INSERT INTO users VALUES
    (imsi,msisdn,imei,NULL,'PURGED',50,40000000,100000000,47,0000000000,1,key,0,0,0x40,'ebd07771ace8677a',opc);
  ```

- **Optional**: Incase, the user subscription entry is missing from oai_db.sql file then it can be added at run time using below commands,

    ```bash
    #Login to mysql container once the container is running
    (docker-compose-host)$ docker exec -it mysql /bin/bash
    (mysql-container)$ mysql -uroot -plinux -D oai_db
    mysql> INSERT INTO users VALUES
    (imsi,msisdn,imei,NULL,'PURGED',50,40000000,100000000,47,0000000000,1,key,0,0,0x40,'ebd07771ace8677a',opc);
    ```

# 2. Deploy the containers #

- Container deployment has to follow a strict order if the `NRF` is used for `SMF` and `UPF` registration

    ```
    mysql --> oai-nrf --> oai-amf --> oai-smf --> oai-upf
    ```
- If no nrf then

    ```
    mysql --> oai-amf --> oai-smf --> oai-upf
    ```

# 3. Networking #

- The [docker-compose.yaml](../docker-compose/docker-compose.yaml) can create the network automatically, currently in the file automatic network creation is disabled. It can be enabled by uncommenting the lines. 

    ```bash
    networks:
          public_net:
              driver: bridge
              name: demo-oai-public-net
              ipam:
                  config:
                      - subnet: 192.168.70.128/26
              driver_opts:
                  com.docker.network.bridge.name: "demo-oai"
    ```

- In user wants to create manual network then below is the command

components. To capture initial message exchange between smf<-->nrf<-->upf. 

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
            ether 02:42:9c:0a:23:44  txqueuelen 0  (Ethernet)
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
    (docker-compose-host)$ docker network ls
    NETWORK ID          NAME                  DRIVER              SCOPE
    d2d34e05bb2d        bridge                bridge              local
    455631b3749c        demo-oai-public-net   bridge              local
    ```

You are ready to check out the tutorial that [how 5g core works](./DEPLOY_SA5G_WITH_DS_TESTER.md).
