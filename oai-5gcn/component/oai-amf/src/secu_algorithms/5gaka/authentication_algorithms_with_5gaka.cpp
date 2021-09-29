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

#include "authentication_algorithms_with_5gaka.hpp"

#include <errno.h>
#include <gmp.h>
#include <nettle/hmac.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OCTET_STRING.h"
#include "logger.hpp"

extern void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len);
extern void hexStr2Byte(const char* src, unsigned char* dest, int len);
/************ algorithm f1 **************/
/*
  Computes network authentication code MAC-A from key K, random, challenge RAND,
  sequence number SQN and authentication management field AMF.
*/

void Authentication_5gaka::f1(
    const uint8_t opc[16], const uint8_t k[16], const uint8_t _rand[16],
    const uint8_t sqn[6], const uint8_t amf[2], uint8_t mac_a[8]) {
  uint8_t temp[16];
  uint8_t in1[16];
  uint8_t out1[16];
  uint8_t rijndaelInput[16];
  uint8_t i;

  RijndaelKeySchedule(k);

  for (i = 0; i < 16; i++) rijndaelInput[i] = _rand[i] ^ opc[i];

  RijndaelEncrypt(rijndaelInput, temp);

  for (i = 0; i < 6; i++) {
    in1[i]     = sqn[i];
    in1[i + 8] = sqn[i];
  }

  for (i = 0; i < 2; i++) {
    in1[i + 6]  = amf[i];
    in1[i + 14] = amf[i];
  }

  /*
   * XOR op_c and in1, rotate by r1=64, and XOR
   * * * * on the constant c1 (which is all zeroes)
   */
  for (i = 0; i < 16; i++) rijndaelInput[(i + 8) % 16] = in1[i] ^ opc[i];

  /*
   * XOR on the value temp computed before
   */
  for (i = 0; i < 16; i++) rijndaelInput[i] ^= temp[i];

  RijndaelEncrypt(rijndaelInput, out1);
  for (i = 0; i < 16; i++) out1[i] ^= opc[i];

  for (i = 0; i < 8; i++) mac_a[i] = out1[i];

  return;
}

/*-------------------------------------------------------------------
   Algorithms f2-f5
  -------------------------------------------------------------------

   Takes key K and random challenge RAND, and returns response RES,
   confidentiality key CK, integrity key IK and anonymity key AK.

  -----------------------------------------------------------------*/
void Authentication_5gaka::f2345(
    const uint8_t opc[16], const uint8_t k[16], const uint8_t _rand[16],
    uint8_t res[8], uint8_t ck[16], uint8_t ik[16], uint8_t ak[6]) {
  uint8_t temp[16];
  uint8_t out[16];
  uint8_t rijndaelInput[16];
  uint8_t i;

  RijndaelKeySchedule(k);

  for (i = 0; i < 16; i++) rijndaelInput[i] = _rand[i] ^ opc[i];

  RijndaelEncrypt(rijndaelInput, temp);

  /*
   * To obtain output block OUT2: XOR OPc and TEMP,
   * * * * rotate by r2=0, and XOR on the constant c2 (which *
   * * * * is all zeroes except that the last bit is 1).
   */
  for (i = 0; i < 16; i++) rijndaelInput[i] = temp[i] ^ opc[i];

  rijndaelInput[15] ^= 1;
  RijndaelEncrypt(rijndaelInput, out);

  for (i = 0; i < 16; i++) out[i] ^= opc[i];

  for (i = 0; i < 8; i++) res[i] = out[i + 8];

  for (i = 0; i < 6; i++) ak[i] = out[i];
  /*
   * To obtain output block OUT3: XOR OPc and TEMP,
   * * * * rotate by r3=32, and XOR on the constant c3 (which *
   * * * * is all zeroes except that the next to last bit is 1).
   */

  for (i = 0; i < 16; i++) rijndaelInput[(i + 12) % 16] = temp[i] ^ opc[i];

  rijndaelInput[15] ^= 2;
  RijndaelEncrypt(rijndaelInput, out);

  for (i = 0; i < 16; i++) out[i] ^= opc[i];

  for (i = 0; i < 16; i++) ck[i] = out[i];

  /*
   * To obtain output block OUT4: XOR OPc and TEMP,
   * * * * rotate by r4=64, and XOR on the constant c4 (which *
   * * * * is all zeroes except that the 2nd from last bit is 1).
   */
  for (i = 0; i < 16; i++) rijndaelInput[(i + 8) % 16] = temp[i] ^ opc[i];

  rijndaelInput[15] ^= 4;
  RijndaelEncrypt(rijndaelInput, out);

  for (i = 0; i < 16; i++) out[i] ^= opc[i];

  for (i = 0; i < 16; i++) ik[i] = out[i];

  return;
} /* end of function f2345 */

/*-------------------------------------------------------------------
   Algorithm f1
  -------------------------------------------------------------------

   Computes resynch authentication code MAC-S from key K, random
   challenge RAND, sequence number SQN and authentication management
   field AMF.

  -----------------------------------------------------------------*/

void Authentication_5gaka::f1star(
    const uint8_t opc[16], const uint8_t k[16], const uint8_t _rand[16],
    const uint8_t sqn[6], const uint8_t amf[2], uint8_t mac_s[8]) {
  uint8_t temp[16];
  uint8_t in1[16];
  uint8_t out1[16];
  uint8_t rijndaelInput[16];
  uint8_t i;

  RijndaelKeySchedule(k);

  for (i = 0; i < 16; i++) rijndaelInput[i] = _rand[i] ^ opc[i];

  RijndaelEncrypt(rijndaelInput, temp);

  for (i = 0; i < 6; i++) {
    in1[i]     = sqn[i];
    in1[i + 8] = sqn[i];
  }

  for (i = 0; i < 2; i++) {
    in1[i + 6]  = amf[i];
    in1[i + 14] = amf[i];
  }

  /*
   * XOR op_c and in1, rotate by r1=64, and XOR
   * * * * on the constant c1 (which is all zeroes)
   */
  for (i = 0; i < 16; i++) rijndaelInput[(i + 8) % 16] = in1[i] ^ opc[i];

  /*
   * XOR on the value temp computed before
   */
  for (i = 0; i < 16; i++) rijndaelInput[i] ^= temp[i];

  RijndaelEncrypt(rijndaelInput, out1);

  for (i = 0; i < 16; i++) out1[i] ^= opc[i];

  for (i = 0; i < 8; i++) mac_s[i] = out1[i + 8];

  return;
}

/*-------------------------------------------------------------------
   Algorithm f5
  -------------------------------------------------------------------

   Takes key K and random challenge RAND, and returns resynch
   anonymity key AK.

  -----------------------------------------------------------------*/
void Authentication_5gaka::f5star(
    const uint8_t opc[16], const uint8_t k[16], const uint8_t _rand[16],
    uint8_t ak[6]) {
  uint8_t temp[16];
  uint8_t out[16];
  uint8_t rijndaelInput[16];
  uint8_t i;

  RijndaelKeySchedule(k);

  for (i = 0; i < 16; i++) rijndaelInput[i] = _rand[i] ^ opc[i];

  RijndaelEncrypt(rijndaelInput, temp);

  /*
   * To obtain output block OUT5: XOR OPc and TEMP,
   * * * * rotate by r5=96, and XOR on the constant c5 (which *
   * * * * is all zeroes except that the 3rd from last bit is 1).
   */
  for (i = 0; i < 16; i++) rijndaelInput[(i + 4) % 16] = temp[i] ^ opc[i];

  rijndaelInput[15] ^= 8;
  RijndaelEncrypt(rijndaelInput, out);

  for (i = 0; i < 16; i++) out[i] ^= opc[i];

  for (i = 0; i < 6; i++) ak[i] = out[i];

  return;
}

/*-------------------------------------------------------------------
   Function to compute OPc from OP and K.
  -----------------------------------------------------------------*/
void Authentication_5gaka::ComputeOPc(
    const uint8_t kP[16], const uint8_t opP[16], uint8_t opcP[16]) {
  uint8_t i;

  RijndaelKeySchedule(kP);
  // FPRINTF_DEBUG ("Compute
  // opc:\n\tK:\t%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
  // kP[0], kP[1], kP[2], kP[3], kP[4], kP[5], kP[6], kP[7], kP[8], kP[9],
  // kP[10], kP[11], kP[12], kP[13], kP[14], kP[15]);
  RijndaelEncrypt(opP, opcP);
  // FPRINTF_DEBUG
  // ("\tIn:\t%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n\tRinj:\t%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
  //        opP[0], opP[1], opP[2], opP[3], opP[4], opP[5], opP[6], opP[7],
  //        opP[8], opP[9], opP[10], opP[11], opP[12], opP[13], opP[14],
  //        opP[15], opcP[0], opcP[1], opcP[2], opcP[3], opcP[4], opcP[5],
  //        opcP[6], opcP[7], opcP[8], opcP[9], opcP[10], opcP[11], opcP[12],
  //        opcP[13], opcP[14], opcP[15]);

  for (i = 0; i < 16; i++) opcP[i] ^= opP[i];

  // FPRINTF_DEBUG
  // ("\tOut:\t%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
  // opcP[0], opcP[1], opcP[2], opcP[3], opcP[4], opcP[5], opcP[6], opcP[7],
  // opcP[8], opcP[9], opcP[10], opcP[11], opcP[12], opcP[13], opcP[14],
  // opcP[15]);
  return;
}

void Authentication_5gaka::generate_autn(
    const uint8_t sqn[6], const uint8_t ak[6], const uint8_t amf[2],
    const uint8_t mac_a[8], uint8_t autn[16]) {
  for (int i = 0; i < 6; i++) {
    autn[i] = sqn[i] ^ ak[i];
  }
  memcpy(&autn[6], amf, 2);
  memcpy(&autn[8], mac_a, 8);
}

void Authentication_5gaka::kdf(
    uint8_t* key, uint16_t key_len, uint8_t* s, uint16_t s_len, uint8_t* out,
    uint16_t out_len) {
  struct hmac_sha256_ctx ctx;
  memset(&ctx, 0, sizeof(ctx));
  hmac_sha256_set_key(&ctx, key_len, key);
  hmac_sha256_update(&ctx, s_len, s);
  hmac_sha256_digest(&ctx, out_len, out);
}

void Authentication_5gaka::derive_kseaf(
    std::string serving_network, uint8_t kausf[32], uint8_t kseaf[32]) {
  Logger::amf_n1().debug("derive_kseaf ...");
  // Logger::amf_n1().debug("inputstring: snn(%s)", serving_network.c_str());
  OCTET_STRING_t netName;
  OCTET_STRING_fromBuf(
      &netName, serving_network.c_str(), serving_network.length());
  // print_buffer("amf_n1", "inputstring: snn(hex)", netName.buf, netName.size);
  uint8_t S[100];
  S[0] = 0x6C;  // FC
  memcpy(&S[1], netName.buf, netName.size);
  // memcpy (&S[1+netName.size], &netName.size, 2);
  S[1 + netName.size] = (uint8_t)((netName.size & 0xff00) >> 8);
  S[2 + netName.size] = (uint8_t)(netName.size & 0x00ff);
  // print_buffer("amf_n1", "inputstring S", S, 3+netName.size);
  // print_buffer("amf_n1", "key KEY", kausf, 32);
  kdf(kausf, 32, S, 3 + netName.size, kseaf, 32);
  // print_buffer("amf_n1", "KDF out: Kseaf", kseaf, 32);
  // Logger::amf_n1().debug("derive kseaf finished!");
}

void Authentication_5gaka::derive_kausf(
    uint8_t ck[16], uint8_t ik[16], std::string serving_network, uint8_t sqn[6],
    uint8_t ak[6], uint8_t kausf[32]) {
  Logger::amf_n1().debug("derive_kausf ...");
  // Logger::amf_n1().debug("inputstring: snn(%s)", serving_network.c_str());
  OCTET_STRING_t netName;
  OCTET_STRING_fromBuf(
      &netName, serving_network.c_str(), serving_network.length());
  // print_buffer("amf_n1", "inputstring: snn(hex)", netName.buf, netName.size);
  uint8_t S[100];
  uint8_t key[32];
  memcpy(&key[0], ck, 16);
  memcpy(&key[16], ik, 16);  // KEY
  S[0] = 0x6A;
  memcpy(&S[1], netName.buf, netName.size);
  // memcpy (&S[1+netName.size], &netName.size, 2);
  S[1 + netName.size] = (uint8_t)((netName.size & 0xff00) >> 8);
  S[2 + netName.size] = (uint8_t)(netName.size & 0x00ff);
  for (int i = 0; i < 6; i++) {
    S[3 + netName.size + i] = sqn[i] ^ ak[i];
  }
  S[9 + netName.size]  = 0x00;
  S[10 + netName.size] = 0x06;
  // print_buffer("amf_n1", "inputstring S", S, 11+netName.size);
  // print_buffer("amf_n1", "key KEY", key, 32);
  kdf(key, 32, S, 11 + netName.size, kausf, 32);
  // print_buffer("amf_n1", "KDF out: Kausf", kausf, 32);
  // Logger::amf_n1().debug("derive kausf finished!");
}

void Authentication_5gaka::derive_kamf(
    std::string imsi, uint8_t* kseaf, uint8_t* kamf, uint16_t abba) {
  Logger::amf_n1().debug("derive_kamf ...");
  std::string ueSupi = imsi;  // OK
  // Logger::amf_n1().debug("inputstring: supi(%s)", ueSupi.c_str());
  // int supiLen = (imsi.length()*sizeof(unsigned char))/2;
  // unsigned char * supi = (unsigned char*)calloc(1, supiLen);
  // hexStr2Byte(imsi.c_str(), supi, imsi.length());
  OCTET_STRING_t supi;
  OCTET_STRING_fromBuf(&supi, ueSupi.c_str(), ueSupi.length());
  // uint8_t supi[8] = {0x64, 0xf0, 0x11, 0x10, 0x32, 0x54, 0x76, 0x98};
  int supiLen = supi.size;
  // print_buffer("amf_n1", "inputstring: supi(hex)", supi.buf, supiLen);
  uint8_t S[100];
  S[0] = 0x6D;  // FC = 0x6D
  memcpy(&S[1], supi.buf, supiLen);
  // memcpy (&S[1+supiLen], &supiLen, 2);
  S[1 + supiLen] = (uint8_t)((supiLen & 0xff00) >> 8);
  S[2 + supiLen] = (uint8_t)(supiLen & 0x00ff);
  S[3 + supiLen] = abba & 0x00ff;
  S[4 + supiLen] = (abba & 0xff00) >> 8;
  S[5 + supiLen] = 0x00;
  S[6 + supiLen] = 0x02;
  // print_buffer("amf_n1", "inputstring S", S, 7+supiLen);
  // print_buffer("amf_n1", "key KEY", kseaf, 32);
  kdf(kseaf, 32, S, 7 + supiLen, kamf, 32);
  // print_buffer("amf_n1", "KDF out: Kamf", kamf, 32);
  // Logger::amf_n1().debug("derive kamf finished!");
}

void Authentication_5gaka::derive_knas(
    algorithm_type_dist_t nas_alg_type, uint8_t nas_alg_id, uint8_t kamf[32],
    uint8_t* knas) {
  Logger::amf_n1().debug("derive_knas ...");
  uint8_t S[20];
  uint8_t out[32] = {0};
  S[0]            = 0x69;  // FC
  S[1]            = (uint8_t)(nas_alg_type & 0xFF);
  S[2]            = 0x00;
  S[3]            = 0x01;
  S[4]            = nas_alg_id;
  S[5]            = 0x00;
  S[6]            = 0x01;
  // print_buffer("amf_n1", "inputstring S", S, 7);
  // print_buffer("amf_n1", "key KEY", kamf, 32);
  kdf(kamf, 32, S, 7, out, 32);
  // memcpy (knas, &out[31 - 16 + 1], 16);
  for (int i = 0; i < 16; i++) knas[i] = out[16 + i];
  // print_buffer("amf_n1", "knas", knas, 16);
  // Logger::amf_n1().debug("derive knas finished!");
}

void Authentication_5gaka::derive_kgnb(
    uint32_t uplinkCount, uint8_t accessType, uint8_t kamf[32], uint8_t* kgnb) {
  Logger::amf_n1().debug("derive_kgnb ...");
  uint8_t S[20];
  S[0]                 = 0x6E;
  *(uint32_t*) (S + 1) = htonl(uplinkCount);
  S[5]                 = 0x00;
  S[6]                 = 0x04;
  S[7]                 = accessType;
  S[8]                 = 0x00;
  S[9]                 = 0x01;
  // print_buffer("amf_n1", "inputstring S", S, 10);
  // print_buffer("amf_n1", "key KEY", kamf, 32);
  kdf(kamf, 32, S, 10, kgnb, 32);
  // print_buffer("amf_n1", "kgnb", kgnb, 32);
  // Logger::amf_n1().debug("derive kgnb finished!");
}
void Authentication_5gaka::handover_ncc_derive_knh(
    uint32_t uplinkCount, uint8_t accessType, uint8_t kamf[32], uint8_t* kgnb,
    uint8_t* knh, int ncc) {
  Logger::amf_n1().debug("derive_handover_ncc_knh ...");
  uint8_t S[20], SS[ncc][35];
  S[0]                 = 0x6E;
  *(uint32_t*) (S + 1) = htonl(uplinkCount);
  S[5]                 = 0x00;
  S[6]                 = 0x04;
  S[7]                 = accessType;
  S[8]                 = 0x00;
  S[9]                 = 0x01;
  kdf(kamf, 32, S, 10, kgnb, 32);
  for (int i = 0; i < ncc; i++) {
    if (i == 0) {
      SS[0][0] = 0x6f;
      memcpy(SS[0] + 1, kgnb, 32);
      SS[0][33] = 0x00;
      SS[0][34] = 0x20;
      kdf(kamf, 32, SS[0], 35, knh, 32);
    } else {
      SS[i][0] = 0x6f;
      memcpy(SS[i] + 1, knh, 32);
      SS[i][33] = 0x00;
      SS[i][34] = 0x20;
      kdf(kamf, 32, SS[i], 35, knh, 32);
    }
    print_buffer("amf_n1", "SS", SS[i], 35);
    print_buffer("amf_n1", "Knh", knh, 32);
  }
}
void Authentication_5gaka::derive_kasme(
    uint8_t ck[16], uint8_t ik[16], uint8_t plmn[3], uint8_t sqn[6],
    uint8_t ak[6], uint8_t* kasme) {
  uint8_t s[14];
  int i;
  uint8_t key[32];
  memcpy(&key[0], ck, 16);
  memcpy(&key[16], ik, 16);
  /*
   * FC
   */
  s[0] = 0x10;
  /*
   * SN id is composed of MCC and MNC
   * * * * Octets:
   * * * *   1      MCC digit 2 | MCC digit 1
   * * * *   2      MNC digit 3 | MCC digit 3
   * * * *   3      MNC digit 2 | MNC digit 1
   */
  memcpy(&s[1], plmn, 3);
  /*
   * L0
   */
  s[4] = 0x00;
  s[5] = 0x03;

  /*
   * P1
   */
  for (i = 0; i < 6; i++) {
    s[6 + i] = sqn[i] ^ ak[i];
  }
  /*
   * L1
   */
  s[12] = 0x00;
  s[13] = 0x06;
  kdf(key, 32, s, 14, kasme, 32);
}

int Authentication_5gaka::generate_vector(
    const uint8_t opc[16], uint64_t imsi, uint8_t key[16], uint8_t plmn[3],
    uint8_t sqn[6], auc_vector_t* vector) {
  uint8_t amf[] = {0x80, 0x00};
  uint8_t mac_a[8];
  uint8_t ck[16];
  uint8_t ik[16];
  uint8_t ak[6];

  if (vector == NULL) {
    return EINVAL;
  }

  /*
   * Compute MAC
   */
  f1(opc, key, vector->rand, sqn, amf, mac_a);
  // print_buffer ("MAC_A   : ", mac_a, 8);
  // print_buffer ("SQN     : ", sqn, 6);
  // print_buffer ("RAND    : ", vector->rand, 16);
  /*
   * Compute XRES, CK, IK, AK
   */
  f2345(opc, key, vector->rand, vector->xres, ck, ik, ak);
  // print_buffer ("AK      : ", ak, 6);
  // print_buffer ("CK      : ", ck, 16);
  // print_buffer ("IK      : ", ik, 16);
  // print_buffer ("XRES    : ", vector->xres, 8);
  /*
   * AUTN = SQN ^ AK || AMF || MAC
   */
  generate_autn(sqn, ak, amf, mac_a, vector->autn);
  // print_buffer ("AUTN    : ", vector->autn, 16);
  derive_kasme(ck, ik, plmn, sqn, ak, vector->kasme);
  // print_buffer ("KASME   : ", vector->kasme, 32);
  return 0;
}

uint8_t* Authentication_5gaka::sqn_ms_derive(
    const uint8_t opc[16], uint8_t* key, uint8_t* auts, uint8_t* rand_p) {
  /*
   * AUTS = Conc(SQN MS ) || MAC-S
   * * * * Conc(SQN MS ) = SQN MS ^ f5* (RAND)
   * * * * MAC-S = f1* (SQN MS || RAND || AMF)
   */
  uint8_t ak[6]                        = {0};
  uint8_t* conc_sqn_ms                 = NULL;
  uint8_t* mac_s                       = NULL;
  uint8_t mac_s_computed[MAC_S_LENGTH] = {0};
  uint8_t* sqn_ms                      = NULL;
  uint8_t amf[2]                       = {0x00, 0x00};
  int i                                = 0;

  conc_sqn_ms = auts;
  mac_s       = &auts[6];
  sqn_ms      = (uint8_t*) malloc(SQN_LENGTH_OCTEST);
  /*
   * if (hss_config.valid_opc == 0) {
   * SetOP(hss_config.operator_key);
   * }
   */
  /*
   * Derive AK from key and rand
   */
  f5star(opc, key, rand_p, ak);

  for (i = 0; i < 6; i++) {
    sqn_ms[i] = ak[i] ^ conc_sqn_ms[i];
  }

  print_buffer("amf_n1", "sqn_ms_derive() KEY    : ", key, 16);
  print_buffer("amf_n1", "sqn_ms_derive() RAND   : ", rand_p, 16);
  print_buffer("amf_n1", "sqn_ms_derive() AUTS   : ", auts, 14);
  print_buffer("amf_n1", "sqn_ms_derive() AK     : ", ak, 6);
  print_buffer("amf_n1", "sqn_ms_derive() SQN_MS : ", sqn_ms, 6);
  print_buffer("amf_n1", "sqn_ms_derive() MAC_S  : ", mac_s, 8);
  f1star(opc, key, rand_p, sqn_ms, amf, mac_s_computed);
  print_buffer("amf_n1", "MAC_S computed : ", mac_s_computed, 8);

  if (memcmp(mac_s_computed, mac_s, 8) != 0) {
    // FPRINTF_ERROR ( "Failed to verify computed SQN_MS\n");
    Logger::amf_n1().warn("Failed to verify computed SQN_MS");
    free(sqn_ms);
    return NULL;
  }

  return sqn_ms;
}
