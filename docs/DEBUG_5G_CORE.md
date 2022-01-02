<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Deployment: How to debug the Network Functions? </font></b>
    </td>
  </tr>
</table>

Currently there is no special tool for debugging the problems of core network components. Though there are certain methods which we are using in our team to diagnose an issue. 

This page content expects you to have read the [deployment pre-requisites](./DEPLOY_PRE_REQUESITES.md) and try to deploy a [mini](./DEPLOY_SA5G_MINI_DS_TESTER_DEPLOYMENT.md) or [basic](./DEPLOY_SA5G_BASIC_DS_TESTER_DEPLOYMENT.md) normal deployment.

**TABLE OF CONTENTS**

1.  [Building images in debug mode](#1-building-images-in-debug-mode)
2.  [Debuggers deployment of core network functions](#2-debuggers-deployment-of-core-network-functions)
3.  [Basic debugging](#3-basic-debugging)
4.  [How to report an issue?](#4-how-to-report-an-issue)

# 1. Building images in debug mode

By default all the dockerfiles present in any network function repository (AMF, SMF, NRF, UPF) are built with `release` tag. In the `release` mode the logging information is limited. This is done to reduce the image size and have a better performance. In beginning when a user is trying to understand the functioning of core network it is recommended to build the images in `debug mode`. This way user will have more logs and can have better understanding. To build any core network image in debug mode follow the below steps **after cloning the network function repository**, the example is for AMF, 

```bash
# clone amf repository 
$ git clone -b <prefered_branch or develop> https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-amf.git
$ cd oai-cn5g-amf/docker/
# Depending on the environment where the image will be used choose the correct dockerfile
$ vi/vim/nano/subl Dockerfile.amf.ubuntu18
# replace the line RUN ./build_amf --clean --Verbose --build-type Release --jobs with 
# RUN ./build_amf --clean --Verbose --build-type Debug --jobs
```

The same can be done for baremetal deployment of any core network function, just build with `Debug` tag in `./build_amf`. There will be no change in running the core network function. Everything will be the same, now when the network function is started there will be extra logs with `[debug]` tag apart from the normal `[info ]`

## 1.1 Building the image with code inside (Only for development purpose)

If you are interested in doing development you can leave the code inside the container. This is good for developers, they can code in docker-environment. They can even mount the code as a volumne so that they can use their prefered editor (though vim/nano/vi are the best). To do this the dockerfile has to be edited and only the `BUILDER IMAGE` of the file is required the rest `TARGET IMAGE`part including CMD and ENTERYPOINT should be removed. 


# 2. Debuggers deployment of core network functions

It is really important to safely keep the logs and configuration of core network components in case of error. If they are deployed in baremetal the logs and the configuration can be easily retrived based on how the core network is started. Here are some tips related to running/deploying core network in different environment, 

# 2.1 Deploying as a process (baremetal deployment prefered by developers)

1. In case of all in one sort of process deployment there can some conflicting dependencies between different components of core network. These conflicts have to be resolved on case by case bases by the user.
2. Once all the core-network components are build in debug mode with there dependencies store the logs in a file rather than printing on the terminal. 
3. All the configuration file should be stored in one place so that it is easy to reterive. 

```bash
# storing logs and configuration in your prefered location
nohup /usr/local/bin/amf -c /tmp/oai-cn-5g/config/amf.conf -o >> /tmp/oai-cn-5g/logs/amf.log 2>&1
```

The above command will launch the network function in background and all the logs can be seen using `tail -100f /tmp/oai-cn-5g/logs/amf.log`. In case to stop the component kill the its process process using `pkill <component-name>` or `ps -eaf`

# 2.2 Docker environment (Recommeneded)

Using docker environment for deployment and development is the prefered environment because there it is easy to have dedicated working environment for each network component. It is lightweight and easy to manage. The docker-compose provided in [earlier tutorials](./DEPLOY_SA5G_DS_TESTER.md) is good for learning how the OAI core network works and how to use it. But if the user wants to change some parameters which are not variable or not allowed using docker-compose then it is hard to use docker-compose approach. If the user wants to provide their own configuration file then it is better to change the docker-compose. Follow the below steps to create a new developer/debugger specific docker-compose, 

## 2.2.1 Prerequisites

1. Build the docker-images in debug mode following the [previous section](#1-building-images-in-debug-mode)
2. Create a new folder `oai-docker-compose`
3. In the `oai-docker-compose` folder create `confs`, `logs`, `entrypoints` and `healthchecks` folder
4. In the `confs` folder copy the configuration files for each component you want to use. The configuration files are located here 


    | File Name   | Repository                                   | Location        |
    |:----------- |:-------------------------------------------- |:--------------- |
    | amf.conf    | (Gitlab) cn5g/oai-cn5g-amf                   | [etc/amf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-amf/-/blob/develop/etc/amf.conf)    |
    | smf.conf    | (Gitlab) cn5g/oai-cn5g-smf                   | [etc/smf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-smf/-/blob/develop/etc/smf.conf)    |
    | nrf.conf    | (Gilab) cn5g/oai-cn5g-nrf                    | [etc/nrf.conf](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-nrf/-/blob/develop/etc/nrf.conf)   |
    | spgw_u.conf | (Github) OPENAIRINTERFACE/openair-spgwu-tiny | [etc/spgw_u.conf](https://github.com/OPENAIRINTERFACE/openair-spgwu-tiny/blob/nrf_fqdn/etc/spgw_u.conf) |

5. Create empty log files for the component you want to use in the `logs` folder using `touch` command example
6. Copy [oai_db.sql](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed/-/blob/master/docker-compose/oai_db.sql) make user database depending on the IMSI, LTKEY and Opc. New user entry can be added after this [line](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed/-/blob/master/docker-compose/oai_db.sql#L193)

```
# Creating directory structure
$ mkdir ~/oai-docker-compose
$ mkdir -p ~/oai-docker-compose/confs
$ mkdir -p ~/oai-docker-compose/logs
$ mkdir -p ~/oai-docker-compose/entrypoints
$ mkdir -p ~/oai-docker-compose/healthchecks 

# Creating empty log files
$ touch ~/oai-docker-compose/logs/amf.log
$ touch ~/oai-docker-compose/logs/smf.log
$ touch ~/oai-docker-compose/logs/nrf.log
$ touch ~/oai-docker-compose/logs/spgwu.log

# Copying the configuration files, if you have yours you can use that else copy from the repository and make changes manually
$ cp ~/oai-cn5g-amf/etc/amf.conf ~/oai-docker-compose/confs/
$ cp ~/oai-cn5g-smf/etc/smf.conf ~/oai-docker-compose/confs/
$ cp ~/oai-cn5g-nrf/etc/nrf.conf ~/oai-docker-compose/confs/
$ cp ~/openair-spgwu-tiny/etc/spgwu.conf ~/oai-docker-compose/confs/
```

### 2.2.2 Creating entrypoint files 

The example of amf entrypoint.sh is below for other network functions it is analogus. 

```bash
#!/bin/bash
set -eumb

echo "Running amf to check logs use tail -100f ~/oai-docker-compose/logs/amf.log"
exec nohup /usr/local/bin/amf -c /openair-amf/etc/amf.conf -o >> /openair-amf/etc/amf.log 2>&1
```

in the above replace amf with nrf,smf to create entrypoints for nrf and smf for spgwu it is slightly different

```
echo "Running spgwu to check logs use tail -100f ~/oai-docker-compose/logs/spgw_u.log"
exec nohup /usr/local/bin/spgwu -c /openair-spgwu-tiny/etc/spgw_u.conf -o >> /openair-spgwu-tiny/etc/spgw_u.log 2>&1
```

Create entrypoints for all the network functions which are required. 

# 2.2.3 Healthchecks

The healthchecks can be directly used from [here](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed/-/tree/master/docker-compose), copy them in the `healthchecks` folder. 

# 2.2.4 Creating docker-compose

To run this docker-compose the network `demo-oai-public-net` should be created. To know how to create the network [follow](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed/-/blob/master/docs/DEPLOY_SA5G_WITH_DS_TESTER.md#3-configuring-host-machines)

```
version: '3.8'
services:
    oai-nrf:
        container_name: oai-nrf
        image: oai-nrf:develop
        entrypoint: ["/openair-nrf/bin/nrf-entrypoint.sh"]
        restart: always
        volumes:
            - ./healthchecks/nrf-healthcheck.sh:/tmp/nrf-healthcheck.sh
            - ./confs/nrf.conf:/openair-nrf/etc/nrf.conf
            - ./entrypoints/nrf-entrypoint.sh:/openair-nrf/bin/nrf-entrypoint.sh
            - ./logs/nrf.log:/openair-nrf/etc/nrf.log:rw
        healthcheck:
            test: /bin/bash -c "/tmp/nrf-healthcheck.sh"
            interval: 10s
            timeout: 15s
            retries: 5
        networks:
            public_net:
                ipv4_address: 192.168.70.130
    mysql:
        container_name: mysql
        image: mysql:5.7
        restart: always
        volumes:
            - ./oai_db.sql:/docker-entrypoint-initdb.d/oai_db.sql
            - ./healthchecks/mysql-healthcheck.sh:/tmp/mysql-healthcheck.sh
        depends_on:
            - oai-nrf
        environment:
            - TZ=Europe/Paris
            - MYSQL_DATABASE=oai_db
            - MYSQL_USER=test
            - MYSQL_PASSWORD=test
            - MYSQL_ROOT_PASSWORD=linux
        healthcheck:
            test: /bin/bash -c "/tmp/mysql-healthcheck.sh"
            interval: 10s
            timeout: 5s
            retries: 5
        networks:
            public_net:
                ipv4_address: 192.168.70.131
    oai-amf:
        container_name: oai-amf
        image: oai-amf:develop
        entrypoint: ["/openair-amf/bin/amf-entrypoint.sh"]
        restart: always
        depends_on:
            - mysql
        volumes:
            - ./healthchecks/amf-healthcheck.sh:/tmp/amf-healthcheck.sh
            - ./confs/amf.conf:/openair-amf/etc/amf.conf
            - ./entrypoints/amf-entrypoint.sh:/openair-amf/bin/amf-entrypoint.sh
            - ./logs/amf.log:/openair-amf/etc/amf.log:rw
        healthcheck:
            test: /bin/bash -c "/tmp/amf-healthcheck.sh"
            interval: 10s
            timeout: 15s
            retries: 5
        networks:
            public_net:
                ipv4_address: 192.168.70.132
    oai-smf:
        container_name: oai-smf
        entrypoint: ["/openair-smf/bin/smf-entrypoint.sh"]
        restart: always
        image: oai-smf:develop
        depends_on:
            - oai-amf
        volumes:
            - ./healthchecks/smf-healthcheck.sh:/tmp/smf-healthcheck.sh
            - ./entrypoints/smf-entrypoint.sh:/openair-smf/bin/smf-entrypoint.sh
            - ./confs/smf.conf:/openair-smf/etc/smf.conf
            - ./logs/smf.log:/openair-smf/etc/smf.log:rw
        healthcheck:
            test: /bin/bash -c "/tmp/smf-healthcheck.sh"
            interval: 10s
            timeout: 5s
            retries: 5
        networks:
            public_net:
                ipv4_address: 192.168.70.133
    oai-spgwu:
        container_name: "oai-spgwu"
        image: oai-spgwu:develop
        entrypoint: ["/openair-spgwu-tiny/bin/spgwu-entrypoint.sh"]
        restart: always
        depends_on:
            - oai-smf
        cap_add:
            - NET_ADMIN
            - SYS_ADMIN
        cap_drop:
            - ALL
        privileged: true
        volumes:
            - ./healthchecks/spgwu-healthcheck.sh:/tmp/spgwu-healthcheck.sh
            - ./entrypoints/spgwu-entrypoint.sh:/openair-spgwu-tiny/bin/spgwu-entrypoint.sh
            - ./confs/spgw_u.conf:/openair-spgwu-tiny/etc/spgw_u.conf
            - ./logs/spgw_u.log:/openair-spgwu-tiny/etc/spgw_u.log:rw
        healthcheck:
            test: /bin/bash -c "/tmp/spgwu-healthcheck.sh"
            interval: 10s
            timeout: 5s
            retries: 5
        networks:
            public_net:
                ipv4_address: 192.168.70.134
networks:
    public_net:
        external:
           name: demo-oai-public-net

```


## 2.2.5 Playing with docker-compose

```
# start docker-compose
docker-compose -p <project-name> -f <file-name> up -d
# if made changes in the conf files located in ./confs/ restart the container/service
docker-compose -p <project-name> -f <file-name> restart <service-name>
# force recreate a service 
docker-compose -p <project-name> -f <file-name> up -d <service-name> --force-create
# incase the code is present in side the container and some changes are made then just restart the container never remove
docker-compose -p <project-name> -f <file-name> restart <service-name>
# stop the containers/service
docker-compose -p <project-name> -f <file-name> stop <service-name>
# remove the deployment
docker-compose -p <project-name> -f <file-name> down
```

Network components configuration is present in `~/oai-docker-compose/confs/` the logs are present in `~/oai-docker-compose/logs/`. There will be only one log file and it will container huge amount of logs. If needed this can also be rotated to avoid having one bulky file. To make it rotating make changes in the entrypoint.sh script

# 3. Basic debugging

1. Building the images in debug mode will provide more information about UE attach-detach process. 
2. Capturing packets to understand message flow between the components and encapsulation-de-capsulation 
3. The captured packets can be compared with the end-to-end pcaps which are available in this [tutorial](./DEPLOY_SA5G_DS_TESTER.md)
4. Check the UE subscription information is available in the Mysql database and the OPC is correctly configured in AMF. 


# 4. How to report an issue?

To report an issue regarding any-component of CN5G or attach-detach procedure follow the below procedure,

1. Share the testing scenario, what the test is trying to achieve
2. Debug logs of the 5GCN components and packet capture/tcpdump of the 5GCN components. Depending on where the packets are captured take care of interface on which the packets are captured. Also it will be nice to capture packets using a filter `ngap || http || pfcp || gtp`
3. If you have an issue with testing then you can send an email at openair5g-cn@lists.eurecom.fr with the configuration files, log files in debug mode and pcaps with appropriate filters. 
4. In case you want to report a bug in the code of any network function then you can do it directly via gitlab.
