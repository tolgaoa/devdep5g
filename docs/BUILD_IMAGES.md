<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Deployment : Building Container Images</font></b>
    </td>
  </tr>
</table>

# 1.  Retrieve the correct network function branches #

**CAUTION: PLEASE READ THIS SECTION VERY CAREFULLY!**

This repository only has tutorials and Continuous Integration scripts.

Each 5G Network Function source code is managed in its own repository.

They are called as `git sub-modules` in the `component` folder.

Before doing anything, you SHALL retrieve the code for each git sub-module.

## 1.1. You are interested on a very stable version. ##

We recommend to synchronize with the master branches on all git sub-modules.

We also recommend that you synchronize this "tutorial" repository with a provided tag. By doing so, the `docker-compose` files will be aligned with feature sets of each cNF.

**At the time of writing (2021/10/29), the release tag is `v1.2.1`.**

| CNF Name    | Branch Name | Tag      | Ubuntu 18.04 | RHEL8 (UBI8)    |
| ----------- | ----------- | -------- | ------------ | ----------------|
| FED REPO    | N/A         | `v1.2.1` |              |                 |
| AMF         | `master`    | `v1.2.1` | X            | X               |
| SMF         | `master`    | `v1.2.1` | X            | X               |
| NRF         | `master`    | `v1.2.1` | X            | X               |
| SPGW-U-TINY | `master`    | `v1.1.4` | X            | X               |
| UDR         | `master`    | `v1.2.1` | X            | X               |
| UDM         | `master`    | `v1.2.1` | X            | X               |
| AUSF        | `master`    | `v1.2.1` | X            | X               |
| UPF-VPP     | `master`    | `v1.2.1` | X            | X               |

```bash
# Clone directly on the latest release tag
$ git clone --branch v1.2.1 https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed.git
$ cd oai-cn5g-fed
# If you forgot to clone directly to the latest release tag
$ git checkout -f v1.2.1

# Synchronize all git submodules
$ ./scripts/syncComponents.sh 
---------------------------------------------------------
OAI-NRF     component branch : master
OAI-AMF     component branch : master
OAI-SMF     component branch : master
OAI-SPGW-U  component branch : master
OAI-AUSF    component branch : master
OAI-UDM     component branch : master
OAI-UDR     component branch : master
OAI-UPF-VPP component branch : master
---------------------------------------------------------
git submodule deinit --all --force
git submodule init
git submodule update
```

or a little bit more dangerous

```bash
$ ./scripts/syncComponents.sh --nrf-branch v1.2.1 --amf-branch v1.2.1 \
                              --smf-branch v1.2.1 --spgwu-tiny-branch v1.1.4 \
                              --udr-branch v1.2.1 --udm-branch v1.2.1 \
                              --ausf-branch v1.2.1 --upf-vpp-branch v1.2.1
---------------------------------------------------------
OAI-NRF     component branch : v1.2.1
OAI-AMF     component branch : v1.2.1
OAI-SMF     component branch : v1.2.1
OAI-SPGW-U  component branch : v1.1.4
OAI-UDR     component branch : v1.2.1
OAI-UDM     component branch : v1.2.1
OAI-AUSF    component branch : v1.2.1
OAI-UPF-VPP component branch : v1.2.1
---------------------------------------------------------
git submodule deinit --all --force
git submodule init
git submodule update
```

## 1.2. You are interested on the latest features. ##

All the latest features are somehow pushed to the `develop` branches of each NF repository.

It means that we/you are able to build and the Continuous Integration test suite makes sure it
does NOT break any existing tested feature.

So for example, at time of writing, N2 Handover support code is included in `v1.1.0` release.
But it is not tested yet. So it is not advertized in the `CHANGELOG.md` and the Release Notes.

Anyhow, the tutorials' docker-compose files on the latest commit of the `master` branch of
`oai-cn5g-fed` repository SHALL support any additional un-tested feature.

```bash
# Clone
$ git clone  https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed.git
$ cd oai-cn5g-fed
# On an existing repository, resync to the last `master` commit
$ git fetch --prune
$ git checkout master
$ git rebase origin/master

# Synchronize all git submodules
$ ./scripts/syncComponents.sh --nrf-branch develop --amf-branch develop \
                              --smf-branch develop --spgwu-tiny-branch develop \
                              --ausf-branch develop --udm-branch develop \
                              --udr-branch develop --upf-vpp-branch develop
---------------------------------------------------------
OAI-NRF     component branch : develop
OAI-AMF     component branch : develop
OAI-SMF     component branch : develop
OAI-SPGW-U  component branch : develop
OAI-AUSF    component branch : develop
OAI-UDM     component branch : develop
OAI-UDR     component branch : develop
OAI-UPF-VPP component branch : develop
---------------------------------------------------------
git submodule deinit --all --force
git submodule init
git submodule update
```

# 2. Generic Parameters #

Here in our network configuration, we need to pass the "GIT PROXY" configuration.

*   If you do not need, remove the `--build-arg NEEDED_GIT_PROXY=".."` option.
*   If you do need it, change with your proxy value.

If you have re-building CN5G images, be careful that `docker` or `podman` may re-use `cached` blobs
to construct the intermediate layers.

We recommend to add the `--no-cache` option in that case.

# 3. Build AMF Image #

## 3.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-amf --tag oai-amf:latest \
               --file component/oai-amf/docker/Dockerfile.amf.ubuntu18 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-amf
$ docker image prune --force
$ docker image ls
oai-amf                 latest             f478bafd7a06        1 minute ago          279MB
...
```

## 3.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-amf repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-amf --tag oai-amf:latest \
               --file component/oai-amf/docker/Dockerfile.amf.rhel8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-amf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.


# 4. Build SMF Image #

## 4.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-smf --tag oai-smf:latest \
               --file component/oai-smf/docker/Dockerfile.smf.ubuntu18 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-smf
$ docker image prune --force
$ docker image ls
oai-smf                 latest             f478bafd7a06        1 minute ago          293MB
...
```

## 4.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-smf repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-smf --tag oai-smf:latest \
               --file component/oai-smf/docker/Dockerfile.smf.rhel8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-smf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 5. Build NRF Image #

## 5.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-nrf --tag oai-nrf:latest \
               --file component/oai-nrf/docker/Dockerfile.nrf.ubuntu18 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-nrf
$ docker image prune --force
$ docker image ls
oai-nrf                 latest             04334b29e103        1 minute ago          247MB
...
```

## 5.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-nrf repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-nrf --tag oai-nrf:latest \
               --file component/oai-nrf/docker/Dockerfile.nrf.rhel8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-nrf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 6. Build SPGW-U Image #

## 6.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-spgwu-tiny --tag oai-spgwu-tiny:latest \
               --file component/oai-upf-equivalent/docker/Dockerfile.ubuntu18.04 \
               --build-arg EURECOM_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-upf-equivalent
$ docker image prune --force
$ docker image ls
oai-spgwu-tiny          latest             dec6311cef3b        1 minute ago          255MB
...
```

## 6.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-spgwu repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-spgwu-tiny --tag oai-spgwu-tiny:latest \
               --file component/oai-spgwu-tiny/docker/Dockerfile.centos8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-upf-equivalent
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 7. Build AUSF Image #

## 7.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-ausf --tag oai-ausf:latest \
               --file component/oai-ausf/docker/Dockerfile.ausf.ubuntu18 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-ausf
$ docker image prune --force
$ docker image ls
oai-ausf          latest              77a96de94c23        1 minute ago        231MB
...
```

## 7.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-nrf repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-ausf --tag oai-ausf:latest \
               --file component/oai-ausf/docker/Dockerfile.ausf.rhel8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-ausf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 8. Build UDM Image #

## 8.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-udm --tag oai-udm:latest \
               --file component/oai-udm/docker/Dockerfile.udm.ubuntu18 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-udm
$ docker image prune --force
$ docker image ls
oai-udm                 latest             10a4334e31be        1 minute ago          229MB
...
```

## 8.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-nrf repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-udm --tag oai-udm:latest \
               --file component/oai-udm/docker/Dockerfile.udm.rhel8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-udm
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 9. Build UDR Image #

## 9.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-udr --tag oai-udr:latest \
               --file component/oai-udr/docker/Dockerfile.udr.ubuntu18 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-udr
$ docker image prune --force
$ docker image ls
oai-udr                 latest             581e07d59ec3        1 minute ago          234MB
...
```

## 9.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-nrf repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-udr --tag oai-udr:latest \
               --file component/oai-udr/docker/Dockerfile.udr.rhel8 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-udr
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.


# 10. Build UPF-VPP Image #

## 9.1 On a Ubuntu 18.04 Host ##

```bash
$ docker build --target oai-upf-vpp --tag oai-upf-vpp:latest \
               --file component/oai-upf-vpp/docker/Dockerfile.upf-vpp.ubuntu18 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-upf-vpp
$ docker image prune --force
$ docker image ls
oai-upf-vpp             latest             581e07d59ec3        1 minute ago          937MB
...
```

## 9.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in the oai-nrf repository in a new folder named `tmp` before building the image.

```bash
$ sudo podman build --target oai-upf-vpp --tag oai-upf-vpp:latest \
               --file component/oai-upf-vpp/docker/Dockerfile.upf-vpp.rhel7 \
               --build-arg NEEDED_GIT_PROXY="http://proxy.eurecom.fr:8080" \
               component/oai-upf-vpp
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.


You are ready to [Configure the Containers](./CONFIGURE_CONTAINERS.md) or deploying the images using [helm-charts](./DEPLOY_SA5G_HC.md)
