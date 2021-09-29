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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "secu_defs.h"

#include <openssl/aes.h>
#include <openssl/cmac.h>
#include <openssl/evp.h>

// test
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include "assertions.h"
#include "conversions.h"
#include "dynamic_memory_check.h"

/*!
   @brief Create integrity cmac t for a given message.
   @param[in] stream_cipher Structure containing various variables to setup
   encoding
   @param[out] out For EIA2 the output string is 32 bits long
*/
int nas_stream_encrypt_nia2(
    nas_stream_cipher_t* const stream_cipher, uint8_t const out[4]) {
  return 0;
}
