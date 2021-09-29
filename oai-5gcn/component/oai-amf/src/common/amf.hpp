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

/*! \file amf.hpp
 \brief
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef __AMF_HPP
#define __AMF_HPP

#include "3gpp_23.003.h"
/*
typedef struct {
  std::string mcc;
  std::string mnc;
  uint32_t tac;
} plmn_t;

typedef struct s_nssai  // section 28.4, TS23.003
{
  uint8_t sST;
  //uint32_t sD:24;
  std::string sD;
  s_nssai(const uint8_t &sst, const std::string sd)
      :
      sST(sst),
      sD(sd) {
  }
  s_nssai()
      :
      sST(),
      sD() {
  }
  s_nssai(const s_nssai &p)
      :
      sST(p.sST),
      sD(p.sD) {
  }
  bool operator==(const struct s_nssai &s) const {
    if ((s.sST == this->sST) && (s.sD.compare(this->sD) == 0)) {
      return true;
    } else {
      return false;
    }
  }

} snssai_t;
*/

// SMF + AMF + 3GPP TS 29.571 (Common data)
enum class http_response_codes_e {
  HTTP_RESPONSE_CODE_0                      = 0,
  HTTP_RESPONSE_CODE_200_OK                 = 200,
  HTTP_RESPONSE_CODE_201_CREATED            = 201,
  HTTP_RESPONSE_CODE_202_ACCEPTED           = 202,
  HTTP_RESPONSE_CODE_204_UPDATED            = 204,
  HTTP_RESPONSE_CODE_BAD_REQUEST            = 400,
  HTTP_RESPONSE_CODE_UNAUTHORIZED           = 401,
  HTTP_RESPONSE_CODE_FORBIDDEN              = 403,
  HTTP_RESPONSE_CODE_NOT_FOUND              = 404,
  HTTP_RESPONSE_CODE_METHOD_NOT_ALLOWED     = 405,
  HTTP_RESPONSE_CODE_REQUEST_TIMEOUT        = 408,
  HTTP_RESPONSE_CODE_406_NOT_ACCEPTED       = 406,
  HTTP_RESPONSE_CODE_CONFLICT               = 409,
  HTTP_RESPONSE_CODE_GONE                   = 410,
  HTTP_RESPONSE_CODE_LENGTH_REQUIRED        = 411,
  HTTP_RESPONSE_CODE_PRECONDITION_FAILED    = 412,
  HTTP_RESPONSE_CODE_PAYLOAD_TOO_LARGE      = 413,
  HTTP_RESPONSE_CODE_URI_TOO_LONG           = 414,
  HTTP_RESPONSE_CODE_UNSUPPORTED_MEDIA_TYPE = 415,
  HTTP_RESPONSE_CODE_TOO_MANY_REQUESTS      = 429,
  HTTP_RESPONSE_CODE_INTERNAL_SERVER_ERROR  = 500,
  HTTP_RESPONSE_CODE_NOT_IMPLEMENTED        = 501,
  HTTP_RESPONSE_CODE_SERVICE_UNAVAILABLE    = 503,
  HTTP_RESPONSE_CODE_GATEWAY_TIMEOUT        = 504
};

// for CURL
#define SMF_CURL_TIMEOUT_MS 100L
#define SMF_NUMBER_RETRIES 3
constexpr auto CURL_MIME_BOUNDARY = "----Boundary";
#define NRF_CURL_TIMEOUT_MS 100L
#define AUSF_CURL_TIMEOUT_MS 100L
#define CURL_TIMEOUT_MS 100L

#define BUFFER_SIZE_1024 1024
#define BUFFER_SIZE_512 512
#define BUFFER_SIZE_256 256

#endif
