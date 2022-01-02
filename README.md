<h1 align="center">
    <a href="https://openairinterface.org/"><img src="https://openairinterface.org/wp-content/uploads/2015/06/cropped-oai_final_logo.png" alt="OAI" width="550"></a>
</h1>

<p align="center">
    <a href="https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed/-/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-OAI--Public--V1.1-blue" alt="License"></a>
    <a href="https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-upf-vpp/-/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-Apache--2.0-blue" alt="License"></a>
    <a href="https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed/-/releases"><img alt="GitLab Release (custom instance)" src="https://img.shields.io/gitlab/v/release/oai/cn5g/oai-cn5g-fed?gitlab_url=https%3A%2F%2Fgitlab.eurecom.fr&include_prereleases&sort=semver"></a>
    <a href="https://releases.ubuntu.com/18.04/"><img src="https://img.shields.io/badge/OS-Ubuntu18-Green" alt="Supported OS"></a>
    <a href="https://www.redhat.com/en/enterprise-linux-8"><img src="https://img.shields.io/badge/OS-RHEL8-Green" alt="Supported OS"></a>
</p>

<p align="center">
    <a href="https://jenkins-oai.eurecom.fr/job/OAI-CN5G-AMF/"><img src="https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins-oai.eurecom.fr%2Fjob%2FOAI-CN5G-AMF%2F&label=build%20AMF"></a>
    <a href="https://jenkins-oai.eurecom.fr/job/OAI-CN5G-NRF/"><img src="https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins-oai.eurecom.fr%2Fjob%2FOAI-CN5G-NRF%2F&label=build%20NRF"></a>
    <a href="https://jenkins-oai.eurecom.fr/job/OAI-CN5G-SMF/"><img src="https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins-oai.eurecom.fr%2Fjob%2FOAI-CN5G-SMF%2F&label=build%20SMF"></a>
    <a href="https://jenkins-oai.eurecom.fr/job/OAI-CN5G-AUSF/"><img src="https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins-oai.eurecom.fr%2Fjob%2FOAI-CN5G-AUSF%2F&label=build%20AUSF"></a>
    <a href="https://jenkins-oai.eurecom.fr/job/OAI-CN5G-UDM/"><img src="https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins-oai.eurecom.fr%2Fjob%2FOAI-CN5G-UDM%2F&label=build%20UDM"></a>
    <a href="https://jenkins-oai.eurecom.fr/job/OAI-CN5G-UDR/"><img src="https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins-oai.eurecom.fr%2Fjob%2FOAI-CN5G-UDR%2F&label=build%20UDR"></a>
    <a href="https://jenkins-oai.eurecom.fr/job/OAI-CN5G-UPF-VPP/"><img src="https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins-oai.eurecom.fr%2Fjob%2FOAI-CN5G-UPF-VPP%2F&label=build%20UPF-VPP"></a>
</p>

<p align="center">
  <a href="https://hub.docker.com/r/rdefosseoai/oai-amf"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-amf?label=AMF%20docker%20pulls"></a>
  <a href="https://hub.docker.com/r/rdefosseoai/oai-ausf"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-ausf?label=AUSF%20docker%20pulls"></a>
  <a href="https://hub.docker.com/r/rdefosseoai/oai-nrf"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-nrf?label=NRF%20docker%20pulls"></a>
  <a href="https://hub.docker.com/r/rdefosseoai/oai-smf"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-smf?label=SMF%20docker%20pulls"></a>
  <a href="https://hub.docker.com/r/rdefosseoai/oai-udm"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-udm?label=UDM%20docker%20pulls"></a>
  <a href="https://hub.docker.com/r/rdefosseoai/oai-udr"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-udr?label=UDR%20docker%20pulls"></a>
  <a href="https://hub.docker.com/r/rdefosseoai/oai-upf-vpp"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-upf-vpp?label=UPF-VPP%20docker%20pulls"></a>
  <a href="https://hub.docker.com/r/rdefosseoai/oai-spgwu-tiny"><img alt="Docker Pulls" src="https://img.shields.io/docker/pulls/rdefosseoai/oai-spgwu-tiny?label=SPGWU-TINY%20docker%20pulls"></a>
</p>

<h2 align="center">
 OPENAIR-CN-5G: An implementation of the 5G Core network by the OpenAirInterface community.
</h2>

`OPENAIR-CN-5G` is an implementation of the 3GPP specifications for the 5G Core Network.
At the moment, it contains the following network elements:

* Access and Mobility Management Function (**[AMF](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-amf)**)
* Authentication Server Management Function (**[AUSF](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-ausf)**)
* Network Repository Function (**[NRF](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-nrf)**)
* Session Management Function (**[SMF](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-smf)**)
* Unified Data Management (**[UDM](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-udm)**)
* Unified Data Repository (**[UDR](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-udr)**)
* User Plane Function (**UPF**) with 2 variants:
  * Simple Implementation based on our 4G CUPS component (**[SPGWU-TINY](https://github.com/OPENAIRINTERFACE/openair-spgwu-tiny)**)
  * VPP-Based Implementation (**[UPF-VPP](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-upf-vpp)**)
* Network Slicing Selection Function (**NSSF**)
* Network Data Analytics Function (**NWDAF**)
* Unstructured Data Storage Function (**UDSF**)

Each has its own repository. Some of these repositories are still private, soon to be released.

This repository is a **Federation of the OpenAir CN 5G repositories**.

Its main purpose is for Continuous Integration scripting.

It also hosts some tutorials.

* [How to do a container-based simple deployment](docs/DEPLOY_HOME.md).
* [How to create a container-based developer environment](docs/DEBUG_5G_CORE.md).

# Licence info

It is distributed under `OAI Public License V1.1`.
See [OAI Website for more details](https://www.openairinterface.org/?page_id=698).

The text for `OAI Public License V1.1` is also available under [LICENSE](LICENSE)
file at the root of this repository.

Note that the `UPF-VPP` implementation is distributed under `Apache V2.0 License`.

See [Apache Website for more details](http://www.apache.org/licenses/LICENSE-2.0).

# Collaborative work

This source code is managed through a GITLAB server, a collaborative development platform.

Process is explained in [CONTRIBUTING](CONTRIBUTING.md) file.

# Contribution requests

In a general way, anybody who is willing can contribute on any part of the
code in any network component.

Contributions can be simple bugfixes, advices and remarks on the design,
architecture, coding/implementation.

# Release Notes

They are available on the [CHANGELOG](CHANGELOG.md) file.

