/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "_5GSNetworkFeatureSupport.h"

int encode__5gs_network_feature_support(
    _5GSNetworkFeatureSupport _5gsnetworkfeaturesupport, uint8_t iei,
    uint8_t* buffer, uint32_t len) {
  printf("encode__5gs_network_feature_support\n");
  uint8_t* lenPtr;
  uint32_t encoded  = 0;
  uint8_t bitStream = 0x0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, _5GS_NETWORK_FEATURE_SUPPORT_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);
  encoded++;

  // bitStream |= ((_5gsnetworkfeaturesupport.mpsi&0x01) |
  // (_5gsnetworkfeaturesupport.iwk_n26&0x01) |
  // (_5gsnetworkfeaturesupport.emf&0x03) | (_5gsnetworkfeaturesupport.emc&0x03)
  // | (_5gsnetworkfeaturesupport.ims_VoPS_N3GPP&0x01) |
  // (_5gsnetworkfeaturesupport.ims_VoPS_3GPP&0x01));
  bitStream |=
      (((_5gsnetworkfeaturesupport.mpsi & 0x01) << 7) |
       ((_5gsnetworkfeaturesupport.iwk_n26 & 0x01) << 6) |
       ((_5gsnetworkfeaturesupport.emf & 0x03) << 4) |
       ((_5gsnetworkfeaturesupport.emc & 0x03) << 2) |
       ((_5gsnetworkfeaturesupport.ims_VoPS_N3GPP & 0x01) << 1) |
       ((_5gsnetworkfeaturesupport.ims_VoPS_3GPP & 0x01) << 0));
  ENCODE_U8(buffer + encoded, bitStream, encoded);
  bitStream = 0x0;
  // bitStream |= (_5gsnetworkfeaturesupport.mcsi&0x01) |
  // (_5gsnetworkfeaturesupport.emcn&0x01);
  bitStream |= ((_5gsnetworkfeaturesupport.mcsi & 0x01) << 1) |
               ((_5gsnetworkfeaturesupport.emcn & 0x01) << 0);
  ENCODE_U8(buffer + encoded, bitStream, encoded);

  *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);
  return encoded;
}

int decode__5gs_network_feature_support(
    _5GSNetworkFeatureSupport* _5gsnetworkfeaturesupport, uint8_t iei,
    uint8_t* buffer, uint32_t len) {
  int decoded       = 0;
  uint8_t ielen     = 0;
  uint8_t bitStream = 0x0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  ielen = *(buffer + decoded);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  DECODE_U8(buffer + decoded, bitStream, decoded);
  _5gsnetworkfeaturesupport->mpsi           = ((bitStream & 0x80) >> 7);
  _5gsnetworkfeaturesupport->iwk_n26        = ((bitStream & 0x40) >> 6);
  _5gsnetworkfeaturesupport->emf            = ((bitStream & 0x30) >> 4);
  _5gsnetworkfeaturesupport->emc            = ((bitStream & 0x0c) >> 2);
  _5gsnetworkfeaturesupport->ims_VoPS_N3GPP = ((bitStream & 0x02) >> 1);
  _5gsnetworkfeaturesupport->ims_VoPS_3GPP  = ((bitStream & 0x01) >> 0);
  DECODE_U8(buffer + decoded, bitStream, decoded);
  _5gsnetworkfeaturesupport->mcsi = ((bitStream & 0x02) >> 1);
  _5gsnetworkfeaturesupport->emcn = ((bitStream & 0x01) >> 0);

#if 0
    DECODE_U8(buffer+decoded,bitStream,decoded);
	printf("decode__5gs_network_feature_support,bitStream:0x%x\n", bitStream);
    //_5gsnetworkfeaturesupport->mpsi = ((bitStream&0x80)>>8);
    _5gsnetworkfeaturesupport->mpsi = ((bitStream)>>8);
    _5gsnetworkfeaturesupport->iwk_n26 = ((bitStream&0x40)>>7);
    _5gsnetworkfeaturesupport->emf = ((bitStream&0x30)>>4);
    _5gsnetworkfeaturesupport->emc = ((bitStream&0x0c)>>2);
    _5gsnetworkfeaturesupport->ims_VoPS_N3GPP = ((bitStream&0x02)>>1);
    _5gsnetworkfeaturesupport->ims_VoPS_3GPP = ((bitStream&0x01)>>0);
    DECODE_U8(buffer+decoded,bitStream,decoded);
	printf("decode__5gs_network_feature_support,bitStream:0x%x\n",bitStream);
    //_5gsnetworkfeaturesupport->mcsi = ((bitStream&0x02)>>1);
    _5gsnetworkfeaturesupport->mcsi = ((bitStream&0x02)>>1);
    _5gsnetworkfeaturesupport->emcn = ((bitStream&0x01)>>0);
#endif

  return decoded;
}
