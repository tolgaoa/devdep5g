FROM ubuntu:bionic
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Paris
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get upgrade --yes && DEBIAN_FRONTEND=noninteractive apt-get install --yes \
    tzdata \
    build-essential \
    psmisc \
    net-tools \
    bc \
    tshark \
    iputils-ping \
    iptables \
    iperf3 \
    iproute2 \
    libconfig++9v5 \
    libsctp1 \
  && rm -rf /var/lib/apt/lists/*

# expose ports
EXPOSE 80/tcp 9090/tcp 38412/sctp

CMD ["sleep", "infinity"]
