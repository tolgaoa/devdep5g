/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

/*! \file nas_message.c
 \brief
 \author
 \company EURECOM
 \date 2019
 \email:
 */

#include "common_types.h"
#include "nas_message.h"
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "mmData.h"
#include "secu_defs.h"
#include "dynamic_memory_check.h"

#define SR_MAC_SIZE_BYTES 2

/* Functions used to decode layer 3 NAS messages */
static int _nas_message_header_decode(
    const unsigned char* const buffer,
    nas_message_security_header_t* const header, const size_t length,
    nas_message_decode_status_t* const status, bool* const is_sr);

static int _nas_message_plain_decode(
    const unsigned char* buffer, const nas_message_security_header_t* header,
    nas_message_plain_t* msg, size_t length);

static int _nas_message_protected_decode(
    unsigned char* const buffer, nas_message_security_header_t* header,
    nas_message_plain_t* msg, size_t length,
    fivegmm_security_context_t* const fivegmm_security_context,
    nas_message_decode_status_t* status);

/* Functions used to encode layer 3 NAS messages */

/*
 * Encode header of a security protected NAS message
 * @param [unsigned char* ] buffer: Pointer to the encoded data buffer
 * @param [const nas_message_security_header_t*] header: Security header
 * structure to encode
 * @param [size_t] length: Maximal capacity of the output buffer
 * @return The number of bytes in the buffer if the data have been successfully
 * encoded. 1, if the header is not a security header (header of plain NAS
 * message); encoded. A negative error code otherwise. -1 otherwise.
 */
static int _nas_message_header_encode(
    unsigned char* buffer, const nas_message_security_header_t* header,
    size_t length);

/*
 *  Encode plain NAS message
 * @param [unsigned char* ] buffer: Pointer to the encoded data buffer
 * @param [const nas_message_security_header_t* ] header: Plain NAS message
 * structure to encode
 * @param [size_t] length: Maximal capacity of the output buffer
 * @return The number of bytes in the buffer if the data have been successfully
 * encoded; A negative error code otherwise.
 */
static int _nas_message_plain_encode(
    unsigned char* buffer, const nas_message_security_header_t* header,
    const nas_message_plain_t* msg, size_t length);

/*
 *  Encode security protected NAS message
 * @param [unsigned char* ] buffer: Pointer to the encoded data buffer
 * @param [const nas_message_security_protected_t* ] msg: Security protected NAS
 * message structure to encode
 * @param [size_t] length: Maximal capacity of the output buffer
 * @param [void*] security:
 * @return The number of bytes in the buffer if the data have been successfully
 * encoded; A negative error code otherwise.
 */
static int _nas_message_protected_encode(
    unsigned char* buffer, const nas_message_security_protected_t* msg,
    size_t length, void* security);

/* Functions used to decrypt and encrypt layer 3 NAS messages */
static int _nas_message_decrypt(
    unsigned char* const dest, unsigned char* const src, uint8_t type,
    uint32_t code, uint8_t seq, size_t length,
    fivegmm_security_context_t* const fivegmm_security_context,
    nas_message_decode_status_t* status);

/*
 *  Encrypt plain NAS message
 * @param [unsigned char*] dest: Pointer to the encrypted data buffer
 * @param [const unsigned char*] src: Pointer to the decrypted data buffer
 * message structure to encode
 * @param [uint8_t ] type: Maximal capacity of the output bufferThe security
 * header type
 * @param [uint32_t] code: The message authentication code
 * @param [fivegmm_security_context_t* const ] fivegmm_security_context
 * @return The number of bytes in the output buffer if data have been
 * successfully encrypted; RETURNerror otherwise.
 */
static int _nas_message_encrypt(
    unsigned char* dest, const unsigned char* src, uint8_t type, uint32_t code,
    uint8_t seq, int const direction, size_t length,
    fivegmm_security_context_t* const fivegmm_security_context);

/* Functions used for integrity protection of layer 3 NAS messages */

/*
 *  Run integrity algorithm onto cyphered or uncyphered NAS message encoded in
 * the input buffer and return the computed message authentication code
 * @param [const unsigned char* const ] buffer: Pointer to the integrity
 * protected data buffer
 * @param [size_t const] length: Length of the input buffer
 * @param [void* ] security:
 * @return The message authentication code
 */
static uint32_t _nas_message_get_mac(
    const unsigned char* const buffer, size_t const length, int const direction,
    fivegmm_security_context_t* const fivegmm_security_context);

int nas_message_encode(
    unsigned char* buffer, const nas_message_t* const msg, size_t length,
    void* security) {
  fivegmm_security_context_t* fivegmm_security_context =
      (fivegmm_security_context_t*) security;
  int bytes;

  /*
   * Encode the header
   */
  int size = _nas_message_header_encode(buffer, &msg->header, length);
  if (size < 0) {
    return TLV_BUFFER_TOO_SHORT;
  } else if (size > 2) {
    /*
     * Encode security protected NAS message
     */
    bytes = _nas_message_protected_encode(
        buffer + size, &msg->security_protected, length - size,
        fivegmm_security_context);

    /*
     * Integrity protect the NAS message
     */
    if (bytes > 0) {
      /*
       * Compute offset of the sequence number field
       */
      int offset = size - sizeof(uint8_t);

      /*
       * Compute the NAS message authentication code
       */

      uint32_t mac = _nas_message_get_mac(
          buffer + offset, bytes + size - offset,
#if TEST_MAC_ENCRYPT_DECRYPT__
          DIRECTION__,
#else
          SECU_DIRECTION_DOWNLINK,
#endif
          fivegmm_security_context);
      /*
       * Set the message authentication code of the NAS message
       */

      *(uint32_t*) (buffer + 2 * sizeof(uint8_t)) = htonl(mac);

      if (fivegmm_security_context) {
#if TEST_MAC_ENCRYPT_DECRYPT__
#if !DIRECTION__
        fivegmm_security_context->ul_count.seq_num += 1;

        if (!fivegmm_security_context->ul_count.seq_num) {
          fivegmm_security_context->ul_count.overflow += 1;
        }
#else
        fivegmm_security_context->dl_count.seq_num += 1;

        if (!fivegmm_security_context->dl_count.seq_num) {
          fivegmm_security_context->dl_count.overflow += 1;
        }
#endif
#else
        fivegmm_security_context->dl_count.seq_num += 1;

        if (!fivegmm_security_context->dl_count.seq_num) {
          fivegmm_security_context->dl_count.overflow += 1;
        }
#endif
      } else {
        // OAILOG_DEBUG (LOG_NAS, "Did not increment
        // fivegmm_security_context.dl_count.seq_num because no security
        // context\n");
      }
    }
    /*
     * Log message header
     */

  } else {
    /*
     * Encode plain NAS message
     */
    bytes =
        _nas_message_plain_encode(buffer, &msg->header, &msg->plain, length);
  }

  if (bytes < 0) {
    return bytes;
  }
  //  if (size > 1) {
  //    return size + bytes;
  //  }

  return bytes;
}

int nas_message_decode(
    const unsigned char* const buffer, nas_message_t* msg, size_t length,
    void* security, nas_message_decode_status_t* status) {
  fivegmm_security_context_t* fivegmm_security_context =
      (fivegmm_security_context_t*) security;
  int bytes    = 0;
  uint32_t mac = 0;
  int size     = 0;
  bool is_sr   = false;
  /*
   * Decode the header
   */
  if (fivegmm_security_context) {
    status->security_context_available = 1;
  }
  size =
      _nas_message_header_decode(buffer, &msg->header, length, status, &is_sr);

  if (size < 0) {
    return TLV_BUFFER_TOO_SHORT;
  }
  if (size > 1) {
    // found security header
    /*
     * Compute offset of the sequence number field
     */
    int offset = size - sizeof(uint8_t);
    if (fivegmm_security_context) {
#if TEST_MAC_ENCRYPT_DECRYPT__
#if !DIRECTION__
      status->security_context_available = 1;
      if (fivegmm_security_context->ul_count.seq_num >
          msg->header.sequence_number) {
        fivegmm_security_context->ul_count.overflow += 1;
      }
      fivegmm_security_context->ul_count.seq_num = msg->header.sequence_number;
#else
      status->security_context_available = 1;
      if (fivegmm_security_context->dl_count.seq_num >
          msg->header.sequence_number) {
        fivegmm_security_context->dl_count.overflow += 1;
      }
      fivegmm_security_context->dl_count.seq_num = msg->header.sequence_number;
#endif
#else
      status->security_context_available = 1;
      if (fivegmm_security_context->ul_count.seq_num >
          msg->header.sequence_number) {
        fivegmm_security_context->ul_count.overflow += 1;
      }
      fivegmm_security_context->ul_count.seq_num = msg->header.sequence_number;
#endif
      /*
       * Compute the NAS message authentication code, return 0 if no security
       * context
       */
      mac = _nas_message_get_mac(
          buffer + offset, length - offset,
#if TEST_MAC_ENCRYPT_DECRYPT__
          DIRECTION__,
#else
          SECU_DIRECTION_UPLINK,
#endif
          fivegmm_security_context);
      /*
       * Check NAS message integrity
       */
      if (mac == msg->header.message_authentication_code) {
        status->mac_matched = 1;
      } else {
        // OAILOG_DEBUG (LOG_NAS, "msg->header.message_authentication_code =
        // %04X != computed = %04X\n", msg->header.message_authentication_code,
        // mac);
      }
    }

    /*
     * Decode security protected NAS message
     */
    bytes = _nas_message_protected_decode(
        (unsigned char* const)(buffer + size), &msg->header, &msg->plain,
        length - size, fivegmm_security_context, status);
  } else {
    /*
     * Decode plain NAS message
     */
    bytes =
        _nas_message_plain_decode(buffer, &msg->header, &msg->plain, length);
  }

  if (bytes < 0) {
    // TODO: commented for testing purpose  return bytes;
    // OAILOG_FUNC_RETURN (LOG_NAS, bytes);
  }

  // return bytes;
  return RETURNok;
}

static int _nas_message_header_encode(
    unsigned char* buffer, const nas_message_security_header_t* header,
    size_t length) {
  int size = 0;

  /*
   * Encode the first octet of the header (security header type or EPS bearer
   * identity, and protocol discriminator)
   */
  ENCODE_U8(buffer, header->extended_protocol_discriminator, size);

  // Security header type associated with a spare half octet;
  ENCODE_U8(buffer + size, *((uint8_t*) (header) + 1), size);
  // ENCODE_U8 (buffer+size,header->security_header_type,size);
#if DEBUG_IS_ON
  printf(
      "extended_protocol_discriminator %d, security_header_type %d \n",
      header->extended_protocol_discriminator, header->security_header_type);
#endif
  if (header->extended_protocol_discriminator ==
      EPD_5GS_MOBILITY_MANAGEMENT_MESSAGES) {
    if (header->security_header_type != SECURITY_HEADER_TYPE_NOT_PROTECTED) {
#if DEBUG_IS_ON
      printf("security_header_type != SECURITY_HEADER_TYPE_NOT_PROTECTED\n");
#endif
      if (length < NAS_MESSAGE_SECURITY_HEADER_SIZE) {
        /*
         * The buffer is not big enough to contain security header
         */
        return RETURNerror;
      }

      /*
       * Encode the message authentication code
       */
      ENCODE_U32(buffer + size, header->message_authentication_code, size);
      /*
       * Encode the sequence number
       */
      ENCODE_U8(buffer + size, header->sequence_number, size);
    }
#if DEBUG_IS_ON
    printf("security_header_type: SECURITY_HEADER_TYPE_NOT_PROTECTED\n");
#endif
  }
  return size;
}

static int _nas_message_protected_encode(
    unsigned char* buffer, const nas_message_security_protected_t* msg,
    size_t length, void* security) {
  fivegmm_security_context_t* fivegmm_security_context =
      (fivegmm_security_context_t*) security;
  int bytes                = TLV_BUFFER_TOO_SHORT;
  unsigned char* plain_msg = (unsigned char*) calloc(1, length);

  if (plain_msg) {
    /*
     * Encode the security protected NAS message as plain NAS message
     */
    int size =
        _nas_message_plain_encode(plain_msg, &msg->header, &msg->plain, length);

    if (size > 0) {
      /*
       * Encrypt the encoded plain NAS message
       */

      bytes = _nas_message_encrypt(
          buffer, plain_msg, msg->header.security_header_type,
          msg->header.message_authentication_code, msg->header.sequence_number,
#if TEST_MAC_ENCRYPT_DECRYPT__
          DIRECTION__,
#else
          SECU_DIRECTION_DOWNLINK,
#endif
          size, fivegmm_security_context);
    }

    free_wrapper((void**) &plain_msg);
  }
  return bytes;
}

static int _nas_message_plain_encode(
    unsigned char* buffer, const nas_message_security_header_t* header,
    const nas_message_plain_t* msg, size_t length) {
  int bytes = TLV_PROTOCOL_NOT_SUPPORTED;

  if (header->extended_protocol_discriminator ==
      EPD_5GS_MOBILITY_MANAGEMENT_MESSAGES) {
    /*
     * Encode EPS Mobility Management L3 message
     */
    bytes = fivegmm_msg_encode((MM_msg*) (&msg->mm), (uint8_t*) buffer, length);

  } else if (
      header->extended_protocol_discriminator ==
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES) {
    /*
     * Encode EPS Session Management L3 message
     */
    bytes = fivegsm_msg_encode((SM_msg*) (&msg->sm), (uint8_t*) buffer, length);
  } else {
    /*
     * Discard L3 messages with not supported protocol discriminator
     */
  }
  return bytes;
}

static int _nas_message_encrypt(
    unsigned char* dest, const unsigned char* src, uint8_t security_header_type,
    uint32_t code, uint8_t seq, int const direction, size_t length,
    fivegmm_security_context_t* const fivegmm_security_context) {
  nas_stream_cipher_t stream_cipher = {0};
  uint32_t count                    = 0;

  if (!fivegmm_security_context) {
    // OAILOG_ERROR(LOG_NAS, "No security context set for encryption protection
    // algorithm\n"); OAILOG_FUNC_RETURN (LOG_NAS, 0);
  }
  switch (security_header_type) {
    case SECURITY_HEADER_TYPE_NOT_PROTECTED:
    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED:
    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW:
      // OAILOG_DEBUG (LOG_NAS, "No encryption of message according to security
      // header type 0x%02x\n", security_header_type);
      memcpy(dest, src, length);
      break;
    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED:
    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED_NEW:
      switch (fivegmm_security_context->selected_algorithms.encryption) {
        case NAS_SECURITY_ALGORITHMS_NEA1: {
          if (direction == SECU_DIRECTION_UPLINK) {
            count = 0x00000000 |
                    ((fivegmm_security_context->ul_count.overflow && 0x0000FFFF)
                     << 8) |
                    (fivegmm_security_context->ul_count.seq_num & 0x000000FF);
          } else {
            count = 0x00000000 |
                    ((fivegmm_security_context->dl_count.overflow && 0x0000FFFF)
                     << 8) |
                    (fivegmm_security_context->dl_count.seq_num & 0x000000FF);
          }

          stream_cipher.key        = fivegmm_security_context->knas_enc;
          stream_cipher.key_length = AUTH_KNAS_ENC_SIZE;
          stream_cipher.count      = count;
          stream_cipher.bearer     = 0x00;  // 33.401 section 8.1.1
          stream_cipher.direction  = direction;
          stream_cipher.message    = (uint8_t*) src;
          /*
           * length in bits
           */
          stream_cipher.blength = length << 3;
          // If the test or use encrypt opens the next line,otherwise close
          // nas_stream_encrypt_nea1 (&stream_cipher, (uint8_t*)dest);
          // OAILOG_FUNC_RETURN (LOG_NAS, length);
          // Open the next line without testing or using encryption,otherwise
          // close

          memcpy(dest, src, length);
          return length;
        } break;
        case NAS_SECURITY_ALGORITHMS_NEA2: {
          if (direction == SECU_DIRECTION_UPLINK) {
            count = 0x00000000 |
                    ((fivegmm_security_context->ul_count.overflow && 0x0000FFFF)
                     << 8) |
                    (fivegmm_security_context->ul_count.seq_num & 0x000000FF);
          } else {
            count = 0x00000000 |
                    ((fivegmm_security_context->dl_count.overflow && 0x0000FFFF)
                     << 8) |
                    (fivegmm_security_context->dl_count.seq_num & 0x000000FF);
          }

          stream_cipher.key        = fivegmm_security_context->knas_enc;
          stream_cipher.key_length = AUTH_KNAS_ENC_SIZE;
          stream_cipher.count      = count;
          stream_cipher.bearer     = 0x00;
          stream_cipher.direction  = direction;
          stream_cipher.message    = (uint8_t*) src;
          /*
           * length in bits
           */
          stream_cipher.blength = length << 3;
          // nas_stream_encrypt_nea2 (&stream_cipher, (uint8_t*)dest);
          // OAILOG_FUNC_RETURN (LOG_NAS, length);
        } break;
        case NAS_SECURITY_ALGORITHMS_NEA0: {
          memcpy(dest, src, length);
          // OAILOG_FUNC_RETURN (LOG_NAS, length);
        } break;
        default: {
          // OAILOG_ERROR(LOG_NAS, "Unknown Cyphering protection algorithm
          // %d\n", fivegmm_security_context->selected_algorithms.encryption);
        } break;
      }
      break;
    default: {
      //     OAILOG_ERROR(LOG_NAS, "Unknown security header type %u\n",
      //     security_header_type);
      // OAILOG_FUNC_RETURN (LOG_NAS, 0);
    } break;
  }

  return length;
}

static uint32_t _nas_message_get_mac(
    const unsigned char* const buffer, size_t const length, int const direction,
    fivegmm_security_context_t* const fivegmm_security_context) {
  if (!fivegmm_security_context) {
    // OAILOG_DEBUG (LOG_NAS, "No security context set for integrity protection
    // algorithm\n");
    return 0;
  }

  switch (fivegmm_security_context->selected_algorithms.integrity) {
    case NAS_SECURITY_ALGORITHMS_NIA1: {
      uint8_t mac[4];
      nas_stream_cipher_t stream_cipher = {0};
      uint32_t count;
      uint32_t* mac32;

      if (direction == SECU_DIRECTION_UPLINK) {
        count =
            0x00000000 |
            ((fivegmm_security_context->ul_count.overflow & 0x0000FFFF) << 8) |
            (fivegmm_security_context->ul_count.seq_num & 0x000000FF);
      } else {
        count =
            0x00000000 |
            ((fivegmm_security_context->dl_count.overflow & 0x0000FFFF) << 8) |
            (fivegmm_security_context->dl_count.seq_num & 0x000000FF);
      }

      stream_cipher.key        = fivegmm_security_context->knas_int;
      stream_cipher.key_length = AUTH_KNAS_INT_SIZE;
      stream_cipher.count      = count;
      stream_cipher.bearer     = 0x00;  // 33.401 section 8.1.1
      stream_cipher.direction  = direction;
      stream_cipher.message    = (uint8_t*) buffer;
      /*
       * length in bits
       */
      stream_cipher.blength = length << 3;
      nas_stream_encrypt_nia1(&stream_cipher, mac);
      mac32 = (uint32_t*) &mac;
      return ntohl(*mac32);
    } break;
    case NAS_SECURITY_ALGORITHMS_NIA2: {
      uint8_t mac[4];
      nas_stream_cipher_t stream_cipher = {0};
      uint32_t count;
      uint32_t* mac32;

      if (direction == SECU_DIRECTION_UPLINK) {
        count =
            0x00000000 |
            ((fivegmm_security_context->ul_count.overflow & 0x0000FFFF) << 8) |
            (fivegmm_security_context->ul_count.seq_num & 0x000000FF);
      } else {
        count =
            0x00000000 |
            ((fivegmm_security_context->dl_count.overflow & 0x0000FFFF) << 8) |
            (fivegmm_security_context->dl_count.seq_num & 0x000000FF);
      }

      stream_cipher.key        = fivegmm_security_context->knas_int;
      stream_cipher.key_length = AUTH_KNAS_INT_SIZE;
      stream_cipher.count      = count;
      stream_cipher.bearer     = 0x00;
      stream_cipher.direction  = direction;
      stream_cipher.message    = (uint8_t*) buffer;
      /*
       * length in bits
       */
      stream_cipher.blength = length << 3;
      // nas_stream_encrypt_nia2 (&stream_cipher, mac);
      mac32 = (uint32_t*) &mac;
      // OAILOG_FUNC_RETURN (LOG_NAS, ntohl (*mac32));
    } break;
    case NAS_SECURITY_ALGORITHMS_NIA0: {
      // OAILOG_FUNC_RETURN (LOG_NAS, 0);
    } break;
    default:
      // OAILOG_ERROR(LOG_NAS, "Unknown integrity protection algorithm %d\n",
      // fivegmm_security_context->selected_algorithms.integrity);
      break;
  }
  return 0;
}

static int _nas_message_header_decode(
    const unsigned char* const buffer,
    nas_message_security_header_t* const header, const size_t length,
    nas_message_decode_status_t* const status, bool* const is_sr) {
  int size = 0;

  /*
   * Decode the first octet of the header
   */
  DECODE_U8(buffer, header->extended_protocol_discriminator, size);

  *is_sr = false;
  if (header->extended_protocol_discriminator ==
      EPD_5GS_MOBILITY_MANAGEMENT_MESSAGES) {
    DECODE_U8(buffer + size, header->security_header_type, size);
    //    header->extended_protocol_discriminator ==
    //    EPD_5GS_SESSION_MANAGEMENT_MESSAGES) {
    if (header->security_header_type != SECURITY_HEADER_TYPE_NOT_PROTECTED) {
      if (status) {
        switch (header->security_header_type) {
          case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED:
          case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW:
            status->integrity_protected_message = 1;
            break;
          case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED:
          case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED_NEW:
            status->integrity_protected_message = 1;
            status->ciphered_message            = 1;
            break;
            /*
             case SECURITY_HEADER_TYPE_SERVICE_REQUEST:
             *is_sr = true;
             status->integrity_protected_message = 1;
             OAILOG_FUNC_RETURN (LOG_NAS, size);
             break;
             */
          default:;
        }
      }
      if (*is_sr == false) {
        if (length < NAS_MESSAGE_SECURITY_HEADER_SIZE) {
          /*
           * The buffer is not big enough to contain security header
           */
          return RETURNerror;
        }
        // Decode the message authentication code
        DECODE_U32(buffer + size, header->message_authentication_code, size);
        // Decode the sequence number
        DECODE_U8(buffer + size, header->sequence_number, size);
      }
    }
  }
  return size;
}

static int _nas_message_protected_decode(
    unsigned char* const buffer, nas_message_security_header_t* header,
    nas_message_plain_t* msg, size_t length,
    fivegmm_security_context_t* const fivegmm_security_context,
    nas_message_decode_status_t* const status) {
  int bytes                      = TLV_BUFFER_TOO_SHORT;
  unsigned char* const plain_msg = (unsigned char*) calloc(1, length);

  if (plain_msg) {
    /*
     * Decrypt the security protected NAS message
     */
    _nas_message_decrypt(
        plain_msg, buffer, header->security_header_type,
        header->message_authentication_code, header->sequence_number, length,
        fivegmm_security_context, status);
    /*
     * Decode the decrypted message as plain NAS message
     */
    bytes = _nas_message_plain_decode(plain_msg, header, msg, length);
    free_wrapper((void**) &plain_msg);
  }
  return bytes;
}

static int _nas_message_decrypt(
    unsigned char* const dest, unsigned char* const src,
    uint8_t security_header_type, uint32_t code, uint8_t seq, size_t length,
    fivegmm_security_context_t* const fivegmm_security_context,
    nas_message_decode_status_t* status) {
  nas_stream_cipher_t stream_cipher = {0};
  uint32_t count                    = 0;
  uint8_t direction                 = 0;

  int size                             = 0;
  nas_message_security_header_t header = {0};
#if TEST_MAC_ENCRYPT_DECRYPT__
  direction = DIRECTION__;
#else
  direction = SECU_DIRECTION_UPLINK;
#endif

  switch (security_header_type) {
    case SECURITY_HEADER_TYPE_NOT_PROTECTED:
    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED:
    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW:
      // OAILOG_DEBUG (LOG_NAS, "No decryption of message length %lu according
      // to security header type 0x%02x\n", length, security_header_type);
      memcpy(dest, src, length);
      break;

    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED:
    case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED_NEW:
      if (fivegmm_security_context) {
        switch (fivegmm_security_context->selected_algorithms.encryption) {
          case NAS_SECURITY_ALGORITHMS_NEA1: {
            if (0 == status->mac_matched) {
              // OAILOG_ERROR(LOG_NAS, "MAC integrity failed\n");
              // OAILOG_FUNC_RETURN (LOG_NAS, 0);
            }
            if (direction == SECU_DIRECTION_UPLINK) {
              count =
                  0x00000000 |
                  ((fivegmm_security_context->ul_count.overflow && 0x0000FFFF)
                   << 8) |
                  (fivegmm_security_context->ul_count.seq_num & 0x000000FF);
            } else {
              count =
                  0x00000000 |
                  ((fivegmm_security_context->dl_count.overflow && 0x0000FFFF)
                   << 8) |
                  (fivegmm_security_context->dl_count.seq_num & 0x000000FF);
            }

            stream_cipher.key        = fivegmm_security_context->knas_enc;
            stream_cipher.key_length = AUTH_KNAS_ENC_SIZE;
            stream_cipher.count      = count;
            stream_cipher.bearer     = 0x00;
            stream_cipher.direction  = direction;
            // stream_cipher.message = (uint8_t*)src;
            uint8_t* src_encrypt = NULL;
            src_encrypt          = (uint8_t*) calloc(1, length + 3);
            memset(src_encrypt, 0, sizeof(uint8_t));
            memcpy(src_encrypt, src, length);
            stream_cipher.message = (uint8_t*) src_encrypt;

            /*
             * length in bits
             */
            stream_cipher.blength = length << 3;
            memcpy(dest, src, length);
            /*
             * Decode the first octet (security header type or EPS bearer
             * identity,
             * * * * and protocol discriminator)
             */
            // DECODE_U8 (dest, *(uint8_t *) (&header), size);
            // DECODE_U8(dest,header.extended_protocol_discriminator,size);
            // DECODE_U8(dest+size,header.security_header_type,size);
            // return header.extended_protocol_discriminator;
            // OAILOG_FUNC_RETURN (LOG_NAS,
            // header.extended_protocol_discriminator);
          } break;
          case NAS_SECURITY_ALGORITHMS_NEA2: {
            if (0 == status->mac_matched) {
              // OAILOG_ERROR(LOG_NAS, "MAC integrity failed\n");
              // OAILOG_FUNC_RETURN (LOG_NAS, 0);
            }
            if (direction == SECU_DIRECTION_UPLINK) {
              count =
                  0x00000000 |
                  ((fivegmm_security_context->ul_count.overflow && 0x0000FFFF)
                   << 8) |
                  (fivegmm_security_context->ul_count.seq_num & 0x000000FF);
            } else {
              count =
                  0x00000000 |
                  ((fivegmm_security_context->dl_count.overflow && 0x0000FFFF)
                   << 8) |
                  (fivegmm_security_context->dl_count.seq_num & 0x000000FF);
            }

            stream_cipher.key        = fivegmm_security_context->knas_enc;
            stream_cipher.key_length = AUTH_KNAS_ENC_SIZE;
            stream_cipher.count      = count;
            stream_cipher.bearer     = 0x00;
            stream_cipher.direction  = direction;
            stream_cipher.message    = (uint8_t*) src;
            /*
             * length in bits
             */
            stream_cipher.blength = length << 3;
            // nas_stream_encrypt_nea2 (&stream_cipher, (uint8_t*)dest);
            /*
             * Decode the first octet (security header type or EPS bearer
             * identity, and protocol discriminator)
             */
            DECODE_U8(dest, header.extended_protocol_discriminator, size);
            DECODE_U8(dest + size, header.security_header_type, size);
            // OAILOG_FUNC_RETURN (LOG_NAS,
            // header.extended_protocol_discriminator);
          } break;
          case NAS_SECURITY_ALGORITHMS_NEA0:
            // OAILOG_DEBUG (LOG_NAS, "NAS_SECURITY_ALGORITHMS_EEA0 dir %d
            // ul_count.seq_num %d dl_count.seq_num %d\n", direction,
            // fivegmm_security_context->ul_count.seq_num,
            // fivegmm_security_context->dl_count.seq_num);
            memcpy(dest, src, length);
            /*
             * Decode the first octet (security header type or EPS bearer
             * identity, and protocol discriminator)
             */
            DECODE_U8(dest, header.extended_protocol_discriminator, size);
            DECODE_U8(dest + size, header.security_header_type, size);
            // OAILOG_FUNC_RETURN (LOG_NAS,
            // header.extended_protocol_discriminator);
            break;

          default:
            // OAILOG_ERROR(LOG_NAS, "Unknown Cyphering protection algorithm
            // %d\n", fivegmm_security_context->selected_algorithms.encryption);
            memcpy(dest, src, length);
            /*
             * Decode the first octet (security header type or EPS bearer
             * identity,
             * * * * and protocol discriminator)
             */
            DECODE_U8(dest, header.extended_protocol_discriminator, size);
            DECODE_U8(dest + size, header.security_header_type, size);
            // OAILOG_FUNC_RETURN (LOG_NAS,
            // header.extended_protocol_discriminator);
            break;
        }
      } else {
        // OAILOG_ERROR(LOG_NAS, "No security context\n");
        // OAILOG_FUNC_RETURN (LOG_NAS, 0);
      }

      break;

    default: {
    }
      // OAILOG_ERROR(LOG_NAS, "Unknown security header type %u",
      // security_header_type); OAILOG_FUNC_RETURN (LOG_NAS, 0);
  };
  return 0;
}

static int _nas_message_plain_decode(
    const unsigned char* buffer, const nas_message_security_header_t* header,
    nas_message_plain_t* msg, size_t length) {
  int bytes = TLV_PROTOCOL_NOT_SUPPORTED;
  if (header->extended_protocol_discriminator ==
      EPD_5GS_MOBILITY_MANAGEMENT_MESSAGES) {
    /*
     * Decode 5G Mobility Management L3 message
     */
    bytes = mm_msg_decode(&msg->mm, (uint8_t*) buffer, length);
  } else if (
      header->extended_protocol_discriminator ==
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES) {
    /*
     * Decode 5G Session Management L3 message
     */
    bytes = sm_msg_decode(&msg->sm, (uint8_t*) buffer, length);
  }

  return bytes;
}
