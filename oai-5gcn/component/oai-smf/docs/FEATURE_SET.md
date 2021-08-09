<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface SMF Feature Set</font></b>
    </td>
  </tr>
</table>

**Table of Contents**

1. [5GC Service Based Architecture](#1-5gc-service-based-architecture)
2. [OAI SMF Available Interfaces](#2-oai-smf-available-interfaces)
3. [OAI SMF Feature List](#3-oai-smf-feature-list)

# 1. 5GC Service Based Architecture #

![5GC SBA](./images/5gc_sba.png)

![Scope of the implementation](images/oai_5gc_current_status.jpg)

# 2. OAI SMF Available Interfaces #

| **ID** | **Interface** | **Status**         | **Comment**                                                               |
| ------ | ------------- | ------------------ | --------------------------------------------------------------------------|
| 1      | N4            | :heavy_check_mark: | between SMF and UPF (PFCP)                                                |
| 2      | N7            | :x:                | between SMF and PCF                                                       |
| 3      | N10           | :heavy_check_mark: | between SMF and UDM (Nudm_SubscriberDataManagement)                       |
| 4      | N11 (*)       | :heavy_check_mark: | between SMF and AMF (Nsmf_PDU_Session Services, Namf_N1N2MessageTransfer) |
| 5      | N16/16a       | :x:                | between SMFs                                                              |

(*): support both HTTP/1.1 and HTTP/2
# 3. OAI SMF Feature List #

Based on document **3GPP TS 23.501 v16.0.0 (Section 6.2.2)**.

| **ID** | **Classification**                                                  | **Status**         | **Comments**                                |
| ------ | ------------------------------------------------------------------- | ------------------ | ------------------------------------------- |
| 1      | Session Management (Session Establishment/Modification/Release)     | :heavy_check_mark: |                                             |
| 2      | UE IP address allocation & management​                               | :heavy_check_mark: | IP Address pool is controlled by SMF        |
| 3      | DHCPv4 (server and client) and DHCPv6 (server and client) function  | :x:                |                                             |
| 4      | Respond to ARP requests and/or IPv6 Neighbour Solicitation requests | :x:                |                                             |
| 5      | Selection of UPF function​                                           | :heavy_check_mark: | Local configuration in SMF                  |
| 6      | Configures traffic steering at UPF                                  | :x:                |                                             |
| 7      | Termination of interfaces towards PCFs                              | :x:                |                                             |
| 8      | Lawful intercept                                                    | :x:                |                                             |
| 8      | Charging data collection and support of charging interfaces         | :x:                |                                             |
| 10     | Termination of SM parts of NAS messages                             | :heavy_check_mark: |                                             |
| 11     | Downlink Data Notification                                          | :heavy_check_mark: |                                             |
| 12     | Determine SSC mode of a session​                                     | :heavy_check_mark: | Only support SSC mode 1​                     |
| 13     | Initiator of AN specific SM information, sent via AMF over N2 to AN | :heavy_check_mark: |                                             |
| 14     | Support for Control Plane CIoT 5GS Optimisation                     | :x:                |                                             |
| 15     | Support of header compression. ​                                     | :x:                |                                             |
| 16     | Act as I-SMF in deployments                                         | :x:                |                                             |
| 17     | Provisioning of external parameters                                 | :x:                |                                             |
