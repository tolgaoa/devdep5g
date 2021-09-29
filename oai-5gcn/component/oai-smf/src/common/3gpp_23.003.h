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

/*! \file 3gpp_23.003.h
 \brief
 \author Lionel Gauthier
 \company Eurecom
 \email: lionel.gauthier@eurecom.fr
 */
#ifndef FILE_3GPP_23_003_SEEN
#define FILE_3GPP_23_003_SEEN

#include <stdint.h>

typedef struct plmn_s {
  uint8_t mcc_digit2 : 4;
  uint8_t mcc_digit1 : 4;
  uint8_t mcc_digit3 : 4;
  uint8_t mnc_digit3 : 4;
  uint8_t mnc_digit2 : 4;
  uint8_t mnc_digit1 : 4;
} plmn_t;

#define INVALID_TAC_0000 (uint16_t) 0x0000
#define INVALID_TAC_FFFE (uint16_t) 0xFFFE
#define INVALID_TAC (uint32_t) 0x00000000

#define INVALID_TMSI                                                           \
  UINT32_MAX /*!< \brief  The network shall not allocate a TMSI with all 32    \
                bits equal to 1 (this is because the TMSI must be stored in    \
                the SIM, and the SIM uses 4 octets with all bits               \
                            equal to 1 to indicate that no valid TMSI is       \
                available).  */
typedef uint16_t tac_t;
typedef struct tai_s {
  plmn_t plmn; /*!< \brief  <MCC> + <MNC>        */
  tac_t tac;   /*!< \brief  Tracking Area Code   */
} tai_t;

typedef struct eci_s {
  uint32_t gnb_id : 20;
  uint32_t cell_id : 8;
  uint32_t empty : 4;
} ci_t;

typedef struct cgi_s {
  plmn_t plmn;
  ci_t cell_identity;  // 28 bits
} cgi_t;

typedef struct nr_tai_s /*5G ADD it*/
{
  plmn_t plmn;
  uint32_t tac : 24;
} nr_tai_t;

typedef struct nr_cell_identity_s /*5G ADD it */
{
  uint32_t gnb_id;
  uint8_t cell_id : 4;
} nr_cell_identity_t;

typedef struct nr_cgi_s /*5G ADD it */
{
  plmn_t plmn;
  nr_cell_identity_t cell_identity;
} nr_cgi_t;

typedef struct fiveG_s_tmsi_s /*5G ADD it */
{
  uint16_t amf_set_id : 10;
  uint8_t amf_pointer : 6;
  uint32_t fiveG_s_tmsi;  // 32
} fiveG_s_tmsi_t;

typedef struct fiveG_s_gua_s /*5G ADD it */
{
  plmn_t plmn;
  uint8_t region_id;
  uint16_t amf_set_id : 10;
  uint8_t amf_pointer : 6;

} fiveG_s_gua_t;

typedef struct amf_set_id_s /*5G ADD it*/
{
  uint16_t amf_set_id : 10;
} amf_set_id_t;

typedef struct allowed_nssai /*5G ADD it*/
{
  uint8_t sST;
  uint32_t sD : 24;
} allowed_nssai;

typedef struct allowed_nssai_s /*5G ADD it*/
{
  allowed_nssai* s_nssai;
  uint32_t count;
} allowed_nssai_t;

#endif
