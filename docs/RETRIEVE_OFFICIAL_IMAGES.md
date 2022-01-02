<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Deployment : Pulling Container Images</font></b>
    </td>
  </tr>
</table>

This page is only valid for an `Ubuntu18` host.

If you are using any other distributions, please refer to [Build your own images](./BUILD_IMAGES.md).

If you want to the up-to-date new features, please refer to [Build your own images](./BUILD_IMAGES.md).

# Pulling the images from Docker Hub #

Currently the images are hosted under the user account `rdefosseoai`.

This may change in the future.

Once again you may need to log on [docker-hub](https://hub.docker.com/) if your organization has reached pulling limit as `anonymous`.

```bash
$ docker login
Login with your Docker ID to push and pull images from Docker Hub. If you don't have a Docker ID, head over to https://hub.docker.com to create one.
Username:
Password:
```

The OAI CI/CD team has automated more frequent pushes to Docker-Hub on `rdefosseoai` account. Two important things to be noted:
  - We will keep pushing to the `latest` tag when a milestone is reached.
  - We are making pushes on the `develop` tag whenever a contribution has been accepted. These images are **EXPERIMENTAL**.

Now pull images.

```bash
$ docker pull rdefosseoai/oai-amf:latest
$ docker pull rdefosseoai/oai-nrf:latest
$ docker pull rdefosseoai/oai-spgwu-tiny:latest
$ docker pull rdefosseoai/oai-smf:latest
$ docker pull rdefosseoai/oai-udr:latest
$ docker pull rdefosseoai/oai-udm:latest
$ docker pull rdefosseoai/oai-ausf:latest
$ docker pull rdefosseoai/oai-upf-vpp:latest
```

And **re-tag** them for tutorials' docker-compose file to work.

```bash
$ docker image tag rdefosseoai/oai-amf:latest oai-amf:latest
$ docker image tag rdefosseoai/oai-nrf:latest oai-nrf:latest
$ docker image tag rdefosseoai/oai-smf:latest oai-smf:latest
$ docker image tag rdefosseoai/oai-spgwu-tiny:latest oai-spgwu-tiny:latest
$ docker image tag rdefosseoai/oai-udr:latest oai-udr:latest
$ docker image tag rdefosseoai/oai-udm:latest oai-udm:latest
$ docker image tag rdefosseoai/oai-ausf:latest oai-ausf:latest
$ docker image tag rdefosseoai/oai-upf-vpp:latest oai-upf-vpp:latest
```

Finally you may logoff --> your token is stored in plain text..

```bash
$ docker logout
```

We will push new versions when new features are validated.

# Synchronizing the tutorials #

**CAUTION: PLEASE READ THIS SECTION VERY CAREFULLY!**

This repository only has tutorials and Continuous Integration scripts.

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

Later versions of the `master` branch may not work with the pulled images.

You are ready to [Configure the Containers](./CONFIGURE_CONTAINERS.md).
