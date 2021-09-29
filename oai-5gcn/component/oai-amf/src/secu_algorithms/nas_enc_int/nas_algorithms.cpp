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

/*! \file nas_algorithms.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "nas_algorithms.hpp"

#include "logger.hpp"

//------------------------------------------------------------------------------
uint64_t MUL64x(uint64_t V, uint64_t c) {
  if (V & 0x8000000000000000)
    return (V << 1) ^ c;
  else
    return V << 1;
}

//------------------------------------------------------------------------------
uint64_t MUL64xPOW(uint64_t V, uint32_t i, uint64_t c) {
  if (i == 0)
    return V;
  else
    return MUL64x(MUL64xPOW(V, i - 1, c), c);
}

//------------------------------------------------------------------------------
uint64_t MUL64(uint64_t V, uint64_t P, uint64_t c) {
  uint64_t result = 0;
  int i           = 0;
  for (i = 0; i < 64; i++) {
    if ((P >> i) & 0x1) result ^= MUL64xPOW(V, i, c);
  }
  return result;
}

//------------------------------------------------------------------------------
uint32_t mask32bit(int n) {
  uint32_t mask = 0x0;
  if (n % 32 == 0) return 0xffffffff;
  while (n--) mask = (mask >> 1) ^ 0x80000000;
  return mask;
}

//------------------------------------------------------------------------------
int nas_algorithms::nas_stream_encrypt_nea1(
    nas_stream_cipher_t* const stream_cipher, uint8_t* const out) {
  snow_3g_context_t snow_3g_context;
  uint32_t* KS;
  uint32_t K[4], IV[4];
  int n             = (stream_cipher->blength + 31) / 32;
  uint32_t zero_bit = stream_cipher->blength & 0x7;
  memset(&snow_3g_context, 0, sizeof(snow_3g_context));
  memcpy(K + 3, stream_cipher->key + 0, 4);
  memcpy(K + 2, stream_cipher->key + 4, 4);
  memcpy(K + 1, stream_cipher->key + 8, 4);
  memcpy(K + 0, stream_cipher->key + 12, 4);
  K[3]  = hton_int32(K[3]);
  K[2]  = hton_int32(K[2]);
  K[1]  = hton_int32(K[1]);
  K[0]  = hton_int32(K[0]);
  IV[3] = stream_cipher->count;
  IV[2] = ((((uint32_t) stream_cipher->bearer) << 3) |
           ((((uint32_t) stream_cipher->direction) & 0x1) << 2))
          << 24;
  IV[1] = IV[3];
  IV[0] = IV[2];
  snow3g_initialize(K, IV, &snow_3g_context);
  KS = (uint32_t*) malloc(4 * n);
  snow3g_generate_key_stream(n, (uint32_t*) KS, &snow_3g_context);
  if (zero_bit > 0) {
    KS[n - 1] = KS[n - 1] & (uint32_t)(0xFFFFFFFF << (8 - zero_bit));
  }
  for (int i = 0; i < n; i++) {
    KS[i] = hton_int32(KS[i]);
  }
  for (int i = 0; i < n * 4; i++) {
    stream_cipher->message[i] ^= *(((uint8_t*) KS) + i);
  }
  int ceil_index = 0;
  if (zero_bit > 0) {
    ceil_index = (stream_cipher->blength + 7) >> 3;
    stream_cipher->message[ceil_index - 1] =
        stream_cipher->message[ceil_index - 1] &
        (uint8_t)(0xFF << (8 - zero_bit));
  }
  free(KS);
  KS = NULL;
  memcpy(out, stream_cipher->message, n * 4);
  if (zero_bit > 0) {
    out[ceil_index - 1] = stream_cipher->message[ceil_index - 1];
  }
  return 0;
}

//------------------------------------------------------------------------------
int nas_algorithms::nas_stream_encrypt_nia1(
    nas_stream_cipher_t* const stream_cipher, uint8_t const out[4]) {
  snow_3g_context_t snow_3g_context;
  uint32_t K[4], IV[4], z[5];
  uint32_t MAC_I = 0;
  uint64_t EVAL;
  uint64_t V;
  uint64_t P;
  uint64_t Q;
  uint64_t c;
  uint64_t M_D_2;
  int rem_bits, D;
  uint32_t mask = 0;
  uint32_t* message;

  message = (uint32_t*) stream_cipher->message;
  memcpy(K + 3, stream_cipher->key + 0, 4);
  memcpy(K + 2, stream_cipher->key + 4, 4);
  memcpy(K + 1, stream_cipher->key + 8, 4);
  memcpy(K + 0, stream_cipher->key + 12, 4);
  K[3]  = hton_int32(K[3]);
  K[2]  = hton_int32(K[2]);
  K[1]  = hton_int32(K[1]);
  K[0]  = hton_int32(K[0]);
  IV[3] = (uint32_t) stream_cipher->count;
  IV[2] = ((((uint32_t) stream_cipher->bearer) & 0x0000001F) << 27);
  IV[1] = (uint32_t)(stream_cipher->count) ^
          ((uint32_t)(stream_cipher->direction) << 31);
  IV[0] = ((((uint32_t) stream_cipher->bearer) & 0x0000001F) << 27) ^
          ((uint32_t)(stream_cipher->direction & 0x00000001) << 15);
  z[0] = z[1] = z[2] = z[3] = z[4] = 0;
  snow3g_initialize(K, IV, &snow_3g_context);
  snow3g_generate_key_stream(5, z, &snow_3g_context);
  P    = ((uint64_t) z[0] << 32) | (uint64_t) z[1];
  Q    = ((uint64_t) z[2] << 32) | (uint64_t) z[3];
  D    = ceil(stream_cipher->blength / 64.0) + 1;
  EVAL = 0;
  c    = 0x1b;
  for (int i = 0; i < D - 2; i++) {
    V    = EVAL ^ ((uint64_t) hton_int32(message[2 * i]) << 32 |
                (uint64_t) hton_int32(message[2 * i + 1]));
    EVAL = MUL64(V, P, c);
  }
  rem_bits = stream_cipher->blength % 64;
  if (rem_bits == 0) rem_bits = 64;
  mask = mask32bit(rem_bits % 32);
  if (rem_bits > 32) {
    M_D_2 = ((uint64_t) hton_int32(message[2 * (D - 2)]) << 32) |
            (uint64_t)(hton_int32(message[2 * (D - 2) + 1]) & mask);
  } else {
    M_D_2 = ((uint64_t) hton_int32(message[2 * (D - 2)]) & mask) << 32;
  }
  V    = EVAL ^ M_D_2;
  EVAL = MUL64(V, P, c);
  EVAL ^= stream_cipher->blength;
  EVAL  = MUL64(EVAL, Q, c);
  MAC_I = (uint32_t)(EVAL >> 32) ^ z[4];
  MAC_I = hton_int32(MAC_I);
  memcpy((void*) out, &MAC_I, 4);
  return 0;
}

//------------------------------------------------------------------------------
int nas_algorithms::nas_stream_encrypt_nea2(
    nas_stream_cipher_t* const stream_cipher, uint8_t* const out) {
  uint8_t m[16];
  uint32_t local_count;
  void* ctx;
  uint8_t* data;
  uint32_t zero_bit = 0;
  uint32_t byte_length;

  DevAssert(stream_cipher != NULL);
  DevAssert(out != NULL);
  zero_bit    = stream_cipher->blength & 0x7;
  byte_length = stream_cipher->blength >> 3;

  if (zero_bit > 0) byte_length += 1;

  ctx         = malloc(nettle_aes128.context_size);
  data        = (uint8_t*) malloc(byte_length);
  local_count = hton_int32(stream_cipher->count);
  memset(m, 0, sizeof(m));
  memcpy(&m[0], &local_count, 4);
  m[4] = ((stream_cipher->bearer & 0x1F) << 3) |
         ((stream_cipher->direction & 0x01) << 2);
  /*
   * Other bits are 0
   */
#if NETTLE_VERSION_MAJOR < 3
  nettle_aes128.set_encrypt_key(
      ctx, stream_cipher->key_length, stream_cipher->key);
#else
  nettle_aes128.set_encrypt_key(ctx, stream_cipher->key);
#endif
  nettle_ctr_crypt(
      ctx, nettle_aes128.encrypt, nettle_aes128.block_size, m, byte_length,
      data, stream_cipher->message);

  if (zero_bit > 0)
    data[byte_length - 1] =
        data[byte_length - 1] & (uint8_t)(0xFF << (8 - zero_bit));

  memcpy(out, data, byte_length);
  free(data);
  free(ctx);
  return 0;
}

//------------------------------------------------------------------------------
int nas_algorithms::nas_stream_encrypt_nia2(
    nas_stream_cipher_t* const stream_cipher, uint8_t const out[4]) {
  uint8_t* m               = NULL;
  uint32_t local_count     = 0;
  size_t size              = 4;
  uint8_t data[16]         = {0};
  CMAC_CTX* cmac_ctx       = NULL;
  const EVP_CIPHER* cipher = EVP_aes_128_cbc();
  uint32_t zero_bit        = 0;
  uint32_t m_length;

  DevAssert(stream_cipher != NULL);
  DevAssert(stream_cipher->key != NULL);
  DevAssert(stream_cipher->key_length > 0);
  DevAssert(out != NULL);
  zero_bit = stream_cipher->blength & 0x7;
  m_length = stream_cipher->blength >> 3;

  if (zero_bit > 0) m_length += 1;

  local_count = hton_int32(stream_cipher->count);
  m           = (uint8_t*) calloc(m_length + 8, sizeof(uint8_t));
  memcpy(&m[0], &local_count, 4);
  m[4] = ((stream_cipher->bearer & 0x1F) << 3) |
         ((stream_cipher->direction & 0x01) << 2);
  memcpy(&m[8], stream_cipher->message, m_length);

  Logger::amf_n1().trace(
      "Byte length: %u, Zero bits: %u:", m_length + 8, zero_bit);
  Logger::amf_n1().trace("m:0x%x", *m);
  Logger::amf_n1().trace("Key:0x%x", *stream_cipher->key);
  Logger::amf_n1().trace("Message:0x%x", *stream_cipher->message);

  cmac_ctx = CMAC_CTX_new();
  CMAC_Init(
      cmac_ctx, stream_cipher->key, stream_cipher->key_length, cipher, NULL);
  CMAC_Update(cmac_ctx, m, m_length + 8);
  CMAC_Final(cmac_ctx, data, &size);
  CMAC_CTX_free(cmac_ctx);
  Logger::amf_n1().trace("Out:0x%x", *data);
  memcpy((void*) out, data, 4);
  free(m);
  return 0;
}
