<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Docker Deployment : Pre-Requisites </font></b>
    </td>
  </tr>
</table>

# 1. Install the proper version of Docker #


```bash
$ dpkg --list | grep docker
ii  docker-ce                             5:19.03.6~3-0~ubuntu-bionic                     amd64        Docker: the open-source application container engine
ii  docker-ce-cli                         5:19.03.6~3-0~ubuntu-bionic                     amd64        Docker CLI: the open-source application container engine
```

Also python3 (at least 3.6) shall be installed.

```bash
$ python3 --version
Python 3.6.9
```

**CAUTION: do not forget to add your username to the `docker` group**

Otherwise you will have to run in `sudo` mode.

```bash
$ sudo usermod -a -G docker myusername
```

On Centos 7.7 host:

```bash
$ sudo yum install -y yum-utils
$ sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
$ sudo yum install docker-ce docker-ce-cli containerd.io
$ sudo systemctl start docker
$ docker info
```

# 2. Create an account on Docker Hub #

Go to [docker-hub](https://hub.docker.com/) and create an account.

# 3. Pull base images #

* Ubuntu  version: We need 2 base images: `ubuntu:bionic` and `mysql/mysql:5.7`

Currently we are working to support `CentOS8` and `RHEL8` distributions.

First log with your Docker Hub credentials. This is required if your organization has reached pulling limit as `anonymous`.

```bash
$ docker login
Login with your Docker ID to push and pull images from Docker Hub. If you don't have a Docker ID, head over to https://hub.docker.com to create one.
Username:
Password:
```

On a Ubuntu18.04 host:

```bash
$ docker pull ubuntu:bionic
$ docker pull mysql:5.7
```

Finally you may logoff --> your token is stored in plain text..

```bash
$ docker logout
```

# 4. Network Configuration #

**CAUTION: THIS FIRST STEP IS MANDATORY.**

Based on this [recommendation](https://docs.docker.com/network/bridge/#enable-forwarding-from-docker-containers-to-the-outside-world):

```bash
$ sudo sysctl net.ipv4.conf.all.forwarding=1
$ sudo iptables -P FORWARD ACCEPT
```

**CAUTION: THIS SECOND STEP MAY NOT BE NEEDED IN YOUR ENVIRONMENT.**

* The default docker network (ie "bridge") is on "172.17.0.0/16" range.
* In our Eurecom private network, this IP address range is already in use.
  - We have to change it to another IP range is free in our private network configuration.
  - We picked a **new/IDLE** IP range by adding a `/etc/docker/daemon.json` file:
* If you have to do the same change:
  - Select "192.168.17.1/24" range if it is free in your environment.
  - Select another IP range if not.

```json
{
    "bip": "192.168.17.1/24"
}
```

Restart the docker daemon:

```bash
$ sudo service docker restart
$ docker info
```

Check the new network configuration:

```bash
$ docker network inspect bridge
[
    {
        "Name": "bridge",
....
        "Scope": "local",
        "Driver": "bridge",
        "EnableIPv6": false,
        "IPAM": {
            "Driver": "default",
            "Options": null,
            "Config": [
                {
                    "Subnet": "192.168.17.1/24",
                    "Gateway": "192.168.17.1"
                }
            ]
        },
....
```

Here you have 2 choices:

*  You can pull official images from Docker Hub like [this](./RETRIEVE_OFFICIAL_IMAGES.md).
*  Or you are ready to [build your-self the network function images](./BUILD_IMAGES.md).
