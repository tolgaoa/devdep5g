# Default values for oai-udm.
# This is a YAML-formatted file.
# Declare variables to be passed into your templates.

replicaCount: 1

namespace: "oai"

nfimage:
  registry: local
  repository: tolgaomeratalay/udm
  version: bench #f1_f2345_gautn_kausfv3 #f1_f2345_gautn_annexa4_kausfv1  #modfall_v2 #fall_gautn_v1 #modfall_annexv7 #modfall_anv1 #testintv06
  #pullPolicy: IfNotPresent or Never or Always
  pullPolicy: IfNotPresent

tcpdumpimage:
  registry: local
  repository: corfr/tcpdump
  version: latest
  #pullPolicy: IfNotPresent or Never or Always
  pullPolicy: IfNotPresent

## good to use when pulling images from docker-hub mention 
#imagePullSecrets: 
#  - name: "personalkey"

serviceAccount:
  # Specifies whether a service account should be created
  create: true
  # Annotations to add to the service account
  annotations: {}
  # The name of the service account to use.
  # If not set and create is true, a name is generated using the fullname template
  name: "oai-udm-sa10"

podSecurityContext:
  runAsUser: 0
  runAsGroup: 0

securityContext:
  privileged: false

service:
  type: ClusterIP
  httpPort: 80

start:
  udm: true
  tcpdump: false

multus:
  nudmIPmain: "10.100.13.10"
  nudmIPGw: "10.100.10.1"
  nudmIPnetmask: "16"

config:
  tz: "Europe/Paris"
  instance: "0"
  pidDirectory: "/var/run"
  udmName: "OAI_UDM"
  sbiIfName: "net1"
  sbiPort: "80"
  udmVersionNb: "v1"
  useFqdnDns: "no"
  udrIpAddress: "10.100.12.10"
  udrPort: "80"
  udrVersionNb: "v1"
  udrFqdn: "oai-udr-svc"
  f1ip: "10.100.30.11"
  f2345ip: "10.100.30.10"
  gautnip: "10.100.30.12"
  kausfip: "10.100.30.15"
  annexip: "10.100.30.13"

persistence:
  sharedvolume: false
  volumneName: managed-nfs-storage
  size: 1Gi

resources:
  define: false
  limits:
    cpu: 100m
    memory: 128Mi
  requests:
   cpu: 100m
   memory: 256Mi

readinessProbe: false

livenessProbe: false

terminationGracePeriodSeconds: 30

nodeSelector: {}

nodeName: 