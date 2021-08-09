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

#ifndef _SNSSAI_H_
#define _SNSSAI_H_

#include <stdint.h>
#include "bstrlib.h"

#define SNSSAI_MINIMUM_LENGTH 3
#define SNSSAI_MAXIMUM_LENGTH 10

#define SNSSAI_MINIMUM_LENGTH_TLV 3
#define SNSSAI_MAXIMUM_LENGTH_TLV 10

typedef enum {
  SST_LENGTH                                             = 0b00000001,
  SST_AND_MAPPEDHPLMNSST_LENGTH                          = 0b00000010,
  SST_AND_SD_LENGTH                                      = 0b00000100,
  SST_AND_SD_AND_MAPPEDHPLMNSST_LENGTH                   = 0b00000101,
  SST_AND_SD_AND_MAPPEDHPLMNSST_AND_MAPPEDHPLMNSD_LENGTH = 0b00001000
} length_of_snssai_contents;

typedef struct {
  length_of_snssai_contents len;
  uint8_t sst;
  uint32_t sd : 24;
  uint8_t mappedhplmnsst;
  uint32_t mappedhplmnsd;
} SNSSAI;

int encode_snssai(SNSSAI snssai, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_snssai(SNSSAI* snssai, uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
