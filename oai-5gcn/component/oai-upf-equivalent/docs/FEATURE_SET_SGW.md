<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface Core Serving Gateway Feature Set</font></b>
    </td>
  </tr>
</table>

**Table of Contents**

1. [SGW Fundamentals](#1-sgw-fundamentals)
2. [OAI SGW Available Interfaces](#2-oai-sgw-available-interfaces)
3. [OAI SGW Conformance Functions](#3-oai-sgw-conformance-functions)

# 1. SGW Fundamentals #

(from [Wikipedia.org](https://en.wikipedia.org/wiki/System_Architecture_Evolution))

The Serving Gateway routes and forwards user data packets, while also acting as the mobility anchor for the user plane during inter-eNodeB handovers and as the anchor for mobility between LTE and other 3GPP technologies (terminating S4 interface and relaying the traffic between 2G/3G systems and PGW).

For idle state UEs, the SGW terminates the downlink data path and triggers paging when downlink data arrives for the UE.

*  It manages and stores UE contexts, e.g. parameters of the IP bearer service, network internal routing information.
*  It also performs replication of the user traffic in case of lawful interception. 

# 2. OAI SGW Available Interfaces #

| **ID** | **Interface** | **Status**         | **Comments**                                   | **Protocols** |
| ------ | ------------- | ------------------ | ---------------------------------------------- | ------------- |
| 1      | S5 / S8       | :heavy_check_mark: | Split in control plane only                    | GTP-C/U       |
|        |               | :x:                | Missing split in User plane                    |               |
| 2      | S1-U          | :heavy_check_mark: | Split in control plane only                    | GTP-U/UDP     |
| 3      | S11           | :heavy_check_mark: | S11-C only actually                            | GTP-C/UDP     |
| 4      | S4            | :x:                | No interconnection with SGSN                   | GTP-C/UDP     |
| 5      | S12           | :x:                | No interconnection with UTRAN                  | GTP-U/UDP     |

# 3. OAI SGW Conformance Functions #

Based on document **3GPP TS 23.401 V15.5.0 §4.4.3.2**.

| **ID** | **Classification**                                           | **Status**         | **Comments**                                   |
| ------ | ------------------------------------------------------------ | ------------------ | ---------------------------------------------- |
| 1      | Local Mobility Anchor point for inter eNodeB handover        | :question:         | Should be, X2HO have to be tested.             |
|        | (except when user data is transported using the Control      |                    |                                                |
|        |   Plane CIoT EPS optimization)                               |                    |                                                |
| 2      | Sending sending of one or more "end marker" to               | :x:                | Could be requested                             |
|        | -- the source eNodeB,                                        |                    |                                                |
|        | -- the source SGSN or                                        |                    |                                                |
|        | -- the source RNC                                            |                    |                                                |
|        | immediately after the Serving GW switches the path during    |                    |                                                |
|        | inter-eNodeB and inter-RAT handover(s),                      |                    |                                                |
|        | especially to assist the reordering function in eNodeB       |                    |                                                |
| 3      | Mobility anchoring for inter-3GPP mobility                   | :x:                | No Support of 2G and 3G systems                |
|        | (terminating S4 and relaying the traffic between 2G/3G system and PDN GW) |       |                                                |
| 4      | ECM-IDLE mode downlink packet buffering and                  | :x:                | Buffering is not supported in idle-mode        |
|        |  initiation of network triggered service request procedure   |                    |                                                |
|        |  and optionally Paging Policy Differentiation                |                    |                                                |
| 5      | Lawful Interception                                          | :x:                |                                                |
| 6      | Packet routing and forwarding                                | :heavy_check_mark: |                                                |
| 7      | Transport level packet marking in the uplink & the downlink  | :x:                | Could be supported                             |
|        |  e.g. setting the DiffServ Code Point, based on the QCI, and |                    |                                                |
|        |  optionally the ARP priority level, of the associated EPS bearer |                |                                                |
| 8      | Accounting for inter-operator charging.                      | :x:                |                                                |
|        | -- For GTP based S5/S8, the SGW generates accounting data per UE and bearer |     |                                                |
| 9      | Interfacing OFCS according to charging principles and        | :x:                |                                                |
|        |   through reference points specified in TS 32.240            |                    |                                                |
| 10     | Forwarding of "end marker" to the                            | :x:                |                                                |
|        | -- the source eNodeB,                                        |                    |                                                |
|        | -- the source SGSN or                                        |                    |                                                |
|        | -- the source RNC                                            |                    |                                                |
|        | when the "end marker" is received from PGW and SGW has downlink user plane established. | |                                        |
|        | Upon reception of "end marker", the Serving GW shall not send Downlink Data Notification. | |                                      |
