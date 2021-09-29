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

#ifndef _5GAKA_H_
#define _5GAKA_H_

// extern "C"{
#include <gmp.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
//}

#define SQN_LENGTH_BITS (48)
#define SQN_LENGTH_OCTEST (SQN_LENGTH_BITS / 8)
#define IK_LENGTH_BITS (128)
#define IK_LENGTH_OCTETS (IK_LENGTH_BITS / 8)
#define RAND_LENGTH_OCTETS (16)
#define RAND_LENGTH_BITS (RAND_LENGTH_OCTETS * 8)
#define XRES_LENGTH_OCTETS (8)
#define AUTN_LENGTH_OCTETS (16)
#define KASME_LENGTH_OCTETS (32)
#define MAC_S_LENGTH (8)

typedef mpz_t random_t;
typedef mpz_t sqn_t;

typedef struct {
  uint8_t rand[16];
  uint8_t rand_new;
  uint8_t xres[8];
  uint8_t autn[16];
  uint8_t kasme[32];
} auc_vector_t;

typedef struct {
  uint8_t avType;
  uint8_t rand[16];
  uint8_t xres[8];
  uint8_t xresStar[16];
  uint8_t autn[16];
  uint8_t kausf[32];
} _5G_HE_AV_t;  // clause 6.3.6.2.5, ts33.501

typedef struct {
  uint8_t avType;
  uint8_t rand[16];
  uint8_t hxres[16];
  uint8_t hxresStar[16];
  uint8_t autn[16];
  uint8_t kseaf[32];
} _5G_AV_t;

typedef struct random_state_s {
  pthread_mutex_t lock;
  gmp_randstate_t state;
} random_state_t;

#define _NEA0_ 0b0000
#define _128_NEA1_ 0b0001
#define _128_NEA2_ 0b0010
#define _128_NEA3_ 0b0011
#define _NIA0_ 0b0000
#define _128_NIA1_ 0b0001
#define _128_NIA2_ 0b0010
#define _128_NIA3_ 0b0011

typedef enum {
  NAS_ENC_ALG = 0x01,
  NAS_INT_ALG = 0x02,
  RRC_ENC_ALG = 0x03,
  RRC_INT_ALG = 0x04,
  UP_ENC_ALG  = 0x05,
  UP_INT_ALG  = 0x06
} algorithm_type_dist_t;

class Authentication_5gaka {
 public:
  /****** sequence number functions ********/
  // struct sqn_ue_s *sqn_exists(uint64_t imsi);
  // void sqn_insert(struct sqn_ue_s *item);
  // void sqn_init(struct sqn_ue_s *item);
  // struct sqn_ue_s *sqn_new(uint64_t imsi);
  // void sqn_list_init(void);
  // void sqn_get(uint64_t imsi, uint8_t sqn[6]);
 public:
  /****** random number functions *********/
  // void random_init(void);
  // void generate_random(uint8_t *random, ssize_t length);
 public:
  /****** internal algorithms f1 f2 f3 f4 f5 ********/
  static void f1(
      const uint8_t opc[16], const uint8_t k[16], const uint8_t _rand[16],
      const uint8_t sqn[6], const uint8_t amf[2], uint8_t mac_a[8]);
  static void f1star(
      const uint8_t kP[16], const uint8_t k[16], const uint8_t rand[16],
      const uint8_t sqn[6], const uint8_t amf[2], uint8_t mac_s[8]);
  static void f2345(
      const uint8_t opc[16], const uint8_t k[16], const uint8_t _rand[16],
      uint8_t res[8], uint8_t ck[16], uint8_t ik[16], uint8_t ak[6]);
  static void f5star(
      const uint8_t kP[16], const uint8_t k[16], const uint8_t rand[16],
      uint8_t ak[6]);

 public:
  /****** key derive  ***********/
  static void kdf(
      uint8_t* key, uint16_t key_len, uint8_t* s, uint16_t s_len, uint8_t* out,
      uint16_t out_len);
  static void derive_kasme(
      uint8_t ck[16], uint8_t ik[16], uint8_t plmn[3], uint8_t sqn[6],
      uint8_t ak[6], uint8_t kasme[32]);
  static void derive_kausf(
      uint8_t ck[16], uint8_t ik[16], std::string serving_network,
      uint8_t sqn[6], uint8_t ak[6], uint8_t kausf[32]);
  static void derive_kseaf(
      std::string serving_network, uint8_t kausf[32], uint8_t kseaf[32]);
  static void derive_kamf(
      std::string imsi, uint8_t* kseaf, uint8_t* kamf, uint16_t abba);
  static void derive_knas(
      algorithm_type_dist_t nas_alg_type, uint8_t nas_alg_id, uint8_t kamf[32],
      uint8_t* knas);
  static void derive_kgnb(
      uint32_t uplinkCount, uint8_t accessType, uint8_t kamf[32],
      uint8_t* kgnb);
  static void handover_ncc_derive_knh(
      uint32_t uplinkCount, uint8_t accessType, uint8_t kamf[32], uint8_t* kgnb,
      uint8_t* knh, int ncc);
  static uint8_t* sqn_ms_derive(
      const uint8_t opc[16], uint8_t* key, uint8_t* auts, uint8_t* rand);

 public:
  /****** general functions ********/
  static void ComputeOPc(
      const uint8_t kP[16], const uint8_t opP[16], uint8_t opcP[16]);
  static void generate_autn(
      const uint8_t sqn[6], const uint8_t ak[6], const uint8_t amf[2],
      const uint8_t mac_a[8], uint8_t autn[16]);
  static int generate_vector(
      const uint8_t opc[16], uint64_t imsi, uint8_t key[16], uint8_t plmn[3],
      uint8_t sqn[6], auc_vector_t* vector);

 public:
  /****** Rijndael ********/
  static void RijndaelKeySchedule(const uint8_t key[16]);
  static void RijndaelEncrypt(const uint8_t in[16], uint8_t out[16]);

 private:
  auc_vector_t auc_vector;
};

#endif
