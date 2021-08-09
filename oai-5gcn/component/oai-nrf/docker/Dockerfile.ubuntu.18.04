#---------------------------------------------------------------------
#
# Dockerfile for the Open-Air-Interface NRF service
#   Valid for Ubuntu-18.04 (bionic)
# The port exposed by container are 8080/tcp , 9090/tcp change it according to your setup
#
#---------------------------------------------------------------------

#---------------------------------------------------------------------
# BUILDER IMAGE
#---------------------------------------------------------------------
FROM ubuntu:bionic as oai-nrf-builder

ARG NEEDED_GIT_PROXY

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get upgrade --yes && DEBIAN_FRONTEND=noninteractive apt-get install --yes \
    psmisc \
    git \
  && rm -rf /var/lib/apt/lists/*

# Some GIT configuration command quite useful
RUN /bin/bash -c "if [[ -v NEEDED_GIT_PROXY ]]; then git config --global http.proxy $NEEDED_GIT_PROXY; fi"
RUN git config --global https.postBuffer 123289600
RUN git config --global http.sslverify false

# Copying source code
COPY . /openair-nrf
WORKDIR /openair-nrf

# Installing and Building NRF
WORKDIR /openair-nrf/build/scripts
RUN ./build_nrf --install-deps --force
RUN ./build_nrf --clean --Verbose --build-type Release --jobs

#---------------------------------------------------------------------
# TARGET IMAGE
#---------------------------------------------------------------------
FROM ubuntu:bionic as oai-nrf
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Paris

# We install some debug tools for the moment in addition of mandatory libraries
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get upgrade --yes && DEBIAN_FRONTEND=noninteractive apt-get install --yes \
    psmisc \
    net-tools \
    iputils-ping \
    bc \
    tzdata \
    tshark \
    libasan4 \
    libgoogle-glog0v5 \
    libdouble-conversion1 \
    libconfig++9v5 \
    libcurl4-gnutls-dev \
  && rm -rf /var/lib/apt/lists/*

# Copying executable and generated libraries
WORKDIR /openair-nrf/bin
COPY --from=oai-nrf-builder /openair-nrf/build/nrf/build/nrf oai_nrf
COPY --from=oai-nrf-builder /openair-nrf/scripts/entrypoint.sh entrypoint.sh
COPY --from=oai-nrf-builder /usr/local/lib/libpistache.so /usr/local/lib/
COPY --from=oai-nrf-builder /usr/local/lib/libnghttp2_asio.so.1 /usr/local/lib/
COPY --from=oai-nrf-builder /usr/lib/x86_64-linux-gnu/libboost_system.so.1.65.1 /usr/lib/x86_64-linux-gnu/
COPY --from=oai-nrf-builder /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.65.1 /usr/lib/x86_64-linux-gnu/
RUN ldconfig

# Copying template configuration files
COPY --from=oai-nrf-builder /openair-nrf/etc/nrf.conf /openair-nrf/etc/

WORKDIR /openair-nrf

# expose ports
EXPOSE 80/tcp 9090/tcp

ENTRYPOINT ["/bin/bash","/openair-nrf/bin/entrypoint.sh"]

CMD ["/openair-nrf/bin/oai_nrf", "-c", "/openair-nrf/etc/nrf.conf", "-o"]
