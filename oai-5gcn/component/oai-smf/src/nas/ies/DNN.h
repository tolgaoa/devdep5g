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

#ifndef _DNN_H_
#define _DNN_H_

#include <stdint.h>
#include "bstrlib.h"

#define DNN_MINIMUM_LENGTH 3
#define DNN_MAXIMUM_LENGTH 102

#define DNN_MINIMUM_LENGTH_TLV 3
#define DNN_MAXIMUM_LENGTH_TLV 102

typedef bstring DNN;

int encode_dnn(DNN dnn, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_dnn(DNN* dnn, uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
