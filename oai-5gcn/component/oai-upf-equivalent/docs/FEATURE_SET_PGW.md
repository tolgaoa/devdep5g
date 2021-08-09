<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface Core PDN Gateway Feature Set</font></b>
    </td>
  </tr>
</table>

**Table of Contents**

1. [PGW Fundamentals](#1-pgw-fundamentals)
2. [OAI PGW Available Interfaces](#2-oai-pgw-available-interfaces)
3. [OAI PGW Conformance Functions](#3-oai-pgw-conformance-functions)

# 1. PGW Fundamentals #

(from [Wikipedia.org](https://en.wikipedia.org/wiki/System_Architecture_Evolution))

The Packet Data Network Gateway provides connectivity from the UE to external packet data networks by being the point of exit and entry of traffic for the UE.

A UE may have simultaneous connectivity with more than one PGW for accessing multiple PDNs.

The PGW performs:

-  Policy enforcement,
-  Packet filtering for each user,
-  Charging support,
-  Lawful interception and
-  Packet screening.

Another key role of the PGW is to act as the anchor for mobility between 3GPP and non-3GPP technologies such as WiMAX and 3GPP2 (CDMA 1X and EvDO)

# 2. OAI PGW Available Interfaces #

| **ID** | **Interface** | **Status**         | **Comments**                                   | **Protocols** |
| ------ | ------------- | ------------------ | ---------------------------------------------- | ------------- |
| 1      | S5 / S8       | :heavy_check_mark: | Available on control plane only.               | GTP-based     |
| 2      | Gx            | :x:                | Policy                                         |               |
| 3      | Gy            | :x:                | Charging                                       |               |
| 4      | SGi           | :heavy_check_mark: |                                                |               |

# 3. OAI SGW Conformance Functions #

Based on document **3GPP TS 23.401 V15.5.0 §4.4.3.3**.

| **ID** | **Classification**                                           | **Status**         | **Comments**                                   |
| ------ | ------------------------------------------------------------ | ------------------ | ---------------------------------------------- |
| 1      | Per-user based packet filtering (ie deep packet inspection)  | :heavy_check_mark: |                                                |
| 2      | Lawful Interception                                          | :x:                |                                                |
| 3      | UE IP address allocation                                     | :heavy_check_mark: | Pools of IP addresses                          |
| 4      | Transport level packet marking in the uplink and downlink    | :x:                |                                                |
| 5      | Accounting for inter-operator charging                       | :x:                |                                                |
| 6      | UL and DL service level charging as defined in TS 23.203     | :x:                |                                                |
| 7      | Interfacing OFCS through                                     | :x:                |                                                |
| 8      | UL and DL service level gating control                       | :x:                |                                                |
| 9      | UL and DL service level rate enforcement                     | :x:                |                                                |
| 10     | UL and DL rate enforcement based on APN-AMBR                 | :x:                |                                                |
| 11     | DL rate enforcement based on the accumulated MBRs of the     | :x:                |                                                |
|        | aggregate of SDFs with the same GBR QCI                      |                    |                                                |
| 12     | DHCPv4 (server and client) and DHCPv6 (client and server)    | :x:                |                                                |
| 13     | The network does not support PPP bearer type                 | :x:                |                                                |
| 14     | The PDN GW may support Non-IP data transfer                  | :x:                |                                                |
| 15     | Packet screening                                             | :x:                |                                                |
| 16     | Sending of one or more "end marker(s)" to the source SGW     | :x:                |                                                |
|        |   immediately after switching the path during SGW change     |                    |                                                |
| 17     | PCC related features (e.g. involving PCRF and OCS)           | :x:                |                                                |
| 18     | UL and DL bearer binding as defined in TS 23.203             | :x:                |                                                |
| 19     | UL bearer binding verification as defined in TS 23.203       | :x:                |                                                |
| 20     | Functionality as defined in RFC 4861                         | :x:                |                                                |
| 21     | Accounting per UE and bearer                                 | :x:                |                                                |

