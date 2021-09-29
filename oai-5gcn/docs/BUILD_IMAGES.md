<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Docker Deployment : Building Container Images</font></b>
    </td>
  </tr>
</table>

# 1.  Retrieve the correct network function branches #

| CNF Name    | Branch Name             | Commit at time of writing                  | Ubuntu 18.04 | RHEL8 (UBI8)    |
| ----------- |:----------------------- | ------------------------------------------ | ------------ | ----------------|
| AMF         | `develop`               | `1a9f65c6a1e1846b13b82ad337a965596565fdfe` | X            | X               |
| SMF         | `develop`               | `11d6375c4ac408805f294172cc789cd196a75dc6` | X            | X               |
| NRF         | `develop`               | `a221f39c9d9729d0652042aee918c81b23d95de6` | X            | X               |
| SPGW-U-TINY | `gtp_extension_header`  | `3898c773f91bb21451d8a9d4ef8e3d06ab184e1d` | X            | -               |

```bash
$ git clone https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed.git
$ cd oai-cn5g-fed
$ git checkout master
$ git pull origin master
$ ./scripts/syncComponents.sh -h
Openair-CN components synchronization
   Original Author: Raphael Defosseux

   Requirement: git shall be installed

   By default (no options) all components will be synchronized to
     the 'develop' branch.
   Each component can be synchronized a dedicated branch.

Usage:
------
    syncComponents.sh [OPTIONS]

Options:
--------
    --nrf-branch ####
    Specify the source branch for the OAI-NRF component

    --amf-branch ####
    Specify the source branch for the OAI-AMF component

    --smf-branch ####
    Specify the source branch for the OAI-SMF component

    --spgwu-tiny-branch ####
    Specify the source branch for the OAI-SPGW-U-TINY component

    --help OR -h
    Print this help message.

$ ./scripts/syncComponents.sh --spgwu-tiny-branch gtp_extension_header
---------------------------------------------------------
OAI-AMF    component branch : develop
OAI-SMF    component branch : develop
OAI-NRF    component branch : develop
OAI-SPGW-U component branch : gtp_extension_header
---------------------------------------------------------
....
```

# 2. Generic Parameters #

Here in our network configuration, we need to pass the "GIT PROXY" configuration.

*   If you do not need, remove the `--build-arg NEEDED_GIT_PROXY=".."` option.
*   If you do need it, change with your proxy value.

# 3. Build AMF Image #

## 3.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-amf --tag oai-amf:develop \
               --file component/oai-amf/docker/Dockerfile.ubuntu.18.04 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-amf
$ docker image prune --force
$ docker image ls
oai-amf                 develop             f478bafd7a06        1 minute ago          258MB
...
```

## 3.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository. For that the base image needs ca and entitlement .pem files. Copy the ca and entitlement .pem files in the oai-amf repository in a new folder name tmp before building the image. 

```bash
$ sudo podman build --target oai-amf --tag oai-amf:develop \
               --file component/oai-amf/docker/Dockerfile.amf.rhel8.2 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-amf
...
```

The above command is with podman, incase of docker it can be changed with its docker equivalent. 


# 4. Build SMF Image #

## 4.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-smf --tag oai-smf:develop \
               --file component/oai-smf/docker/Dockerfile.ubuntu.18.04 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-smf
$ docker image prune --force
$ docker image ls
oai-smf                 develop             f478bafd7a06        1 minute ago          274MB
...
```

## 4.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository. For that the base image needs ca and entitlement .pem files. Copy the ca and entitlement .pem files in the oai-smf repository in a new folder name tmp before building the image. 

```bash
$ sudo podman build --target oai-smf --tag oai-smf:develop \
               --file component/oai-smf/docker/Dockerfile.smf.rhel8.2 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-smf
...
```

The above command is with podman, incase of docker it can be changed with its docker equivalent. 



# 5. Build NRF Image #

## 5.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-nrf --tag oai-nrf:develop \
               --file component/oai-nrf/docker/Dockerfile.ubuntu.18.04 \
               --build-arg EURECOM_PROXY="http://proxy.eurecom.fr:8080" component/oai-nrf
$ docker image prune --force
$ docker image ls
oai-nrf                 develop             04334b29e103        1 minute ago          280MB
...
```

## 5.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository. For that the base image needs ca and entitlement .pem files. Copy the ca and entitlement .pem files in the oai-nrf repository in a new folder name tmp before building the image. 

```bash
$ sudo podman build --target oai-nrf --tag oai-nrf:develop \
               --file component/oai-nrf/docker/Dockerfile.nrf.rhel8.2 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-nrf
...
```

The above command is with podman, incase of docker it can be changed with its docker equivalent. 



# 6. Build SPGW-U Image #

## 6.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-spgwu-tiny --tag oai-spgwu-tiny:gtp-ext-header \
               --file component/oai-upf-equivalent/docker/Dockerfile.ubuntu18.04 \
               --build-arg EURECOM_PROXY="http://proxy.eurecom.fr:8080" component/oai-upf-equivalent
$ docker image prune --force
$ docker image ls
oai-spgwu-tiny          gtp-ext-header             dec6311cef3b        1 minute ago          255MB
...
```

## 6.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository. For that the base image needs ca and entitlement .pem files. Copy the ca and entitlement .pem files in the oai-spgwu repository in a new folder name tmp before building the image. 

```bash
$ sudo podman build --target oai-spgwu-tiny --tag oai-spgwu-tiny:develop \
               --file component/oai-spgwu-tiny/docker/Dockerfile.centos8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-upf-equivalent
...
```

The above command is with podman, incase of docker it can be changed with its docker equivalent. 

You are ready to [Configure the Containers](./CONFIGURE_CONTAINERS.md) or deploying the images using [helm-charts] (./DEPLOY_SA5G_HC.md)
