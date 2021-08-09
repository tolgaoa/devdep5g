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
#include "SORTransparentContainer.h"

int encode_sor_transparent_container(
    SORTransparentContainer sortransparentcontainer, uint8_t iei,
    uint8_t* buffer, uint32_t len) {
  uint8_t* lenPtr  = NULL;
  uint32_t encoded = 0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, SOR_TRANSPARENT_CONTAINER_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);
  encoded++;

  ENCODE_U8(buffer + encoded, sortransparentcontainer.sorHeader, encoded);

  *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);
  return encoded;

  /*
   uint8_t *lenPtr;
   uint32_t encoded = 0;
   int encode_result;
   CHECK_PDU_POINTER_AND_LENGTH_ENCODER
   (buffer,SOR_TRANSPARENT_CONTAINER_MINIMUM_LENGTH , len);


   if( iei >0  )
   {
   *buffer=iei;
   encoded++;
   }



   lenPtr = (buffer + encoded);
   encoded++;
   encoded++;



   if ((encode_result = encode_bstring (sortransparentcontainer, buffer +
   encoded, len - encoded)) < 0)//加密,实体,首地址,长度 return encode_result;
   else
   encoded += encode_result;

   uint32_t res = encoded - 1 - ((iei > 0) ? 1 : 0);
   *lenPtr =res/(1<<8);
   lenPtr++;
   *lenPtr = res%(1<<8);

   return encoded;
   */
}

int decode_sor_transparent_container(
    SORTransparentContainer* sortransparentcontainer, uint8_t iei,
    uint8_t* buffer, uint32_t len) {
  int decoded   = 0;
  uint8_t ielen = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  ielen = *(buffer + decoded);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  DECODE_U8(buffer + decoded, sortransparentcontainer->sorHeader, decoded);
  return decoded;
  /*
   int decoded=0;
   uint8_t ielen=0;
   int decode_result;

   if (iei > 0)
   {
   CHECK_IEI_DECODER (iei, *buffer);
   decoded++;
   }


   ielen = *(buffer + decoded);
   decoded++;
   ielen = ( ielen << 8)+*(buffer + decoded);
   decoded++;
   CHECK_LENGTH_DECODER (len - decoded, ielen);


   if((decode_result = decode_bstring (sortransparentcontainer, ielen, buffer +
   decoded, len - decoded)) < 0) return decode_result; else decoded +=
   decode_result; return decoded;
   */
}
