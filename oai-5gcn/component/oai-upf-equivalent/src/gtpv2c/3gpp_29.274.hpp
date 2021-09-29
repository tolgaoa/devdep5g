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

/*! \file 3gpp_29.274.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_3GPP_29_274_HPP_SEEN
#define FILE_3GPP_29_274_HPP_SEEN

#include <arpa/inet.h>
#include <cstring>
#include <exception>
//#include <fmt/format.h> conflicts with spdlog
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "3gpp_29.274.h"
#include "logger.hpp"
#include "msg_gtpv2c.hpp"
#include "serializable.hpp"

namespace gtpv2c {

//------------------------------------------------------------------------------
class gtpv2c_tlv : public stream_serializable {
 public:
  static const uint16_t tlv_ie_length = 4;
  uint8_t type;
  uint16_t length;
  union {
    struct {
      uint8_t instance : 4;
      uint8_t spare : 4;
    } bf;
    uint8_t b;
  } u1;

  gtpv2c_tlv() : type(0), length(0), u1() {}

  gtpv2c_tlv(uint8_t t, uint16_t l = 0, uint8_t i = 0) : type(t), length(l) {
    u1.bf.instance = i;
    u1.bf.spare    = 0;
  }

  //~gtpv2c_tlv() {};

  void set_type(const uint8_t& t) { type = t; }

  uint8_t get_type() { return type; }

  void set_length(const uint16_t& l) { length = l; }

  uint16_t get_length() { return length; }

  void set_instance(const uint8_t& i) { u1.bf.instance = i; }

  uint8_t get_instance() { return u1.bf.instance; }

  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&type), sizeof(type));
    auto ns_length = htons(length);
    os.write(reinterpret_cast<const char*>(&ns_length), sizeof(ns_length));
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&type), sizeof(type));
    is.read(reinterpret_cast<char*>(&length), sizeof(length));
    length = ntohs(length);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
};
//------------------------------------------------------------------------------
class gtpv2c_ie : public stream_serializable {
 public:
  gtpv2c_tlv tlv;

  gtpv2c_ie() : tlv() {}
  explicit gtpv2c_ie(const gtpv2c_tlv& t) : tlv(t) {}
  explicit gtpv2c_ie(const uint8_t tlv_type) : tlv() { tlv.type = tlv_type; }

  virtual ~gtpv2c_ie(){};

  virtual void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    throw gtpc_msg_illegal_ie_exception(s.msg_id, tlv.type, __FILE__, __LINE__);
  }

  virtual void dump_to(std::ostream& os) { tlv.dump_to(os); };

  virtual void load_from(std::istream& is) {
    throw gtpc_ie_unimplemented_exception(
        tlv.type); /* should not hapen of course*/
  };

  static gtpv2c_ie* new_gtpv2c_ie_from_stream(std::istream& is);
};
//------------------------------------------------------------------------------
class gtpv2c_grouped_ie : public gtpv2c_ie {
 public:
  std::vector<std::shared_ptr<gtpv2c_ie>> ies;

  gtpv2c_grouped_ie() : gtpv2c_ie(), ies() {}
  gtpv2c_grouped_ie(const gtpv2c_grouped_ie& g) : gtpv2c_ie(g), ies(g.ies) {}

  explicit gtpv2c_grouped_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t), ies() {}
  explicit gtpv2c_grouped_ie(const uint8_t tlv_type)
      : gtpv2c_ie(tlv_type), ies() {}

  gtpv2c_grouped_ie& operator=(gtpv2c_grouped_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(ies, other.ies);
    return *this;
  }

  virtual ~gtpv2c_grouped_ie() { ies.clear(); }

  virtual void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    throw gtpc_msg_illegal_ie_exception(s.msg_id, tlv.type, __FILE__, __LINE__);
  }

  void add_ie(std::shared_ptr<gtpv2c_ie> ie) {
    ies.push_back(ie);
    tlv.length += (gtpv2c_tlv::tlv_ie_length + ie.get()->tlv.get_length());
  }

  virtual void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    for (auto sie : ies) {
      sie.get()->dump_to(os);
    }
  };

  virtual void load_from(std::istream& is) {
    // tlv.load_from(is);
    int32_t remaining_size = tlv.get_length();
    while (remaining_size > 0) {
      gtpv2c_ie* ie = new_gtpv2c_ie_from_stream(is);
      if (ie) {
        remaining_size -= (ie->tlv.get_length() + gtpv2c_tlv::tlv_ie_length);
        ies.push_back(std::shared_ptr<gtpv2c_ie>(ie));  // do not use add_ie()
      } else {
        throw gtpc_tlv_bad_length_exception(tlv.get_type(), tlv.get_length());
      }
    }
  };
};
//------------------------------------------------------------------------------
class gtpv2c_msg_header : public stream_serializable {
 private:
#define GTPV2C_MSG_HEADER_MIN_SIZE 8
  union {
    struct {
      uint8_t spare : 2;
      uint8_t mp : 1;
      uint8_t t : 1;
      uint8_t p : 1;
      uint8_t version : 3;
    } bf;
    uint8_t b;
  } u1;
  uint8_t message_type;
  uint16_t message_length;
  uint32_t teid;
  uint32_t sequence_number;
  union {
    struct {
      uint8_t spare : 4;
      uint8_t message_priority : 4;
    } bf;
    uint8_t b;
  } u2;

 public:
  gtpv2c_msg_header() {
    u1.b            = 0;
    message_type    = 0;
    message_length  = 3 + 1;  // sn + spare
    teid            = 0;
    sequence_number = 0;
    u2.b            = 0;
    u1.bf.version   = 2;
  }

  gtpv2c_msg_header(const gtpv2c_msg_header& h) {
    u1.b            = h.u1.b;
    message_type    = h.message_type;
    message_length  = h.message_length;
    teid            = h.teid;
    sequence_number = h.sequence_number;
    u2.b            = h.u2.b;
  }

  gtpv2c_msg_header& operator=(gtpv2c_msg_header other) {
    std::swap(u1, other.u1);
    std::swap(message_type, other.message_type);
    std::swap(message_length, other.message_length);
    std::swap(teid, other.teid);
    std::swap(sequence_number, other.sequence_number);
    std::swap(u2, other.u2);
    return *this;
  }

  void set_teid(const uint32_t& tid) {
    teid = tid;
    if (u1.bf.t == 0) {
      u1.bf.t = 1;
      message_length += 4;
    }
  }

  bool has_teid() const { return (u1.bf.t == 1); }

  uint32_t get_teid() const { return teid; }

  void set_message_type(const uint8_t& t) { message_type = t; }

  uint8_t get_message_type() const { return message_type; }

  void set_message_length(const uint16_t& l) { message_length = l; }

  uint16_t get_message_length() const { return message_length; }

  void set_sequence_number(const uint32_t& s) {
    sequence_number = s & 0x00FFFFFF;
  }

  uint32_t get_sequence_number() const { return sequence_number; }

  virtual void dump_to(std::ostream& os) {
    // ! Control Plane GTP header length shall be a multiple of 4 octets.
    u1.bf.spare = 0;
    u2.bf.spare = 0;
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(
        reinterpret_cast<const char*>(&message_type), sizeof(message_type));
    auto ns_message_length = htons(message_length);
    os.write(
        reinterpret_cast<const char*>(&ns_message_length),
        sizeof(ns_message_length));
    auto nl_teid = htonl(teid);
    if (u1.bf.t)
      os.write(reinterpret_cast<const char*>(&nl_teid), sizeof(nl_teid));
    else
      u1.bf.mp = 0;
    uint8_t sn[3];
    uint32_t cp_sequence_number = sequence_number;
    sn[2]                       = cp_sequence_number & 0x000000FF;
    cp_sequence_number          = cp_sequence_number >> 8;
    sn[1]                       = cp_sequence_number & 0x000000FF;
    cp_sequence_number          = cp_sequence_number >> 8;
    sn[0]                       = cp_sequence_number & 0x000000FF;

    os.write(reinterpret_cast<const char*>(sn), 3);
    if (!u1.bf.mp) u2.bf.message_priority = 0;
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
  }
  virtual void load_from(std::istream& is) {
    // ! Control Plane GTP header length shall be a multiple of 4 octets.
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&message_type), sizeof(message_type));
    is.read(reinterpret_cast<char*>(&message_length), sizeof(message_length));
    message_length = ntohs(message_length);
    if (u1.bf.t) {
      is.read(reinterpret_cast<char*>(&teid), sizeof(teid));
      teid = ntohl(teid);
    } else {
      teid = 0;
    }
    uint8_t sn[3];
    is.read(reinterpret_cast<char*>(sn), 3);
    sequence_number =
        (((uint32_t) sn[0]) << 16) | (((uint32_t) sn[1]) << 8) | sn[2];
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
  }
};
//------------------------------------------------------------------------------
class gtpv2c_msg : public gtpv2c_msg_header {
 public:
  uint16_t remote_port;

  std::vector<std::shared_ptr<gtpv2c_ie>> ies;

  gtpv2c_msg() : gtpv2c_msg_header(), remote_port(0), ies() {}

  gtpv2c_msg(const gtpv2c_msg& m)
      : gtpv2c_msg_header(m), remote_port(m.remote_port), ies(m.ies) {}

  gtpv2c_msg& operator=(gtpv2c_msg other) {
    std::swap(remote_port, other.remote_port);
    std::swap(ies, other.ies);
    return *this;
  }

  explicit gtpv2c_msg(const gtpv2c_msg_header& hdr)
      : gtpv2c_msg_header(hdr), remote_port(0), ies() {}

  explicit gtpv2c_msg(const gtpv2c_echo_request& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_echo_response& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_create_session_request& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_create_session_response& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_delete_session_request& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_delete_session_response& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_modify_bearer_request& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_modify_bearer_response& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_release_access_bearers_request& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_release_access_bearers_response& gtp_ies);
  explicit gtpv2c_msg(const gtpv2c_downlink_data_notification& gtp_ies);
  explicit gtpv2c_msg(
      const gtpv2c_downlink_data_notification_acknowledge& gtp_ies);

  ~gtpv2c_msg() { ies.clear(); }

  void add_ie(std::shared_ptr<gtpv2c_ie> ie) {
    ies.push_back(ie);
    // std::cout << std::dec<< " add_ie   = " << get_message_length() << " ->
    // "<< get_message_length() + gtpv2c_tlv::tlv_ie_length +
    // ie.get()->tlv.get_length() << std::endl;
    set_message_length(
        get_message_length() + gtpv2c_tlv::tlv_ie_length +
        ie.get()->tlv.get_length());
  }

  void to_core_type(gtpv2c_echo_request& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_echo_response& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_create_session_request& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_create_session_response& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_create_bearer_request& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_create_bearer_response& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_modify_bearer_request& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_modify_bearer_response& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_delete_session_request& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_delete_session_response& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_release_access_bearers_request& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_release_access_bearers_response& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_delete_bearer_command& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_delete_bearer_failure_indication& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_downlink_data_notification& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_downlink_data_notification_acknowledge& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }
  void to_core_type(gtpv2c_downlink_data_notification_failure_indication& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s, i.get()->tlv.u1.bf.instance);
    }
  }

  void dump_to(std::ostream& os) {
    gtpv2c_msg_header::dump_to(os);
    for (auto i : ies) {
      i.get()->dump_to(os);
    }
  }
  void load_from(std::istream& is) {
    gtpv2c_msg_header::load_from(is);

    uint16_t check_msg_length = get_message_length() - 3 - 1;  // sn + spare
    if (has_teid()) check_msg_length -= 4;
    gtpv2c_ie* ie       = nullptr;
    uint16_t ies_length = 0;
    // std::cout << std::dec<< " check_msg_length  = " << check_msg_length <<
    // std::endl;
    do {
      ie = gtpv2c_ie::new_gtpv2c_ie_from_stream(is);
      if (ie) {
        ies_length += (gtpv2c_tlv::tlv_ie_length + ie->tlv.get_length());
        ies.push_back(std::shared_ptr<gtpv2c_ie>(ie));
        // std::cout << std::dec << " ies length  = " << ies_length << " IE
        // length = " << ie->tlv.get_length() << std::endl;
      }
    } while ((ie) && (ies_length < check_msg_length));

    if (ies_length != check_msg_length) {
      // std::cout << " check_msg_length  = " << check_msg_length << "
      // ies_length  = " << ies_length << std::endl;
      throw gtpc_msg_bad_length_exception(
          get_message_type(), get_message_length());
    }
  }
};

inline void ipv6_address_dump_to(
    std::ostream& os, const struct in6_addr& ipv6_address) {
  os.write(reinterpret_cast<const char*>(ipv6_address.__in6_u.__u6_addr8), 16);
}

inline void ipv6_address_load_from(
    std::istream& is, struct in6_addr& ipv6_address) {
  is.read(reinterpret_cast<char*>(ipv6_address.__in6_u.__u6_addr8), 16);
}

inline void ipv4_address_dump_to(
    std::ostream& os, const struct in_addr& ipv4_address) {
  os.write(
      reinterpret_cast<const char*>(&ipv4_address.s_addr),
      sizeof(ipv4_address.s_addr));
}

inline void ipv4_address_load_from(
    std::istream& is, struct in_addr& ipv4_address) {
  is.read(
      reinterpret_cast<char*>(&ipv4_address.s_addr),
      sizeof(ipv4_address.s_addr));
}

//------------------------------------------------------------------------------
// 8.3 International Mobile Subscriber Identity (IMSI)

class gtpv2c_imsi_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
      uint8_t filler : 4;
    } digits; /*!< \brief  IMSI shall consist of decimal digits (0 through 9)
                 only.*/
    //#define IMSI_BCD8_SIZE                    8                /*!< \brief The
    // number of digits in IMSI shall not exceed 15.       */
    uint8_t b[IMSI_BCD8_SIZE];
  } u1;
  uint num_digits;

  //--------
  explicit gtpv2c_imsi_ie(const imsi_t& i) : gtpv2c_ie(GTP_IE_IMSI) {
    // avoid using b[]
    u1.digits.filler  = i.u1.digits.filler;
    u1.digits.digit1  = i.u1.digits.digit1;
    u1.digits.digit2  = i.u1.digits.digit2;
    u1.digits.digit3  = i.u1.digits.digit3;
    u1.digits.digit4  = i.u1.digits.digit4;
    u1.digits.digit5  = i.u1.digits.digit5;
    u1.digits.digit6  = i.u1.digits.digit6;
    u1.digits.digit7  = i.u1.digits.digit7;
    u1.digits.digit8  = i.u1.digits.digit8;
    u1.digits.digit9  = i.u1.digits.digit9;
    u1.digits.digit10 = i.u1.digits.digit10;
    u1.digits.digit11 = i.u1.digits.digit11;
    u1.digits.digit12 = i.u1.digits.digit12;
    u1.digits.digit13 = i.u1.digits.digit13;
    u1.digits.digit14 = i.u1.digits.digit14;
    u1.digits.digit15 = i.u1.digits.digit15;
    num_digits        = i.num_digits;
    tlv.set_length((num_digits + 1) / 2);
  };
  //--------
  gtpv2c_imsi_ie() : gtpv2c_ie(GTP_IE_IMSI) {
    memset(u1.b, 0, IMSI_BCD8_SIZE);
    num_digits = 0;
    tlv.set_length(0);
  };
  //--------
  explicit gtpv2c_imsi_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    std::memset(u1.b, 0, IMSI_BCD8_SIZE);
    num_digits = 0;
  };
  //--------
  void to_core_type(imsi_t& i) {
    i.u1.digits.digit1  = u1.digits.digit1;
    i.u1.digits.digit2  = u1.digits.digit2;
    i.u1.digits.digit3  = u1.digits.digit3;
    i.u1.digits.digit4  = u1.digits.digit4;
    i.u1.digits.digit5  = u1.digits.digit5;
    i.u1.digits.digit6  = u1.digits.digit6;
    i.u1.digits.digit7  = u1.digits.digit7;
    i.u1.digits.digit8  = u1.digits.digit8;
    i.u1.digits.digit9  = u1.digits.digit9;
    i.u1.digits.digit10 = u1.digits.digit10;
    i.u1.digits.digit11 = u1.digits.digit11;
    i.u1.digits.digit12 = u1.digits.digit12;
    i.u1.digits.digit13 = u1.digits.digit13;
    i.u1.digits.digit14 = u1.digits.digit14;
    i.u1.digits.digit15 = u1.digits.digit15;
    i.u1.digits.filler  = u1.digits.filler;
    i.num_digits        = num_digits;
  };

  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(u1.b), tlv.get_length());
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(u1.b), tlv.get_length());
    num_digits = tlv.get_length() * 2;
    if ((u1.b[tlv.get_length() - 1] & 0xF0) == 0xF0) {
      num_digits -= 1;
    }
    if (num_digits > 15) num_digits = 15;  // should not happen
  }

  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    imsi_t imsi = {};
    to_core_type(imsi);
    s.set(imsi, instance);
  }
};
//------------------------------------------------------------------------------
// 8.4 Cause
class gtpv2c_cause_ie : public gtpv2c_ie {
 public:
  uint8_t cause_value;
  union {
    struct {
      uint8_t cs : 1;
      uint8_t bce : 1;
      uint8_t pce : 1;
      uint8_t spare1 : 5;
    } bf;
    uint8_t b;
  } u1;
  uint8_t type_of_the_offending_ie;
  uint16_t length_of_the_offending_ie;
  union {
    struct {
      uint8_t instance : 4;
      uint8_t spare2 : 4;
    } bf;
    uint8_t b;
  } u2;

  //--------
  explicit gtpv2c_cause_ie(const cause_t& i) : gtpv2c_ie(GTP_IE_CAUSE) {
    cause_value                = i.cause_value;
    u1.bf.spare1               = 0;
    u1.bf.pce                  = i.pce;
    u1.bf.bce                  = i.bce;
    u1.bf.cs                   = i.cs;
    type_of_the_offending_ie   = i.offending_ie_type;
    length_of_the_offending_ie = i.offending_ie_length;
    u2.bf.spare2               = 0;
    u2.bf.instance             = i.offending_ie_instance;
    // if (type_of_the_offending_ie) { choose right test?
    if (MANDATORY_IE_INCORRECT == cause_value) {
      tlv.length = 6;
    } else {
      tlv.length = 2;
    }
  }
  //--------
  gtpv2c_cause_ie() : gtpv2c_ie(GTP_IE_CAUSE) {
    cause_value                = 0;
    u1.b                       = 0;
    type_of_the_offending_ie   = 0;
    length_of_the_offending_ie = 0;
    u2.b                       = 0;
    tlv.length                 = 2;
  };
  //--------
  explicit gtpv2c_cause_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    cause_value                = 0;
    u1.b                       = 0;
    type_of_the_offending_ie   = 0;
    length_of_the_offending_ie = 0;
    u2.b                       = 0;
  };
  void to_core_type(cause_t& c) {
    c.cause_value           = cause_value;
    c.pce                   = u1.bf.pce;
    c.bce                   = u1.bf.bce;
    c.cs                    = u1.bf.cs;
    c.offending_ie_type     = type_of_the_offending_ie;
    c.offending_ie_length   = length_of_the_offending_ie;
    c.offending_ie_instance = u2.bf.instance;
  }
  //--------
  void dump_to(std::ostream& os) {
    if (MANDATORY_IE_INCORRECT == cause_value) {
      tlv.length = 6;
    } else {
      tlv.length = 2;
    }
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&cause_value), sizeof(cause_value));
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    if (tlv.length == 6) {
      os.write(
          reinterpret_cast<const char*>(&type_of_the_offending_ie),
          sizeof(type_of_the_offending_ie));
      auto ns_length_of_the_offending_ie = htons(length_of_the_offending_ie);
      os.write(
          reinterpret_cast<const char*>(&ns_length_of_the_offending_ie),
          sizeof(ns_length_of_the_offending_ie));
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&cause_value), sizeof(cause_value));
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));

    if (6 == tlv.length) {
      is.read(
          reinterpret_cast<char*>(&type_of_the_offending_ie),
          sizeof(type_of_the_offending_ie));
      is.read(
          reinterpret_cast<char*>(&length_of_the_offending_ie),
          sizeof(length_of_the_offending_ie));
      length_of_the_offending_ie = ntohs(length_of_the_offending_ie);
      is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    }
  }

  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    cause_t cause = {};
    to_core_type(cause);
    s.set(cause, instance);
  }
};

//------------------------------------------------------------------------------
// 8.5 recovery
class gtpv2c_recovery_ie : public gtpv2c_ie {
 public:
  uint8_t recovery_restart_counter;
  //--------
  explicit gtpv2c_recovery_ie(const recovery_t& i)
      : gtpv2c_ie(GTP_IE_RECOVERY_RESTART_COUNTER) {
    recovery_restart_counter = i.restart_counter;
    tlv.length               = 1;
  }
  //--------
  gtpv2c_recovery_ie() : gtpv2c_ie(GTP_IE_RECOVERY_RESTART_COUNTER) {
    recovery_restart_counter = 0;
    tlv.length               = 1;
  };
  //--------
  explicit gtpv2c_recovery_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    recovery_restart_counter = 0;
  };

  void to_core_type(recovery_t& c) {
    c.restart_counter = recovery_restart_counter;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(
        reinterpret_cast<const char*>(&recovery_restart_counter),
        sizeof(recovery_restart_counter));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(
        reinterpret_cast<char*>(&recovery_restart_counter),
        sizeof(recovery_restart_counter));
  }

  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    recovery_t v = {};
    to_core_type(v);
    s.set(v, instance);
  }
};

//------------------------------------------------------------------------------
// 8.6 Access Point Name
class gtpv2c_access_point_name_ie : public gtpv2c_ie {
 public:
  std::string access_point_name;
  //--------
  explicit gtpv2c_access_point_name_ie(const apn_t& apn)
      : gtpv2c_ie(GTP_IE_ACCESS_POINT_NAME),
        access_point_name(apn.access_point_name) {
    tlv.length = access_point_name.size();
  };
  //--------
  gtpv2c_access_point_name_ie() : gtpv2c_ie(GTP_IE_ACCESS_POINT_NAME){};
  //--------
  explicit gtpv2c_access_point_name_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t){};
  //--------
  void to_core_type(apn_t& a) { a.access_point_name = access_point_name; }
  //--------
  void dump_to(std::ostream& os) {
    tlv.length = access_point_name.size();
    tlv.dump_to(os);
    os << access_point_name;
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    char apn[tlv.length];
    is.read(apn, tlv.length);
    access_point_name.assign(apn, tlv.length);
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    apn_t apn = {};
    to_core_type(apn);
    s.set(apn, instance);
  }
};

//------------------------------------------------------------------------------
// 8.7  Aggregate Maximum Bit Rate (AMBR)
class gtpv2c_aggregate_maximum_bit_rate_ie : public gtpv2c_ie {
 public:
  uint32_t apn_ambr_for_uplink;
  uint32_t apn_ambr_for_downlink;
  //--------
  explicit gtpv2c_aggregate_maximum_bit_rate_ie(const ambr_t& ambr)
      : gtpv2c_ie(GTP_IE_AGGREGATE_MAXIMUM_BIT_RATE) {
    // force length
    tlv.length            = 8;
    apn_ambr_for_uplink   = ambr.br_ul;
    apn_ambr_for_downlink = ambr.br_dl;
  };
  gtpv2c_aggregate_maximum_bit_rate_ie()
      : gtpv2c_ie(GTP_IE_AGGREGATE_MAXIMUM_BIT_RATE) {
    // force length
    tlv.length            = 8;
    apn_ambr_for_uplink   = 0;
    apn_ambr_for_downlink = 0;
  };
  //--------
  explicit gtpv2c_aggregate_maximum_bit_rate_ie(const gtpv2c_tlv& t)
      : gtpv2c_ie(t) {
    // force length
    apn_ambr_for_uplink   = 0;
    apn_ambr_for_downlink = 0;
  };
  //--------
  void to_core_type(ambr_t& ambr) {
    ambr.br_ul = apn_ambr_for_uplink;
    ambr.br_dl = apn_ambr_for_downlink;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    auto nl_apn_ambr_for_uplink   = htobe32(apn_ambr_for_uplink);
    auto nl_apn_ambr_for_downlink = htobe32(apn_ambr_for_downlink);
    os.write(
        reinterpret_cast<const char*>(&nl_apn_ambr_for_uplink),
        sizeof(nl_apn_ambr_for_uplink));
    os.write(
        reinterpret_cast<const char*>(&nl_apn_ambr_for_downlink),
        sizeof(nl_apn_ambr_for_downlink));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(
        reinterpret_cast<char*>(&apn_ambr_for_uplink),
        sizeof(apn_ambr_for_uplink));
    is.read(
        reinterpret_cast<char*>(&apn_ambr_for_downlink),
        sizeof(apn_ambr_for_downlink));
    apn_ambr_for_uplink   = be32toh(apn_ambr_for_uplink);
    apn_ambr_for_downlink = be32toh(apn_ambr_for_downlink);
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    ambr_t v = {};
    to_core_type(v);
    s.set(v, instance);
  }
};

//------------------------------------------------------------------------------
// 8.8 EPS Bearer ID (EBI)
class gtpv2c_eps_bearer_id_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t eps_bearer_id : 4;
      uint8_t spare : 4;
    } bf;
    uint8_t b;
  } u1;
  //--------
  explicit gtpv2c_eps_bearer_id_ie(const ebi_t& e)
      : gtpv2c_ie(GTP_IE_EPS_BEARER_ID) {
    tlv.length          = 1;
    u1.b                = 0;
    u1.bf.eps_bearer_id = e.ebi;
  }
  //--------
  gtpv2c_eps_bearer_id_ie() : gtpv2c_ie(GTP_IE_EPS_BEARER_ID) {
    tlv.length = 1;
    u1.b       = 0;
  }
  //--------
  explicit gtpv2c_eps_bearer_id_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
  };
  //--------
  void to_core_type(ebi_t& e) { e.ebi = u1.bf.eps_bearer_id; }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    ebi_t ebi = {};
    to_core_type(ebi);
    s.set(ebi, instance);
  }
};

//------------------------------------------------------------------------------
// 8.9 IP Address
class gtpv2c_ip_address_ie : public gtpv2c_ie {
 public:
  union {
    in_addr ipv4_address;  // network byte order
    in6_addr ipv6_address;
  } u1;
  bool is_ipv4;
  //--------
  explicit gtpv2c_ip_address_ie(const ip_address_t& i)
      : gtpv2c_ie(GTP_IE_IP_ADDRESS) {
    is_ipv4 = i.is_ipv4;
    if (is_ipv4) {
      u1.ipv6_address = in6addr_any;
      u1.ipv4_address = i.address.ipv4_address;
      tlv.length      = 4;
    } else {
      u1.ipv6_address = i.address.ipv6_address;
      tlv.length      = 16;
    }
  }
  //--------
  gtpv2c_ip_address_ie() : gtpv2c_ie(GTP_IE_IP_ADDRESS) {
    tlv.length      = 4;
    u1.ipv6_address = in6addr_any;
    is_ipv4         = true;
  }
  //--------
  explicit gtpv2c_ip_address_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.ipv6_address = in6addr_any;
    is_ipv4         = true;
  };
  //--------
  void to_core_type(ip_address_t& i) {
    i.is_ipv4 = is_ipv4;
    if (is_ipv4) {
      i.address.ipv6_address = in6addr_any;
      i.address.ipv4_address = u1.ipv4_address;
    } else {
      i.address.ipv6_address = u1.ipv6_address;
    }
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    if (is_ipv4)
      ipv4_address_dump_to(os, u1.ipv4_address);
    else
      ipv6_address_dump_to(os, u1.ipv6_address);
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.length == 4) {
      ipv4_address_load_from(is, u1.ipv4_address);
    } else if (tlv.length == 16) {
      ipv6_address_load_from(is, u1.ipv6_address);
    } else {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    ip_address_t ip_address = {};
    to_core_type(ip_address);
    s.set(ip_address, instance);
  }
};
//------------------------------------------------------------------------------
// 8.10 Mobile Equipment Identity (MEI)
class gtpv2c_mei_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
      uint8_t filler : 4;
    } digits; /*!< \brief  IMSI shall consist of decimal digits (0 through 9)
                 only.*/
#define IMEI_BCD8_SIZE                                                         \
  8 /*!< \brief  The number of digits in IMSI shall not exceed 15.       */
    uint8_t b[IMSI_BCD8_SIZE];
  } u1;
  uint num_digits;

  //--------
  explicit gtpv2c_mei_ie(const mei_t& i)
      : gtpv2c_ie(GTP_IE_MOBILE_EQUIPMENT_IDENTITY) {
    // avoid using b[]
    u1.digits.digit1  = i.u1.digits.digit1;
    u1.digits.digit2  = i.u1.digits.digit2;
    u1.digits.digit3  = i.u1.digits.digit3;
    u1.digits.digit4  = i.u1.digits.digit4;
    u1.digits.digit5  = i.u1.digits.digit5;
    u1.digits.digit6  = i.u1.digits.digit6;
    u1.digits.digit7  = i.u1.digits.digit7;
    u1.digits.digit8  = i.u1.digits.digit8;
    u1.digits.digit9  = i.u1.digits.digit9;
    u1.digits.digit10 = i.u1.digits.digit10;
    u1.digits.digit11 = i.u1.digits.digit11;
    u1.digits.digit12 = i.u1.digits.digit12;
    u1.digits.digit13 = i.u1.digits.digit13;
    u1.digits.digit14 = i.u1.digits.digit14;
    u1.digits.digit15 = i.u1.digits.digit15;
    u1.digits.filler  = i.u1.digits.filler;
    num_digits        = i.num_digits;
    uint num_bytes    = num_digits / 2;
    if (num_digits & 1) {
      u1.b[num_bytes] |= 0xF0;
      num_bytes++;
    }
    tlv.length = num_bytes;
  };
  //--------
  gtpv2c_mei_ie() : gtpv2c_ie(GTP_IE_MOBILE_EQUIPMENT_IDENTITY) {
    memset(u1.b, 0, IMEI_BCD8_SIZE);
    num_digits = 0;
  };
  //--------
  explicit gtpv2c_mei_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    std::memset(u1.b, 0, IMEI_BCD8_SIZE);
    num_digits = 0;
  };
  //--------
  void to_core_type(mei_t& i) {
    i.u1.digits.digit1  = u1.digits.digit1;
    i.u1.digits.digit2  = u1.digits.digit2;
    i.u1.digits.digit3  = u1.digits.digit3;
    i.u1.digits.digit4  = u1.digits.digit4;
    i.u1.digits.digit5  = u1.digits.digit5;
    i.u1.digits.digit6  = u1.digits.digit6;
    i.u1.digits.digit7  = u1.digits.digit7;
    i.u1.digits.digit8  = u1.digits.digit8;
    i.u1.digits.digit9  = u1.digits.digit9;
    i.u1.digits.digit10 = u1.digits.digit10;
    i.u1.digits.digit11 = u1.digits.digit11;
    i.u1.digits.digit12 = u1.digits.digit12;
    i.u1.digits.digit13 = u1.digits.digit13;
    i.u1.digits.digit14 = u1.digits.digit14;
    i.u1.digits.digit15 = u1.digits.digit15;
    i.u1.digits.filler  = u1.digits.filler;
    i.num_digits        = num_digits;
  };
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(u1.b), tlv.get_length());
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(u1.b), tlv.length);
    num_digits = tlv.get_length() * 2;
    if ((u1.b[tlv.get_length() - 1] & 0xF0) == 0xF0) {
      num_digits -= 1;
    }
    if (num_digits > 15) num_digits = 15;  // should not happen
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    mei_t mei = {};
    to_core_type(mei);
    s.set(mei, instance);
  }
};
//------------------------------------------------------------------------------
// 8.11 MSISDN
class gtpv2c_msisdn_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
    } digits; /*!< \brief  IMSI shall consist of decimal digits (0 through 9)
                 only.*/
#define MSISDN_BCD8_SIZE                                                       \
  8 /*!< \brief  The number of digits in IMSI shall not exceed 15.       */
    uint8_t b[MSISDN_BCD8_SIZE];
  } u1;
  uint num_digits;

  //--------
  explicit gtpv2c_msisdn_ie(const msisdn_t& m) : gtpv2c_ie(GTP_IE_MSISDN) {
    // avoid using b[]
    u1.digits.digit1  = m.u1.digits.digit1;
    u1.digits.digit2  = m.u1.digits.digit2;
    u1.digits.digit3  = m.u1.digits.digit3;
    u1.digits.digit4  = m.u1.digits.digit4;
    u1.digits.digit5  = m.u1.digits.digit5;
    u1.digits.digit6  = m.u1.digits.digit6;
    u1.digits.digit7  = m.u1.digits.digit7;
    u1.digits.digit8  = m.u1.digits.digit8;
    u1.digits.digit9  = m.u1.digits.digit9;
    u1.digits.digit10 = m.u1.digits.digit10;
    u1.digits.digit11 = m.u1.digits.digit11;
    u1.digits.digit12 = m.u1.digits.digit12;
    u1.digits.digit13 = m.u1.digits.digit13;
    u1.digits.digit14 = m.u1.digits.digit14;
    u1.digits.digit15 = m.u1.digits.digit15;
    num_digits        = m.num_digits;
    if (num_digits > 15) num_digits = 15;
    uint num_bytes = num_digits / 2;
    if (num_digits & 1) {
      num_bytes++;
    }
    tlv.length = num_bytes;
    num_digits = m.num_digits;
  };
  //--------
  gtpv2c_msisdn_ie() : gtpv2c_ie(GTP_IE_MSISDN) {
    memset(u1.b, 0, MSISDN_BCD8_SIZE);
    num_digits = 0;
  };
  //--------
  explicit gtpv2c_msisdn_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    std::memset(u1.b, 0, MSISDN_BCD8_SIZE);
    num_digits = 0;
  };
  //--------
  void to_core_type(msisdn_t& m) {
    m.u1.digits.digit1  = u1.digits.digit1;
    m.u1.digits.digit2  = u1.digits.digit2;
    m.u1.digits.digit3  = u1.digits.digit3;
    m.u1.digits.digit4  = u1.digits.digit4;
    m.u1.digits.digit5  = u1.digits.digit5;
    m.u1.digits.digit6  = u1.digits.digit6;
    m.u1.digits.digit7  = u1.digits.digit7;
    m.u1.digits.digit8  = u1.digits.digit8;
    m.u1.digits.digit9  = u1.digits.digit9;
    m.u1.digits.digit10 = u1.digits.digit10;
    m.u1.digits.digit11 = u1.digits.digit11;
    m.u1.digits.digit12 = u1.digits.digit12;
    m.u1.digits.digit13 = u1.digits.digit13;
    m.u1.digits.digit14 = u1.digits.digit14;
    m.u1.digits.digit15 = u1.digits.digit15;
    m.num_digits        = num_digits;
  };
  //--------
  void dump_to(std::ostream& os) {
    if (num_digits > 15) num_digits = 15;
    uint num_bytes = num_digits / 2;
    if (num_digits & 1) {
      u1.b[num_bytes] |= 0xF0;
      num_bytes++;
    }
    tlv.length = num_bytes;
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(u1.b), num_bytes);
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(u1.b), tlv.length);
    num_digits = tlv.get_length() * 2;
    if ((u1.b[tlv.get_length() - 1] & 0xF0) == 0xF0) {
      num_digits -= 1;
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    msisdn_t msisdn = {};
    to_core_type(msisdn);
    s.set(msisdn, instance);
  }
};
//------------------------------------------------------------------------------
// 8.12 Indication
class gtpv2c_indication_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t sgwci : 1;
      uint8_t israi : 1;
      uint8_t isrsi : 1;
      uint8_t oi : 1;
      uint8_t dfi : 1;
      uint8_t hi : 1;
      uint8_t dtf : 1;
      uint8_t daf : 1;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t msv : 1;
      uint8_t si : 1;
      uint8_t pt : 1;
      uint8_t p : 1;
      uint8_t crsi : 1;
      uint8_t cfsi : 1;
      uint8_t uimsi : 1;
      uint8_t sqci : 1;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t ccrsi : 1;
      uint8_t israu : 1;
      uint8_t mbmdt : 1;
      uint8_t s4af : 1;
      uint8_t s6af : 1;
      uint8_t srni : 1;
      uint8_t pbic : 1;
      uint8_t retloc : 1;
    } bf;
    uint8_t b;
  } u3;
  union {
    struct {
      uint8_t cpsr : 1;
      uint8_t clii : 1;
      uint8_t csfbi : 1;
      uint8_t ppsi : 1;
      uint8_t ppon : 1;
      uint8_t ppof : 1;
      uint8_t arrl : 1;
      uint8_t cprai : 1;
    } bf;
    uint8_t b;
  } u4;
  union {
    struct {
      uint8_t aopi : 1;
      uint8_t aosi : 1;
      uint8_t pcri : 1;
      uint8_t psci : 1;
      uint8_t bdwi : 1;
      uint8_t dtci : 1;
      uint8_t uasi : 1;
      uint8_t nsi : 1;
    } bf;
    uint8_t b;
  } u5;
  union {
    struct {
      uint8_t wpmsi : 1;
      uint8_t unaccsi : 1;
      uint8_t pnsi : 1;
      uint8_t s11tf : 1;
      uint8_t pmtsmi : 1;
      uint8_t cpopci : 1;
      uint8_t epcosi : 1;
      uint8_t roaai : 1;
    } bf;
    uint8_t b;
  } u6;
  union {
    struct {
      uint8_t tspcmi : 1;
      uint8_t enbcpi : 1;
      uint8_t ltempi : 1;
      uint8_t ltemui : 1;
      uint8_t eevrsi : 1;
      uint8_t spare3 : 1;
      uint8_t spare2 : 1;
      uint8_t spare1 : 1;
    } bf;
    uint8_t b;
  } u7;

  //--------
  explicit gtpv2c_indication_ie(const indication_t& i)
      : gtpv2c_ie(GTP_IE_INDICATION) {
    u1.bf.daf   = i.daf;
    u1.bf.dtf   = i.dtf;
    u1.bf.hi    = i.hi;
    u1.bf.dfi   = i.dfi;
    u1.bf.oi    = i.oi;
    u1.bf.isrsi = i.isrsi;
    u1.bf.israi = i.israi;
    u1.bf.sgwci = i.sgwci;

    u2.bf.sqci  = i.sqci;
    u2.bf.uimsi = i.uimsi;
    u2.bf.cfsi  = i.cfsi;
    u2.bf.crsi  = i.crsi;
    u2.bf.p     = i.p;
    u2.bf.pt    = i.pt;
    u2.bf.si    = i.si;
    u2.bf.msv   = i.msv;

    u3.bf.retloc = i.retloc;
    u3.bf.pbic   = i.pbic;
    u3.bf.srni   = i.srni;
    u3.bf.s6af   = i.s6af;
    u3.bf.s4af   = i.s4af;
    u3.bf.mbmdt  = i.mbmdt;
    u3.bf.israu  = i.israu;
    u3.bf.ccrsi  = i.ccrsi;

    u4.bf.cprai = i.cprai;
    u4.bf.arrl  = i.arrl;
    u4.bf.ppof  = i.ppof;
    u4.bf.ppon  = i.ppon;
    u4.bf.ppsi  = i.ppsi;
    u4.bf.csfbi = i.csfbi;
    u4.bf.clii  = i.clii;
    u4.bf.cpsr  = i.cpsr;

    u5.bf.nsi  = i.nsi;
    u5.bf.uasi = i.uasi;
    u5.bf.dtci = i.dtci;
    u5.bf.bdwi = i.bdwi;
    u5.bf.psci = i.psci;
    u5.bf.pcri = i.pcri;
    u5.bf.aosi = i.aosi;
    u5.bf.aopi = i.aopi;

    u6.bf.roaai   = i.roaai;
    u6.bf.epcosi  = i.epcosi;
    u6.bf.cpopci  = i.cpopci;
    u6.bf.pmtsmi  = i.pmtsmi;
    u6.bf.s11tf   = i.s11tf;
    u6.bf.pnsi    = i.pnsi;
    u6.bf.unaccsi = i.unaccsi;
    u6.bf.wpmsi   = i.wpmsi;

    u7.bf.spare1 = 0;
    u7.bf.spare2 = 0;
    u7.bf.spare3 = 0;
    u7.bf.eevrsi = i.eevrsi;
    u7.bf.ltemui = i.ltemui;
    u7.bf.ltempi = i.ltempi;
    u7.bf.enbcpi = i.enbcpi;
    u7.bf.tspcmi = i.tspcmi;
    tlv.length   = 7;
  };
  //--------
  gtpv2c_indication_ie() : gtpv2c_ie(GTP_IE_INDICATION) {
    u1.b       = 0;
    u2.b       = 0;
    u3.b       = 0;
    u4.b       = 0;
    u5.b       = 0;
    u6.b       = 0;
    u7.b       = 0;
    tlv.length = 7;
  };
  //--------
  explicit gtpv2c_indication_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
    u2.b = 0;
    u3.b = 0;
    u4.b = 0;
    u5.b = 0;
    u6.b = 0;
    u7.b = 0;
  };
  //--------
  void to_core_type(indication_t& i) {
    i.daf   = u1.bf.daf;
    i.dtf   = u1.bf.dtf;
    i.hi    = u1.bf.hi;
    i.dfi   = u1.bf.dfi;
    i.oi    = u1.bf.oi;
    i.isrsi = u1.bf.isrsi;
    i.israi = u1.bf.israi;
    i.sgwci = u1.bf.sgwci;

    i.sqci  = u2.bf.sqci;
    i.uimsi = u2.bf.uimsi;
    i.cfsi  = u2.bf.cfsi;
    i.crsi  = u2.bf.crsi;
    i.p     = u2.bf.p;
    i.pt    = u2.bf.pt;
    i.si    = u2.bf.si;
    i.msv   = u2.bf.msv;

    i.retloc = u3.bf.retloc;
    i.pbic   = u3.bf.pbic;
    i.srni   = u3.bf.srni;
    i.s6af   = u3.bf.s6af;
    i.s4af   = u3.bf.s4af;
    i.mbmdt  = u3.bf.mbmdt;
    i.israu  = u3.bf.israu;
    i.ccrsi  = u3.bf.ccrsi;

    i.cprai = u4.bf.cprai;
    i.arrl  = u4.bf.arrl;
    i.ppof  = u4.bf.ppof;
    i.ppon  = u4.bf.ppon;
    i.ppsi  = u4.bf.ppsi;
    i.csfbi = u4.bf.csfbi;
    i.clii  = u4.bf.clii;
    i.cpsr  = u4.bf.cpsr;

    i.nsi  = u5.bf.nsi;
    i.uasi = u5.bf.uasi;
    i.dtci = u5.bf.dtci;
    i.bdwi = u5.bf.bdwi;
    i.psci = u5.bf.psci;
    i.pcri = u5.bf.pcri;
    i.aosi = u5.bf.aosi;
    i.aopi = u5.bf.aopi;

    i.roaai   = u6.bf.roaai;
    i.epcosi  = u6.bf.epcosi;
    i.cpopci  = u6.bf.cpopci;
    i.pmtsmi  = u6.bf.pmtsmi;
    i.s11tf   = u6.bf.s11tf;
    i.pnsi    = u6.bf.pnsi;
    i.unaccsi = u6.bf.unaccsi;
    i.wpmsi   = u6.bf.wpmsi;

    i.spare1 = 0;
    i.spare2 = 0;
    i.spare3 = 0;
    i.eevrsi = u7.bf.eevrsi;
    i.ltemui = u7.bf.ltemui;
    i.ltempi = u7.bf.ltempi;
    i.enbcpi = u7.bf.enbcpi;
    i.tspcmi = u7.bf.tspcmi;
  };
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    if (tlv.get_length() > 1) {
      os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
      if (tlv.get_length() > 2) {
        os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
        if (tlv.get_length() > 3) {
          os.write(reinterpret_cast<const char*>(&u4.b), sizeof(u4.b));
          if (tlv.get_length() > 4) {
            os.write(reinterpret_cast<const char*>(&u5.b), sizeof(u5.b));
            if (tlv.get_length() > 5) {
              os.write(reinterpret_cast<const char*>(&u6.b), sizeof(u6.b));
              if (tlv.get_length() > 6) {
                os.write(reinterpret_cast<const char*>(&u7.b), sizeof(u7.b));
              }
            }
          }
        }
      }
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() < 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    if (tlv.get_length() > 1) {
      is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
      if (tlv.get_length() > 2) {
        is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
        if (tlv.get_length() > 3) {
          is.read(reinterpret_cast<char*>(&u4.b), sizeof(u4.b));
          if (tlv.get_length() > 4) {
            is.read(reinterpret_cast<char*>(&u5.b), sizeof(u5.b));
            if (tlv.get_length() > 5) {
              is.read(reinterpret_cast<char*>(&u6.b), sizeof(u6.b));
              if (tlv.get_length() > 6) {
                is.read(reinterpret_cast<char*>(&u7.b), sizeof(u7.b));
              }
            }
          }
        }
      }
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    indication_t indication = {};
    to_core_type(indication);
    s.set(indication, instance);
  }
};
//-------------------------------------
// 8.13 Protocol Configuration Options (PCO)

class protocol_or_container_id : public stream_serializable {
 public:
  uint16_t protocol_id;
  uint8_t length_of_protocol_id_contents;
  std::string protocol_id_contents;

  protocol_or_container_id() {
    protocol_id                    = 0;
    length_of_protocol_id_contents = 0;
    protocol_id_contents           = {};
  }
  explicit protocol_or_container_id(std::istream& is)
      : protocol_or_container_id() {
    load_from(is);
  }

  protocol_or_container_id(
      const uint16_t proto_id, const std::string& proto_id_contents)
      : protocol_id(proto_id),
        length_of_protocol_id_contents(protocol_id_contents.size()),
        protocol_id_contents(proto_id_contents) {}

  explicit protocol_or_container_id(const protocol_or_container_id& p)
      : protocol_id(p.protocol_id),
        length_of_protocol_id_contents(p.length_of_protocol_id_contents),
        protocol_id_contents(p.protocol_id_contents) {}

  explicit protocol_or_container_id(const pco_protocol_or_container_id_t& p)
      : protocol_id(p.protocol_id),
        length_of_protocol_id_contents(p.length_of_protocol_id_contents),
        protocol_id_contents(p.protocol_id_contents) {}

  protocol_or_container_id& operator=(protocol_or_container_id other) {
    std::swap(protocol_id, other.protocol_id);
    std::swap(
        length_of_protocol_id_contents, other.length_of_protocol_id_contents);
    std::swap(protocol_id_contents, other.protocol_id_contents);
    return *this;
  }
  //--------
  void dump_to(std::ostream& os) {
    auto ns_protocol_id = htons(protocol_id);
    os.write(
        reinterpret_cast<const char*>(&ns_protocol_id), sizeof(ns_protocol_id));
    os.write(
        reinterpret_cast<const char*>(&length_of_protocol_id_contents),
        sizeof(length_of_protocol_id_contents));
    os << protocol_id_contents;
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&protocol_id), sizeof(protocol_id));
    protocol_id = ntohs(protocol_id);
    is.read(
        reinterpret_cast<char*>(&length_of_protocol_id_contents),
        sizeof(length_of_protocol_id_contents));
    char tmpchar[length_of_protocol_id_contents];
    is.read(tmpchar, length_of_protocol_id_contents);
    protocol_id_contents.assign(tmpchar, length_of_protocol_id_contents);
  }
};

class gtpv2c_pco_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t configuration_protocol : 3;
      uint8_t spare : 4;
      uint8_t ext : 1;
    } bf;
    uint8_t b;
  } u1;
  uint8_t num_protocol_or_container_id;
  std::vector<protocol_or_container_id> protocol_or_container_ids;

  //--------
  gtpv2c_pco_ie()
      : gtpv2c_ie(GTP_IE_PROTOCOL_CONFIGURATION_OPTIONS),
        protocol_or_container_ids(),
        num_protocol_or_container_id(0) {
    tlv.length = 1;
    u1.b       = 0;
    u1.bf.ext  = 1;
    protocol_or_container_ids.reserve(
        PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID);
  }
  //--------
  explicit gtpv2c_pco_ie(const protocol_configuration_options_t& p)
      : gtpv2c_pco_ie() {
    tlv.length                   = 1;
    u1.b                         = 0;
    u1.bf.ext                    = p.ext;
    u1.bf.configuration_protocol = p.configuration_protocol;
    protocol_or_container_ids.reserve(
        PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID);
    for (int i = 0; i < p.num_protocol_or_container_id; i++) {
      protocol_or_container_ids.push_back(
          protocol_or_container_id(p.protocol_or_container_ids[i]));
      tlv.length +=
          (p.protocol_or_container_ids[i].length_of_protocol_id_contents + 3);
    }
    num_protocol_or_container_id = protocol_or_container_ids.size();
  }
  //--------
  explicit gtpv2c_pco_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b                         = 0;
    u1.bf.ext                    = 1;
    num_protocol_or_container_id = 0;
    protocol_or_container_ids.reserve(
        PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID);
  };
  //--------
  void to_core_type(protocol_configuration_options_t& p) {
    p                              = {0};
    p.ext                          = u1.bf.ext;
    p.configuration_protocol       = u1.bf.configuration_protocol;
    p.num_protocol_or_container_id = protocol_or_container_ids.size();
    int i                          = 0;
    for (auto& x : protocol_or_container_ids) {
      p.protocol_or_container_ids[i].protocol_id = x.protocol_id;
      p.protocol_or_container_ids[i].length_of_protocol_id_contents =
          x.length_of_protocol_id_contents;
      p.protocol_or_container_ids[i].protocol_id_contents =
          x.protocol_id_contents;
      i++;
    }
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    for (auto& it : protocol_or_container_ids) {
      it.dump_to(os);
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    num_protocol_or_container_id = 0;
    int32_t remaining_size       = tlv.get_length() - 1;
    while (remaining_size > 0) {
      protocol_or_container_id container = {};
      container.load_from(is);
      if ((container.length_of_protocol_id_contents + 3) <= remaining_size) {
        protocol_or_container_ids.push_back(container);
        remaining_size -= (container.length_of_protocol_id_contents + 3);
        num_protocol_or_container_id++;
      } else {
        throw gtpc_tlv_bad_length_exception(tlv.get_type(), tlv.get_length());
      }
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    protocol_configuration_options_t pco = {};
    to_core_type(pco);
    s.set(pco, instance);
  }
};

//-------------------------------------
// 8.19 EPS Bearer Level Traffic Flow Template (Bearer TFT)
class gtpv2c_bearer_tft_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t numberofpacketfilters : 4;
      uint8_t ebit : 1;
      uint8_t tftoperationcode : 3;
    } bf;
    uint8_t b;
  } u1;
  // std::vector<protocol_or_container_id>         protocol_or_container_ids;

  //--------
  gtpv2c_bearer_tft_ie()
      : gtpv2c_ie(GTP_IE_EPS_BEARER_LEVEL_TRAFFIC_FLOW_TEMPLATE) {
    tlv.length = 2;
    u1.b       = 0;
  }
  //--------
  explicit gtpv2c_bearer_tft_ie(const traffic_flow_template_t& t)
      : gtpv2c_bearer_tft_ie() {
    tlv.length                  = 2;
    u1.bf.tftoperationcode      = t.tftoperationcode;
    u1.bf.ebit                  = t.ebit;
    u1.bf.numberofpacketfilters = t.numberofpacketfilters;
  }
  //--------
  explicit gtpv2c_bearer_tft_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
  };
  //--------
  void to_core_type(traffic_flow_template_t& p) {
    p                  = {0};
    p.tftoperationcode = u1.bf.tftoperationcode;
    p.ebit             = u1.bf.ebit;
    // p.numberofpacketfilters = protocol_or_container_ids.size();
    // int i = 0;
    // for (auto& x : protocol_or_container_ids) {
    //  p.protocol_or_container_ids[i].protocol_id = x.protocol_id;
    //  p.protocol_or_container_ids[i].length_of_protocol_id_contents =
    //  x.length_of_protocol_id_contents;
    //  p.protocol_or_container_ids[i].protocol_id_contents =
    //  x.protocol_id_contents; i++;
    //}
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    // os << num_protocol_or_container_id;
    // for (int i = 0; i < num_protocol_or_container_id; i++) {
    //  protocol_or_container_ids[i].dump_to(os);
    //}
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    // is >> num_protocol_or_container_id;
    // for (int i = 0; i < num_protocol_or_container_id; i++) {
    //  protocol_or_container_ids[i].load_from(is);
    //}
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    traffic_flow_template_t tft = {};
    to_core_type(tft);
    s.set(tft, instance);
  }
};

//-------------------------------------
// 8.28 Bearer Context
class gtpv2c_bearer_context_ie : public gtpv2c_grouped_ie {
 public:
  //--------
  explicit gtpv2c_bearer_context_ie(const bearer_context& b)
      : gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT) {
    std::cout << this
              << "gtpv2c_bearer_context_ie::gtpv2c_bearer_context_ie(const "
                 "bearer_context& b)"
              << std::endl;
  }
  //--------
  gtpv2c_bearer_context_ie() : gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT) {
    tlv.length = 0;
  }
  //--------
  explicit gtpv2c_bearer_context_ie(const gtpv2c_tlv& t)
      : gtpv2c_grouped_ie(t){

        };
  //--------
  void to_core_type(bearer_context& c) {
    for (auto sie : ies) {
      sie.get()->to_core_type(c, sie.get()->tlv.get_instance());
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    bearer_context bearer_context = {};
    to_core_type(bearer_context);
    s.set(bearer_context, instance);
  }
};

//-------------------------------------
// 8.29 Charging ID
class gtpv2c_charging_id_ie : public gtpv2c_ie {
 public:
  uint32_t charging_id;

  //--------
  explicit gtpv2c_charging_id_ie(const charging_id_t& c)
      : gtpv2c_ie(GTP_IE_CHARGING_ID) {
    charging_id = c.charging_id_value;
    tlv.length  = 4;
  }
  //--------
  gtpv2c_charging_id_ie() : gtpv2c_ie(GTP_IE_CHARGING_ID) {
    charging_id = 0;
    tlv.length  = 4;
  }
  //--------
  explicit gtpv2c_charging_id_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    charging_id = 0;
  };
  //--------
  void to_core_type(charging_id_t& c) { c.charging_id_value = charging_id; }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    auto nl_charging_id = htonl(charging_id);
    os.write(
        reinterpret_cast<const char*>(&nl_charging_id), sizeof(nl_charging_id));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 4) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&charging_id), sizeof(charging_id));
    charging_id = ntohl(charging_id);
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    charging_id_t charging_id = {};
    to_core_type(charging_id);
    s.set(charging_id, instance);
  }
};

//-------------------------------------
// 8.32 Bearer Flags
class gtpv2c_bearer_flags_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t ppc : 1;
      uint8_t vb : 1;
      uint8_t vind : 1;
      uint8_t asi : 1;
      uint8_t spare1 : 4;
    } bf;
    uint8_t b;
  } u1;

  //--------
  explicit gtpv2c_bearer_flags_ie(const bearer_flags_t& b)
      : gtpv2c_ie(GTP_IE_CHARGING_ID) {
    u1.b       = 0;
    u1.bf.asi  = b.asi;
    u1.bf.vind = b.vind;
    u1.bf.vb   = b.vb;
    u1.bf.ppc  = b.ppc;
    tlv.length = 1;
  }
  //--------
  gtpv2c_bearer_flags_ie() : gtpv2c_ie(GTP_IE_CHARGING_ID) {
    u1.b       = 0;
    tlv.length = 1;
  }
  //--------
  explicit gtpv2c_bearer_flags_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
  };
  //--------
  void to_core_type(bearer_flags_t& b) {
    b.spare1 = u1.bf.spare1;
    b.asi    = u1.bf.asi;
    b.vind   = u1.bf.vind;
    b.vb     = u1.bf.vb;
    b.ppc    = u1.bf.ppc;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    bearer_flags_t bearer_flags = {};
    to_core_type(bearer_flags);
    s.set(bearer_flags, instance);
  }
};

//-------------------------------------
// 8.34 PDN Type
class gtpv2c_pdn_type_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t pdn_type : 3;
      uint8_t spare : 5;
    } bf;
    uint8_t b;
  } u1;

  //--------
  explicit gtpv2c_pdn_type_ie(const pdn_type_t& p)
      : gtpv2c_ie(GTP_IE_PDN_TYPE) {
    u1.bf.pdn_type = p.pdn_type;
    u1.bf.spare    = 0;
    tlv.length     = 1;
  }
  //--------
  gtpv2c_pdn_type_ie() : gtpv2c_ie(GTP_IE_PDN_TYPE) {
    u1.b       = 0;
    tlv.length = 1;
  }
  //--------
  explicit gtpv2c_pdn_type_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) { u1.b = 0; };
  //--------
  void to_core_type(pdn_type_t& p) { p.pdn_type = u1.bf.pdn_type; }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    pdn_type_t pdn_type = {};
    to_core_type(pdn_type);
    s.set(pdn_type, instance);
  }
};

//------------------------------------------------------------------------------
// 8.14 PDN Address Allocation (PAA)
class gtpv2c_paa_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t pdn_type : 3;
      uint8_t spare : 5;
    } bf;
    uint8_t b;
  } u1;
  uint8_t ipv6_prefix_length;
  in6_addr ipv6_address;
  in_addr ipv4_address;  // network byte order
  //--------
  explicit gtpv2c_paa_ie(const paa_t& p)
      : gtpv2c_ie(GTP_IE_PDN_ADDRESS_ALLOCATION) {
    u1.b           = 0;
    u1.bf.pdn_type = p.pdn_type.pdn_type;
    switch (u1.bf.pdn_type) {
      case PDN_TYPE_E_IPV4:
        ipv6_prefix_length = 0;
        ipv6_address       = in6addr_any;
        ipv4_address       = p.ipv4_address;
        tlv.length         = 5;
        break;
      case PDN_TYPE_E_IPV6:
        ipv6_prefix_length  = p.ipv6_prefix_length;
        ipv6_address        = p.ipv6_address;
        ipv4_address.s_addr = INADDR_ANY;
        tlv.length          = 18;
        break;
      case PDN_TYPE_E_IPV4V6:
        ipv6_prefix_length = p.ipv6_prefix_length;
        ipv6_address       = p.ipv6_address;
        ipv4_address       = p.ipv4_address;
        tlv.length         = 22;
        break;
      case PDN_TYPE_E_NON_IP:
        ipv6_prefix_length  = 0;
        ipv6_address        = in6addr_any;
        tlv.length          = 1;
        ipv4_address.s_addr = INADDR_ANY;
        break;
      default:
        throw gtpc_ie_value_exception(
            GTP_IE_PDN_ADDRESS_ALLOCATION, "pdn_type");
    }
  }
  //--------
  gtpv2c_paa_ie() : gtpv2c_ie(GTP_IE_PDN_ADDRESS_ALLOCATION) {
    u1.bf.pdn_type      = PDN_TYPE_E_NON_IP;
    ipv6_prefix_length  = 0;
    ipv6_address        = in6addr_any;
    tlv.length          = 1;
    ipv4_address.s_addr = INADDR_ANY;
  }
  //--------
  explicit gtpv2c_paa_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.bf.pdn_type      = PDN_TYPE_E_NON_IP;
    ipv6_prefix_length  = 0;
    ipv6_address        = in6addr_any;
    ipv4_address.s_addr = INADDR_ANY;
  };
  //--------
  void to_core_type(paa_t& p) {
    p.pdn_type.pdn_type = u1.bf.pdn_type;
    switch (u1.bf.pdn_type) {
      case PDN_TYPE_E_IPV4:
        p.ipv6_prefix_length = 0;
        p.ipv6_address       = in6addr_any;
        p.ipv4_address       = ipv4_address;
        break;
      case PDN_TYPE_E_IPV6:
        p.ipv6_prefix_length  = ipv6_prefix_length;
        p.ipv6_address        = ipv6_address;
        p.ipv4_address.s_addr = INADDR_ANY;
        break;
      case PDN_TYPE_E_IPV4V6:
        p.ipv6_prefix_length = ipv6_prefix_length;
        p.ipv6_address       = ipv6_address;
        p.ipv4_address       = ipv4_address;
        break;
      case PDN_TYPE_E_NON_IP:
        p.ipv6_prefix_length  = 0;
        p.ipv6_address        = in6addr_any;
        p.ipv4_address.s_addr = INADDR_ANY;
        break;
      default:
        throw gtpc_ie_value_exception(
            GTP_IE_PDN_ADDRESS_ALLOCATION, "pdn_type");
    }
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    switch (u1.bf.pdn_type) {
      case PDN_TYPE_E_IPV4:
        ipv4_address_dump_to(os, ipv4_address);
        break;
      case PDN_TYPE_E_IPV6:
        os << ipv6_prefix_length;
        ipv6_address_dump_to(os, ipv6_address);
        break;
      case PDN_TYPE_E_IPV4V6:
        os.write(
            reinterpret_cast<const char*>(&ipv6_prefix_length),
            sizeof(ipv6_prefix_length));
        ipv6_address_dump_to(os, ipv6_address);
        ipv4_address_dump_to(os, ipv4_address);
        break;
      case PDN_TYPE_E_NON_IP:
        break;
      default:
        throw gtpc_ie_value_exception(
            GTP_IE_PDN_ADDRESS_ALLOCATION, "pdn_type");
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    switch (u1.bf.pdn_type) {
      case PDN_TYPE_E_IPV4:
        if (tlv.length != 5)
          throw gtpc_tlv_bad_length_exception(
              GTP_IE_PDN_ADDRESS_ALLOCATION, tlv.length);
        ipv6_prefix_length = 0;
        ipv6_address       = in6addr_any;
        ipv4_address_load_from(is, ipv4_address);
        break;
      case PDN_TYPE_E_IPV6:
        if (tlv.length != 18)
          throw gtpc_tlv_bad_length_exception(
              GTP_IE_PDN_ADDRESS_ALLOCATION, tlv.length);
        is.read(
            reinterpret_cast<char*>(&ipv6_prefix_length),
            sizeof(ipv6_prefix_length));
        ipv6_address_load_from(is, ipv6_address);
        ipv4_address.s_addr = INADDR_ANY;
        break;
      case PDN_TYPE_E_IPV4V6:
        if (tlv.length != 22)
          throw gtpc_tlv_bad_length_exception(
              GTP_IE_PDN_ADDRESS_ALLOCATION, tlv.length);
        is.read(
            reinterpret_cast<char*>(&ipv6_prefix_length),
            sizeof(ipv6_prefix_length));
        ipv6_address_load_from(is, ipv6_address);
        ipv4_address_load_from(is, ipv4_address);
        break;
      case PDN_TYPE_E_NON_IP:
        if (tlv.length != 1)
          throw gtpc_tlv_bad_length_exception(
              GTP_IE_PDN_ADDRESS_ALLOCATION, tlv.length);
        ipv6_prefix_length  = 0;
        ipv6_address        = in6addr_any;
        ipv4_address.s_addr = INADDR_ANY;
        break;
      default:
        throw gtpc_ie_value_exception(
            GTP_IE_PDN_ADDRESS_ALLOCATION, "pdn_type");
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    paa_t paa = {};
    to_core_type(paa);
    s.set(paa, instance);
  }
};
//-------------------------------------
// 8.15 Bearer Quality of Service (Bearer QoS)
class gtpv2c_bearer_qos_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t pvi : 1;
      uint8_t spare2 : 1;
      uint8_t pl : 4;
      uint8_t pci : 1;
      uint8_t spare1 : 1;
    } bf;
    uint8_t b;
  } u1;
  uint8_t label_qci;
  uint8_t maximum_bit_rate_for_uplink[5];
  uint8_t maximum_bit_rate_for_downlink[5];
  uint8_t guaranted_bit_rate_for_uplink[5];
  uint8_t guaranted_bit_rate_for_downlink[5];

  //--------
  explicit gtpv2c_bearer_qos_ie(const bearer_qos_t& b)
      : gtpv2c_ie(GTP_IE_BEARER_QUALITY_OF_SERVICE) {
    u1.b                = 0;
    u1.bf.pci           = b.pci;
    u1.bf.pl            = b.pl;
    u1.bf.pvi           = b.pvi;
    label_qci           = b.label_qci;
    uint64_t max_uplink = b.maximum_bit_rate_for_uplink & 0x000000FFFFFFFFFF;
    uint64_t max_downlink =
        b.maximum_bit_rate_for_downlink & 0x000000FFFFFFFFFF;
    uint64_t guar_uplink = b.guaranted_bit_rate_for_uplink & 0x000000FFFFFFFFFF;
    uint64_t guar_downlink =
        b.guaranted_bit_rate_for_downlink & 0x000000FFFFFFFFFF;

    for (int i = 4; i >= 0; i--) {
      maximum_bit_rate_for_uplink[i]     = max_uplink & 0xFF;
      maximum_bit_rate_for_downlink[i]   = max_downlink & 0xFF;
      guaranted_bit_rate_for_uplink[i]   = guar_uplink & 0xFF;
      guaranted_bit_rate_for_downlink[i] = guar_downlink & 0xFF;

      max_uplink    = max_uplink >> 8;
      max_downlink  = max_downlink >> 8;
      guar_uplink   = guar_uplink >> 8;
      guar_downlink = guar_downlink >> 8;
    }
    tlv.length = 22;
  }
  //--------
  gtpv2c_bearer_qos_ie() : gtpv2c_ie(GTP_IE_BEARER_QUALITY_OF_SERVICE) {
    u1.b      = 0;
    label_qci = 0;
    for (int i = 0; i < 5; i++) {
      maximum_bit_rate_for_uplink[i]     = 0;
      maximum_bit_rate_for_downlink[i]   = 0;
      guaranted_bit_rate_for_uplink[i]   = 0;
      guaranted_bit_rate_for_downlink[i] = 0;
    }
    tlv.length = 22;
  }
  //--------
  explicit gtpv2c_bearer_qos_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b      = 0;
    label_qci = 0;
    for (int i = 0; i < 5; i++) {
      maximum_bit_rate_for_uplink[i]     = 0;
      maximum_bit_rate_for_downlink[i]   = 0;
      guaranted_bit_rate_for_uplink[i]   = 0;
      guaranted_bit_rate_for_downlink[i] = 0;
    }
  };
  //--------
  void to_core_type(bearer_qos_t& b) {
    b           = {0};
    b.pci       = u1.bf.pci;
    b.pl        = u1.bf.pl;
    b.pvi       = u1.bf.pvi;
    b.label_qci = label_qci;

    uint64_t max_uplink    = 0;
    uint64_t max_downlink  = 0;
    uint64_t guar_uplink   = 0;
    uint64_t guar_downlink = 0;

    for (int i = 0; i < 5; i++) {
      max_uplink    = max_uplink << 8;
      max_downlink  = max_downlink << 8;
      guar_uplink   = guar_uplink << 8;
      guar_downlink = guar_downlink << 8;

      max_uplink |= maximum_bit_rate_for_uplink[i];
      max_downlink |= maximum_bit_rate_for_downlink[i];
      guar_uplink |= guaranted_bit_rate_for_uplink[i];
      guar_downlink |= guaranted_bit_rate_for_downlink[i];
    }
    b.maximum_bit_rate_for_uplink     = max_uplink;
    b.maximum_bit_rate_for_downlink   = max_downlink;
    b.guaranted_bit_rate_for_uplink   = guar_uplink;
    b.guaranted_bit_rate_for_downlink = guar_downlink;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&label_qci), sizeof(label_qci));
    os.write(reinterpret_cast<const char*>(maximum_bit_rate_for_uplink), 5);
    os.write(reinterpret_cast<const char*>(maximum_bit_rate_for_downlink), 5);
    os.write(reinterpret_cast<const char*>(guaranted_bit_rate_for_uplink), 5);
    os.write(reinterpret_cast<const char*>(guaranted_bit_rate_for_downlink), 5);
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.length != 22)
      throw gtpc_tlv_bad_length_exception(
          GTP_IE_BEARER_QUALITY_OF_SERVICE, tlv.length);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&label_qci), sizeof(label_qci));
    is.read(reinterpret_cast<char*>(maximum_bit_rate_for_uplink), 5);
    is.read(reinterpret_cast<char*>(maximum_bit_rate_for_downlink), 5);
    is.read(reinterpret_cast<char*>(guaranted_bit_rate_for_uplink), 5);
    is.read(reinterpret_cast<char*>(guaranted_bit_rate_for_downlink), 5);
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    bearer_qos_t bearer_qos = {};
    to_core_type(bearer_qos);
    s.set(bearer_qos, instance);
  }
};

//-------------------------------------
// 8.16 Flow Quality of Service (Flow QoS)
class gtpv2c_flow_qos_ie : public gtpv2c_ie {
 public:
  uint8_t label_qci;
  uint8_t maximum_bit_rate_for_uplink[5];
  uint8_t maximum_bit_rate_for_downlink[5];
  uint8_t guaranted_bit_rate_for_uplink[5];
  uint8_t guaranted_bit_rate_for_downlink[5];

  //--------
  explicit gtpv2c_flow_qos_ie(const flow_qos_t& b)
      : gtpv2c_ie(GTP_IE_FLOW_QUALITY_OF_SERVICE) {
    label_qci           = b.label_qci;
    uint64_t max_uplink = b.maximum_bit_rate_for_uplink & 0x000000FFFFFFFFFF;
    uint64_t max_downlink =
        b.maximum_bit_rate_for_downlink & 0x000000FFFFFFFFFF;
    uint64_t guar_uplink = b.guaranted_bit_rate_for_uplink & 0x000000FFFFFFFFFF;
    uint64_t guar_downlink =
        b.guaranted_bit_rate_for_downlink & 0x000000FFFFFFFFFF;

    for (int i = 4; i <= 0; i--) {
      maximum_bit_rate_for_uplink[i]     = max_uplink & 0xFF;
      maximum_bit_rate_for_downlink[i]   = max_downlink & 0xFF;
      guaranted_bit_rate_for_uplink[i]   = guar_uplink & 0xFF;
      guaranted_bit_rate_for_downlink[i] = guar_downlink & 0xFF;

      max_uplink    = max_uplink >> 8;
      max_downlink  = max_downlink >> 8;
      guar_uplink   = guar_uplink >> 8;
      guar_downlink = guar_downlink >> 8;
    }
    tlv.length = 21;
  }
  //--------
  gtpv2c_flow_qos_ie() : gtpv2c_ie(GTP_IE_FLOW_QUALITY_OF_SERVICE) {
    label_qci = 0;
    for (int i = 0; i < 5; i++) {
      maximum_bit_rate_for_uplink[i]     = 0;
      maximum_bit_rate_for_downlink[i]   = 0;
      guaranted_bit_rate_for_uplink[i]   = 0;
      guaranted_bit_rate_for_downlink[i] = 0;
    }
    tlv.length = 21;
  }
  //--------
  explicit gtpv2c_flow_qos_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    label_qci = 0;
    for (int i = 0; i < 5; i++) {
      maximum_bit_rate_for_uplink[i]     = 0;
      maximum_bit_rate_for_downlink[i]   = 0;
      guaranted_bit_rate_for_uplink[i]   = 0;
      guaranted_bit_rate_for_downlink[i] = 0;
    }
  };
  //--------
  void to_core_type(flow_qos_t& b) {
    b           = {0};
    b.label_qci = label_qci;

    uint64_t max_uplink    = 0;
    uint64_t max_downlink  = 0;
    uint64_t guar_uplink   = 0;
    uint64_t guar_downlink = 0;

    for (int i = 0; i < 5; i++) {
      max_uplink |= maximum_bit_rate_for_uplink[i];
      max_downlink |= maximum_bit_rate_for_downlink[i];
      guar_uplink |= guaranted_bit_rate_for_uplink[i];
      guar_downlink |= guaranted_bit_rate_for_downlink[i];

      max_uplink    = max_uplink << 8;
      max_downlink  = max_downlink << 8;
      guar_uplink   = guar_uplink << 8;
      guar_downlink = guar_downlink << 8;
    }
    b.maximum_bit_rate_for_uplink     = max_uplink;
    b.maximum_bit_rate_for_downlink   = max_downlink;
    b.guaranted_bit_rate_for_uplink   = guar_uplink;
    b.guaranted_bit_rate_for_downlink = guar_downlink;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&label_qci), sizeof(label_qci));
    os.write(
        reinterpret_cast<const char*>(maximum_bit_rate_for_uplink),
        sizeof(maximum_bit_rate_for_uplink));
    os.write(
        reinterpret_cast<const char*>(maximum_bit_rate_for_downlink),
        sizeof(maximum_bit_rate_for_downlink));
    os.write(
        reinterpret_cast<const char*>(guaranted_bit_rate_for_uplink),
        sizeof(guaranted_bit_rate_for_uplink));
    os.write(
        reinterpret_cast<const char*>(guaranted_bit_rate_for_downlink),
        sizeof(guaranted_bit_rate_for_downlink));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.length != 21)
      throw gtpc_tlv_bad_length_exception(
          GTP_IE_FLOW_QUALITY_OF_SERVICE, tlv.length);
    is.read(reinterpret_cast<char*>(&label_qci), sizeof(label_qci));
    is.read(
        reinterpret_cast<char*>(maximum_bit_rate_for_uplink),
        sizeof(maximum_bit_rate_for_uplink));
    is.read(
        reinterpret_cast<char*>(maximum_bit_rate_for_downlink),
        sizeof(maximum_bit_rate_for_downlink));
    is.read(
        reinterpret_cast<char*>(guaranted_bit_rate_for_uplink),
        sizeof(guaranted_bit_rate_for_uplink));
    is.read(
        reinterpret_cast<char*>(guaranted_bit_rate_for_downlink),
        sizeof(guaranted_bit_rate_for_downlink));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    flow_qos_t flow_qos = {};
    to_core_type(flow_qos);
    s.set(flow_qos, instance);
  }
};

//-------------------------------------
// 8.17 RAT Type
class gtpv2c_rat_type_ie : public gtpv2c_ie {
 public:
  uint8_t rat_type;

  //--------
  explicit gtpv2c_rat_type_ie(const rat_type_t& r)
      : gtpv2c_ie(GTP_IE_RAT_TYPE) {
    rat_type   = r.rat_type;
    tlv.length = 1;
  }
  //--------
  gtpv2c_rat_type_ie() : gtpv2c_ie(GTP_IE_RAT_TYPE) {
    rat_type   = RAT_TYPE_E_RESERVED;
    tlv.length = 1;
  }
  //--------
  explicit gtpv2c_rat_type_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    rat_type = RAT_TYPE_E_RESERVED;
  };
  //--------
  void to_core_type(rat_type_t& r) { r.rat_type = rat_type; }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&rat_type), sizeof(rat_type));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.length != 1)
      throw gtpc_tlv_bad_length_exception(GTP_IE_RAT_TYPE, tlv.length);
    is.read(reinterpret_cast<char*>(&rat_type), sizeof(rat_type));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    rat_type_t rat_type = {};
    to_core_type(rat_type);
    s.set(rat_type, instance);
  }
};
//-------------------------------------
// 8.18 Serving Network
class gtpv2c_serving_network_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;

  //--------
  explicit gtpv2c_serving_network_ie(const serving_network_t& s)
      : gtpv2c_ie(GTP_IE_SERVING_NETWORK) {
    u1.bf.mcc_digit_2 = s.mcc_digit_2;
    u1.bf.mcc_digit_1 = s.mcc_digit_1;
    u2.bf.mnc_digit_3 = s.mnc_digit_3;
    u2.bf.mcc_digit_3 = s.mcc_digit_3;
    u3.bf.mnc_digit_2 = s.mnc_digit_2;
    u3.bf.mnc_digit_1 = s.mnc_digit_1;
    tlv.length        = 3;
  }
  //--------
  gtpv2c_serving_network_ie() : gtpv2c_ie(GTP_IE_SERVING_NETWORK) {
    u1.b       = 0;
    u2.b       = 0;
    u3.b       = 0;
    tlv.length = 3;
  }
  //--------
  explicit gtpv2c_serving_network_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
    u2.b = 0;
    u3.b = 0;
  };
  //--------
  explicit gtpv2c_serving_network_ie(const gtpv2c_serving_network_ie& i)
      : gtpv2c_ie(i), u1(i.u1), u2(i.u2), u3(i.u3) {}
  //--------
  gtpv2c_serving_network_ie& operator=(gtpv2c_serving_network_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    std::swap(u2, other.u2);
    std::swap(u3, other.u3);
    return *this;
  }
  //--------
  void to_core_type(serving_network_t& s) {
    s.mcc_digit_2 = u1.bf.mcc_digit_2;
    s.mcc_digit_1 = u1.bf.mcc_digit_1;
    s.mnc_digit_3 = u2.bf.mnc_digit_3;
    s.mcc_digit_3 = u2.bf.mcc_digit_3;
    s.mnc_digit_2 = u3.bf.mnc_digit_2;
    s.mnc_digit_1 = u3.bf.mnc_digit_1;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.length != 3)
      throw gtpc_tlv_bad_length_exception(GTP_IE_SERVING_NETWORK, tlv.length);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    serving_network_t serving_network = {};
    to_core_type(serving_network);
    s.set(serving_network, instance);
  }
};

//-------------------------------------
// 8.21.1 CGI field
class gtpv2c_cgi_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  uint16_t location_area_code;
  uint16_t cell_identity;

  //--------
  explicit gtpv2c_cgi_field_ie(const cgi_field_t& c) {
    u1.bf.mcc_digit_2  = c.mcc_digit_2;
    u1.bf.mcc_digit_1  = c.mcc_digit_1;
    u2.bf.mnc_digit_3  = c.mnc_digit_3;
    u2.bf.mcc_digit_3  = c.mcc_digit_3;
    u3.bf.mnc_digit_2  = c.mnc_digit_2;
    u3.bf.mnc_digit_1  = c.mnc_digit_1;
    location_area_code = c.location_area_code;
    cell_identity      = c.cell_identity;
  }
  //--------
  gtpv2c_cgi_field_ie() {
    u1.b               = 0;
    u2.b               = 0;
    u3.b               = 0;
    location_area_code = 0;
    cell_identity      = 0;
  }
  //--------
  void to_core_type(cgi_field_t& c) {
    c.mcc_digit_2        = u1.bf.mcc_digit_2;
    c.mcc_digit_1        = u1.bf.mcc_digit_1;
    c.mnc_digit_3        = u2.bf.mnc_digit_3;
    c.mcc_digit_3        = u2.bf.mcc_digit_3;
    c.mnc_digit_2        = u3.bf.mnc_digit_2;
    c.mnc_digit_1        = u3.bf.mnc_digit_1;
    c.location_area_code = location_area_code;
    c.cell_identity      = cell_identity;
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    auto ns_location_area_code = htons(location_area_code);
    os.write(
        reinterpret_cast<const char*>(&ns_location_area_code),
        sizeof(ns_location_area_code));
    auto ns_cell_identity = htons(cell_identity);
    os.write(
        reinterpret_cast<const char*>(&ns_cell_identity),
        sizeof(ns_cell_identity));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(
        reinterpret_cast<char*>(&location_area_code),
        sizeof(location_area_code));
    location_area_code = ntohs(location_area_code);
    is.read(reinterpret_cast<char*>(&cell_identity), sizeof(cell_identity));
    cell_identity = ntohs(cell_identity);
  }
};
//-------------------------------------
// 8.21.2 SAI field
class gtpv2c_sai_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  uint16_t location_area_code;
  uint16_t service_area_code;

  //--------
  explicit gtpv2c_sai_field_ie(const sai_field_t& c) {
    u1.bf.mcc_digit_2  = c.mcc_digit_2;
    u1.bf.mcc_digit_1  = c.mcc_digit_1;
    u2.bf.mnc_digit_3  = c.mnc_digit_3;
    u2.bf.mcc_digit_3  = c.mcc_digit_3;
    u3.bf.mnc_digit_2  = c.mnc_digit_2;
    u3.bf.mnc_digit_1  = c.mnc_digit_1;
    location_area_code = c.location_area_code;
    service_area_code  = c.service_area_code;
  }
  //--------
  gtpv2c_sai_field_ie() {
    u1.b               = 0;
    u2.b               = 0;
    u3.b               = 0;
    location_area_code = 0;
    service_area_code  = 0;
  }
  //--------
  void to_core_type(sai_field_t& c) {
    c.mcc_digit_2        = u1.bf.mcc_digit_2;
    c.mcc_digit_1        = u1.bf.mcc_digit_1;
    c.mnc_digit_3        = u2.bf.mnc_digit_3;
    c.mcc_digit_3        = u2.bf.mcc_digit_3;
    c.mnc_digit_2        = u3.bf.mnc_digit_2;
    c.mnc_digit_1        = u3.bf.mnc_digit_1;
    c.location_area_code = location_area_code;
    c.service_area_code  = service_area_code;
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    auto ns_location_area_code = htons(location_area_code);
    os.write(
        reinterpret_cast<const char*>(&ns_location_area_code),
        sizeof(ns_location_area_code));
    auto ns_service_area_code = htons(service_area_code);
    os.write(
        reinterpret_cast<const char*>(&ns_service_area_code),
        sizeof(ns_service_area_code));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(
        reinterpret_cast<char*>(&location_area_code),
        sizeof(location_area_code));
    location_area_code = ntohs(location_area_code);
    is.read(
        reinterpret_cast<char*>(&service_area_code), sizeof(service_area_code));
    service_area_code = ntohs(service_area_code);
  }
};
//-------------------------------------
// 8.21.3 RAI field
class gtpv2c_rai_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  uint16_t location_area_code;
  uint16_t routing_area_code;

  //--------
  explicit gtpv2c_rai_field_ie(const rai_field_t& c) {
    u1.bf.mcc_digit_2  = c.mcc_digit_2;
    u1.bf.mcc_digit_1  = c.mcc_digit_1;
    u2.bf.mnc_digit_3  = c.mnc_digit_3;
    u2.bf.mcc_digit_3  = c.mcc_digit_3;
    u3.bf.mnc_digit_2  = c.mnc_digit_2;
    u3.bf.mnc_digit_1  = c.mnc_digit_1;
    location_area_code = c.location_area_code;
    routing_area_code  = c.routing_area_code;
  }
  //--------
  gtpv2c_rai_field_ie() {
    u1.b               = 0;
    u2.b               = 0;
    u3.b               = 0;
    location_area_code = 0;
    routing_area_code  = 0;
  }
  //--------
  void to_core_type(rai_field_t& c) {
    c.mcc_digit_2        = u1.bf.mcc_digit_2;
    c.mcc_digit_1        = u1.bf.mcc_digit_1;
    c.mnc_digit_3        = u2.bf.mnc_digit_3;
    c.mcc_digit_3        = u2.bf.mcc_digit_3;
    c.mnc_digit_2        = u3.bf.mnc_digit_2;
    c.mnc_digit_1        = u3.bf.mnc_digit_1;
    c.location_area_code = location_area_code;
    c.routing_area_code  = routing_area_code;
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    auto ns_location_area_code = htons(location_area_code);
    os.write(
        reinterpret_cast<const char*>(&ns_location_area_code),
        sizeof(ns_location_area_code));
    auto ns_routing_area_code = htons(routing_area_code);
    os.write(
        reinterpret_cast<const char*>(&ns_routing_area_code),
        sizeof(ns_routing_area_code));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(
        reinterpret_cast<char*>(&location_area_code),
        sizeof(location_area_code));
    location_area_code = ntohs(location_area_code);
    is.read(
        reinterpret_cast<char*>(&routing_area_code), sizeof(routing_area_code));
    routing_area_code = ntohs(routing_area_code);
  }
};
//-------------------------------------
// 8.21.4 TAI field
class gtpv2c_tai_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  uint16_t tracking_area_code;

  //--------
  explicit gtpv2c_tai_field_ie(const tai_field_t& t) {
    u1.bf.mcc_digit_2  = t.mcc_digit_2;
    u1.bf.mcc_digit_1  = t.mcc_digit_1;
    u2.bf.mnc_digit_3  = t.mnc_digit_3;
    u2.bf.mcc_digit_3  = t.mcc_digit_3;
    u3.bf.mnc_digit_2  = t.mnc_digit_2;
    u3.bf.mnc_digit_1  = t.mnc_digit_1;
    tracking_area_code = t.tracking_area_code;
  }
  //--------
  gtpv2c_tai_field_ie() {
    u1.b               = 0;
    u2.b               = 0;
    u3.b               = 0;
    tracking_area_code = 0;
  }
  //--------
  void to_core_type(tai_field_t& t) {
    t.mcc_digit_2        = u1.bf.mcc_digit_2;
    t.mcc_digit_1        = u1.bf.mcc_digit_1;
    t.mnc_digit_3        = u2.bf.mnc_digit_3;
    t.mcc_digit_3        = u2.bf.mcc_digit_3;
    t.mnc_digit_2        = u3.bf.mnc_digit_2;
    t.mnc_digit_1        = u3.bf.mnc_digit_1;
    t.tracking_area_code = tracking_area_code;
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    auto ns_tracking_area_code = htons(tracking_area_code);
    os.write(
        reinterpret_cast<const char*>(&ns_tracking_area_code),
        sizeof(ns_tracking_area_code));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(
        reinterpret_cast<char*>(&tracking_area_code),
        sizeof(tracking_area_code));
    tracking_area_code = ntohs(tracking_area_code);
  }
};

//-------------------------------------
// 8.21.5 ECGI field
class gtpv2c_ecgi_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  union {
    struct {
      uint8_t eci : 4;
      uint8_t spare : 4;
    } bf;
    uint8_t b;
  } u4;
  uint8_t e_utran_cell_identifier[3];

  //--------
  explicit gtpv2c_ecgi_field_ie(const ecgi_field_t& t) {
    u1.bf.mcc_digit_2          = t.mcc_digit_2;
    u1.bf.mcc_digit_1          = t.mcc_digit_1;
    u2.bf.mnc_digit_3          = t.mnc_digit_3;
    u2.bf.mcc_digit_3          = t.mcc_digit_3;
    u3.bf.mnc_digit_2          = t.mnc_digit_2;
    u3.bf.mnc_digit_1          = t.mnc_digit_1;
    u4.bf.spare                = 0;
    u4.bf.eci                  = t.eci;
    e_utran_cell_identifier[0] = t.e_utran_cell_identifier[0];
    e_utran_cell_identifier[1] = t.e_utran_cell_identifier[1];
    e_utran_cell_identifier[2] = t.e_utran_cell_identifier[2];
  }
  //--------
  gtpv2c_ecgi_field_ie() {
    u1.b                       = 0;
    u2.b                       = 0;
    u3.b                       = 0;
    u4.b                       = 0;
    e_utran_cell_identifier[0] = 0;
    e_utran_cell_identifier[1] = 0;
    e_utran_cell_identifier[2] = 0;
  }
  //--------
  void to_core_type(ecgi_field_t& t) {
    t.mcc_digit_2                = u1.bf.mcc_digit_2;
    t.mcc_digit_1                = u1.bf.mcc_digit_1;
    t.mnc_digit_3                = u2.bf.mnc_digit_3;
    t.mcc_digit_3                = u2.bf.mcc_digit_3;
    t.mnc_digit_2                = u3.bf.mnc_digit_2;
    t.mnc_digit_1                = u3.bf.mnc_digit_1;
    t.spare                      = u4.bf.spare;
    t.eci                        = u4.bf.eci;
    t.e_utran_cell_identifier[0] = e_utran_cell_identifier[0];
    t.e_utran_cell_identifier[1] = e_utran_cell_identifier[1];
    t.e_utran_cell_identifier[2] = e_utran_cell_identifier[2];
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    os.write(reinterpret_cast<const char*>(&u4.b), sizeof(u4.b));
    os.write(
        reinterpret_cast<const char*>(&e_utran_cell_identifier),
        sizeof(e_utran_cell_identifier));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(reinterpret_cast<char*>(&u4.b), sizeof(u4.b));
    is.read(
        reinterpret_cast<char*>(&e_utran_cell_identifier),
        sizeof(e_utran_cell_identifier));
  }
};

//-------------------------------------
// 8.21.6 LAI field
class gtpv2c_lai_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  uint16_t location_area_code;

  //--------
  explicit gtpv2c_lai_field_ie(const lai_field_t& t) {
    u1.bf.mcc_digit_2  = t.mcc_digit_2;
    u1.bf.mcc_digit_1  = t.mcc_digit_1;
    u2.bf.mnc_digit_3  = t.mnc_digit_3;
    u2.bf.mcc_digit_3  = t.mcc_digit_3;
    u3.bf.mnc_digit_2  = t.mnc_digit_2;
    u3.bf.mnc_digit_1  = t.mnc_digit_1;
    location_area_code = t.location_area_code;
  }
  //--------
  gtpv2c_lai_field_ie() {
    u1.b               = 0;
    u2.b               = 0;
    u3.b               = 0;
    location_area_code = 0;
  }
  //--------
  void to_core_type(lai_field_t& t) {
    t.mcc_digit_2        = u1.bf.mcc_digit_2;
    t.mcc_digit_1        = u1.bf.mcc_digit_1;
    t.mnc_digit_3        = u2.bf.mnc_digit_3;
    t.mcc_digit_3        = u2.bf.mcc_digit_3;
    t.mnc_digit_2        = u3.bf.mnc_digit_2;
    t.mnc_digit_1        = u3.bf.mnc_digit_1;
    t.location_area_code = location_area_code;
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    auto ns_location_area_code = htons(location_area_code);
    os.write(
        reinterpret_cast<const char*>(&ns_location_area_code),
        sizeof(ns_location_area_code));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(
        reinterpret_cast<char*>(&location_area_code),
        sizeof(location_area_code));
    location_area_code = ntohs(location_area_code);
  }
};
//-------------------------------------
// 8.21.7 Macro eNodeB ID field
class gtpv2c_macro_enodeb_id_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  union {
    struct {
      uint8_t macro_enodeb_id : 4;
      uint8_t spare : 4;
    } bf;
    uint8_t b;
  } u4;
  uint16_t macro_enodeb_id;

  //--------
  explicit gtpv2c_macro_enodeb_id_field_ie(const macro_enodeb_id_field_t& t) {
    u1.bf.mcc_digit_2     = t.mcc_digit_2;
    u1.bf.mcc_digit_1     = t.mcc_digit_1;
    u2.bf.mnc_digit_3     = t.mnc_digit_3;
    u2.bf.mcc_digit_3     = t.mcc_digit_3;
    u3.bf.mnc_digit_2     = t.mnc_digit_2;
    u3.bf.mnc_digit_1     = t.mnc_digit_1;
    u4.bf.spare           = 0;
    u4.bf.macro_enodeb_id = (t.macro_enodeb_id & 0x00FF0000) >> 16;
    macro_enodeb_id       = t.macro_enodeb_id & 0xFFFF;
  }
  //--------
  gtpv2c_macro_enodeb_id_field_ie() {
    u1.b            = 0;
    u2.b            = 0;
    u3.b            = 0;
    u4.b            = 0;
    macro_enodeb_id = 0;
  }
  //--------
  void to_core_type(macro_enodeb_id_field_t& t) {
    t.mcc_digit_2 = u1.bf.mcc_digit_2;
    t.mcc_digit_1 = u1.bf.mcc_digit_1;
    t.mnc_digit_3 = u2.bf.mnc_digit_3;
    t.mcc_digit_3 = u2.bf.mcc_digit_3;
    t.mnc_digit_2 = u3.bf.mnc_digit_2;
    t.mnc_digit_1 = u3.bf.mnc_digit_1;
    t.spare       = u4.bf.spare;
    t.macro_enodeb_id =
        macro_enodeb_id | ((uint32_t) u4.bf.macro_enodeb_id << 16);
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    auto ns_macro_enodeb_id = htons(macro_enodeb_id);
    os.write(
        reinterpret_cast<const char*>(&ns_macro_enodeb_id),
        sizeof(ns_macro_enodeb_id));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(reinterpret_cast<char*>(&macro_enodeb_id), sizeof(macro_enodeb_id));
    macro_enodeb_id = ntohs(macro_enodeb_id);
  }
};
//-------------------------------------
// 8.21.8 Extended Macro eNodeB ID field
class gtpv2c_extended_macro_enodeb_id_field_ie : public stream_serializable {
 public:
  union {
    struct {
      uint8_t mcc_digit_1 : 4;
      uint8_t mcc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct {
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
    } bf;
    uint8_t b;
  } u2;
  union {
    struct {
      uint8_t mnc_digit_1 : 4;
      uint8_t mnc_digit_2 : 4;
    } bf;
    uint8_t b;
  } u3;
  union {
    struct {
      uint8_t extended_macro_enodeb_id : 4;
      uint8_t spare : 3;
      uint8_t smenb : 1;
    } bf;
    uint8_t b;
  } u4;
  uint16_t extended_macro_enodeb_id;

  //--------
  explicit gtpv2c_extended_macro_enodeb_id_field_ie(
      const extended_macro_enodeb_id_field_t& t) {
    u1.bf.mcc_digit_2 = t.mcc_digit_2;
    u1.bf.mcc_digit_1 = t.mcc_digit_1;
    u2.bf.mnc_digit_3 = t.mnc_digit_3;
    u2.bf.mcc_digit_3 = t.mcc_digit_3;
    u3.bf.mnc_digit_2 = t.mnc_digit_2;
    u3.bf.mnc_digit_1 = t.mnc_digit_1;
    u4.bf.smenb       = t.smenb;
    u4.bf.spare       = 0;
    u4.bf.extended_macro_enodeb_id =
        (t.extended_macro_enodeb_id & 0x00FF0000) >> 16;
    extended_macro_enodeb_id = t.extended_macro_enodeb_id & 0xFFFF;
  }
  //--------
  gtpv2c_extended_macro_enodeb_id_field_ie() {
    u1.b                     = 0;
    u2.b                     = 0;
    u3.b                     = 0;
    u4.b                     = 0;
    extended_macro_enodeb_id = 0;
  }
  //--------
  void to_core_type(extended_macro_enodeb_id_field_t& t) {
    t.mcc_digit_2 = u1.bf.mcc_digit_2;
    t.mcc_digit_1 = u1.bf.mcc_digit_1;
    t.mnc_digit_3 = u2.bf.mnc_digit_3;
    t.mcc_digit_3 = u2.bf.mcc_digit_3;
    t.mnc_digit_2 = u3.bf.mnc_digit_2;
    t.mnc_digit_1 = u3.bf.mnc_digit_1;
    t.smenb       = u4.bf.smenb;
    t.spare       = u4.bf.spare;
    t.extended_macro_enodeb_id =
        extended_macro_enodeb_id |
        ((uint32_t) u4.bf.extended_macro_enodeb_id << 16);
    t.lost_bits = 0;
  }
  //--------
  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(reinterpret_cast<const char*>(&u2.b), sizeof(u2.b));
    os.write(reinterpret_cast<const char*>(&u3.b), sizeof(u3.b));
    os.write(reinterpret_cast<const char*>(&u4.b), sizeof(u4.b));
    auto ns_extended_macro_enodeb_id = htons(extended_macro_enodeb_id);
    os.write(
        reinterpret_cast<const char*>(&ns_extended_macro_enodeb_id),
        sizeof(ns_extended_macro_enodeb_id));
  }
  //--------
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&u2.b), sizeof(u2.b));
    is.read(reinterpret_cast<char*>(&u3.b), sizeof(u3.b));
    is.read(reinterpret_cast<char*>(&u4.b), sizeof(u4.b));
    is.read(
        reinterpret_cast<char*>(&extended_macro_enodeb_id),
        sizeof(extended_macro_enodeb_id));
    extended_macro_enodeb_id = ntohs(extended_macro_enodeb_id);
  }
};

//-------------------------------------
// 8.21 User Location Information (ULI)
class gtpv2c_user_location_information_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t cgi : 1;
      uint8_t sai : 1;
      uint8_t rai : 1;
      uint8_t tai : 1;
      uint8_t ecgi : 1;
      uint8_t lai : 1;
      uint8_t macro_enodeb_id : 1;
      uint8_t extended_macro_enodeb_id : 1;
    } bf;
    uint8_t b;
  } u1;
  gtpv2c_cgi_field_ie cgi;
  gtpv2c_sai_field_ie sai;
  gtpv2c_rai_field_ie rai;
  gtpv2c_tai_field_ie tai;
  gtpv2c_ecgi_field_ie ecgi;
  gtpv2c_lai_field_ie lai;
  gtpv2c_macro_enodeb_id_field_ie macro_enodeb_id;
  gtpv2c_extended_macro_enodeb_id_field_ie extended_macro_enodeb_id;

  //--------
  explicit gtpv2c_user_location_information_ie(const uli_t& u)
      : gtpv2c_ie(GTP_IE_USER_LOCATION_INFORMATION) {
    tlv.length = 1;
    u1.bf.extended_macro_enodeb_id =
        u.user_location_information_ie_hdr.extended_macro_enodeb_id;
    u1.bf.macro_enodeb_id = u.user_location_information_ie_hdr.macro_enodeb_id;
    u1.bf.lai             = u.user_location_information_ie_hdr.lai;
    u1.bf.ecgi            = u.user_location_information_ie_hdr.ecgi;
    u1.bf.tai             = u.user_location_information_ie_hdr.tai;
    u1.bf.rai             = u.user_location_information_ie_hdr.rai;
    u1.bf.sai             = u.user_location_information_ie_hdr.sai;
    u1.bf.cgi             = u.user_location_information_ie_hdr.cgi;

    if (u1.bf.extended_macro_enodeb_id) {
      extended_macro_enodeb_id =
          gtpv2c_extended_macro_enodeb_id_field_ie(u.extended_macro_enodeb_id1);
      tlv.length += 6;
    } else
      extended_macro_enodeb_id = gtpv2c_extended_macro_enodeb_id_field_ie();
    if (u1.bf.macro_enodeb_id) {
      macro_enodeb_id = gtpv2c_macro_enodeb_id_field_ie(u.macro_enodeb_id1);
      tlv.length += 6;
    } else
      macro_enodeb_id = gtpv2c_macro_enodeb_id_field_ie();
    if (u1.bf.lai) {
      lai = gtpv2c_lai_field_ie(u.lai1);
      tlv.length += 4;
    } else
      lai = gtpv2c_lai_field_ie();
    if (u1.bf.ecgi) {
      ecgi = gtpv2c_ecgi_field_ie(u.ecgi1);
      tlv.length += 7;
    } else
      ecgi = gtpv2c_ecgi_field_ie();
    if (u1.bf.tai) {
      tai = gtpv2c_tai_field_ie(u.tai1);
      tlv.length += 5;
    } else
      tai = gtpv2c_tai_field_ie();
    if (u1.bf.rai) {
      rai = gtpv2c_rai_field_ie(u.rai1);
      tlv.length += 5;
    } else
      rai = gtpv2c_rai_field_ie();
    if (u1.bf.sai) {
      sai = gtpv2c_sai_field_ie(u.sai1);
      tlv.length += 7;
    } else
      sai = gtpv2c_sai_field_ie();
    if (u1.bf.cgi) {
      cgi = gtpv2c_cgi_field_ie(u.cgi1);
      tlv.length += 7;
    } else
      cgi = gtpv2c_cgi_field_ie();
  }
  //--------
  gtpv2c_user_location_information_ie()
      : gtpv2c_ie(GTP_IE_USER_LOCATION_INFORMATION) {
    tlv.length               = 1;
    u1.b                     = 0;
    cgi                      = gtpv2c_cgi_field_ie();
    sai                      = gtpv2c_sai_field_ie();
    rai                      = gtpv2c_rai_field_ie();
    tai                      = gtpv2c_tai_field_ie();
    ecgi                     = gtpv2c_ecgi_field_ie();
    lai                      = gtpv2c_lai_field_ie();
    macro_enodeb_id          = gtpv2c_macro_enodeb_id_field_ie();
    extended_macro_enodeb_id = gtpv2c_extended_macro_enodeb_id_field_ie();
  }
  //--------
  explicit gtpv2c_user_location_information_ie(const gtpv2c_tlv& t)
      : gtpv2c_ie(t) {
    u1.b                     = 0;
    cgi                      = gtpv2c_cgi_field_ie();
    sai                      = gtpv2c_sai_field_ie();
    rai                      = gtpv2c_rai_field_ie();
    tai                      = gtpv2c_tai_field_ie();
    ecgi                     = gtpv2c_ecgi_field_ie();
    lai                      = gtpv2c_lai_field_ie();
    macro_enodeb_id          = gtpv2c_macro_enodeb_id_field_ie();
    extended_macro_enodeb_id = gtpv2c_extended_macro_enodeb_id_field_ie();
  };
  explicit gtpv2c_user_location_information_ie(
      const gtpv2c_user_location_information_ie& i)
      : gtpv2c_ie(i),
        u1(i.u1),
        cgi(i.cgi),
        sai(i.sai),
        rai(i.rai),
        tai(i.tai),
        ecgi(i.ecgi),
        lai(i.lai),
        macro_enodeb_id(i.macro_enodeb_id),
        extended_macro_enodeb_id(i.extended_macro_enodeb_id) {}

  //--------
  gtpv2c_user_location_information_ie& operator=(
      gtpv2c_user_location_information_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    std::swap(cgi, other.cgi);
    std::swap(sai, other.sai);
    std::swap(rai, other.rai);
    std::swap(tai, other.tai);
    std::swap(ecgi, other.ecgi);
    std::swap(lai, other.lai);
    std::swap(macro_enodeb_id, other.macro_enodeb_id);
    std::swap(extended_macro_enodeb_id, other.extended_macro_enodeb_id);
    return *this;
  }
  //--------
  void to_core_type(uli_t& u) {
    u = {0};
    u.user_location_information_ie_hdr.extended_macro_enodeb_id =
        u1.bf.extended_macro_enodeb_id;
    u.user_location_information_ie_hdr.macro_enodeb_id = u1.bf.macro_enodeb_id;
    u.user_location_information_ie_hdr.lai             = u1.bf.lai;
    u.user_location_information_ie_hdr.ecgi            = u1.bf.ecgi;
    u.user_location_information_ie_hdr.tai             = u1.bf.tai;
    u.user_location_information_ie_hdr.rai             = u1.bf.rai;
    u.user_location_information_ie_hdr.sai             = u1.bf.sai;
    u.user_location_information_ie_hdr.cgi             = u1.bf.cgi;

    if (u1.bf.extended_macro_enodeb_id) {
      extended_macro_enodeb_id.to_core_type(u.extended_macro_enodeb_id1);
    };
    if (u1.bf.macro_enodeb_id) {
      macro_enodeb_id.to_core_type(u.macro_enodeb_id1);
    }
    if (u1.bf.lai) {
      lai.to_core_type(u.lai1);
    }
    if (u1.bf.ecgi) {
      ecgi.to_core_type(u.ecgi1);
    }
    if (u1.bf.tai) {
      tai.to_core_type(u.tai1);
    }
    if (u1.bf.rai) {
      rai.to_core_type(u.rai1);
    }
    if (u1.bf.sai) {
      sai.to_core_type(u.sai1);
    }
    if (u1.bf.cgi) {
      cgi.to_core_type(u.cgi1);
    }
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    if (u1.bf.extended_macro_enodeb_id) {
      extended_macro_enodeb_id.dump_to(os);
    };
    if (u1.bf.macro_enodeb_id) {
      macro_enodeb_id.dump_to(os);
    }
    if (u1.bf.lai) {
      lai.dump_to(os);
    }
    if (u1.bf.ecgi) {
      ecgi.dump_to(os);
    }
    if (u1.bf.tai) {
      tai.dump_to(os);
    }
    if (u1.bf.rai) {
      rai.dump_to(os);
    }
    if (u1.bf.sai) {
      sai.dump_to(os);
    }
    if (u1.bf.cgi) {
      cgi.dump_to(os);
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    if (u1.bf.extended_macro_enodeb_id) {
      extended_macro_enodeb_id.load_from(is);
    };
    if (u1.bf.macro_enodeb_id) {
      macro_enodeb_id.load_from(is);
    }
    if (u1.bf.lai) {
      lai.load_from(is);
    }
    if (u1.bf.ecgi) {
      ecgi.load_from(is);
    }
    if (u1.bf.tai) {
      tai.load_from(is);
    }
    if (u1.bf.rai) {
      rai.load_from(is);
    }
    if (u1.bf.sai) {
      sai.load_from(is);
    }
    if (u1.bf.cgi) {
      cgi.load_from(is);
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    uli_t uli = {};
    to_core_type(uli);
    s.set(uli, instance);
  }
};
//-------------------------------------
// 8.22 Fully Qualified TEID (F-TEID)
class gtpv2c_fully_qualified_teid_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t interface_type : 6;
      uint8_t v6 : 1;
      uint8_t v4 : 1;
    } bf;
    uint8_t b;
  } u1;
  uint32_t teid_gre_key;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;

  //--------
  explicit gtpv2c_fully_qualified_teid_ie(const fteid_t& f)
      : gtpv2c_ie(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER) {
    tlv.length           = 5;
    u1.b                 = 0;
    u1.bf.v4             = f.v4;
    u1.bf.v6             = f.v6;
    u1.bf.interface_type = f.interface_type;
    teid_gre_key         = f.teid_gre_key;
    ipv4_address         = f.ipv4_address;
    ipv6_address         = f.ipv6_address;
    if (u1.bf.v4) {
      tlv.length += 4;
    }
    if (u1.bf.v6) {
      tlv.length += 16;
    }
  }
  //--------
  gtpv2c_fully_qualified_teid_ie()
      : gtpv2c_ie(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER) {
    tlv.length          = 5;
    u1.b                = 0;
    teid_gre_key        = 0;
    ipv4_address.s_addr = INADDR_ANY;
    ipv6_address        = in6addr_any;
  }
  //--------
  explicit gtpv2c_fully_qualified_teid_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b                = 0;
    teid_gre_key        = 0;
    ipv4_address.s_addr = INADDR_ANY;
    ipv6_address        = in6addr_any;
  };
  //--------
  gtpv2c_fully_qualified_teid_ie& operator=(
      gtpv2c_fully_qualified_teid_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    std::swap(teid_gre_key, other.teid_gre_key);
    std::swap(ipv4_address, other.ipv4_address);
    std::swap(ipv6_address, other.ipv6_address);
    return *this;
  }
  //--------
  void to_core_type(fteid_t& f) {
    f                = {0};
    f.v4             = u1.bf.v4;
    f.v6             = u1.bf.v6;
    f.interface_type = u1.bf.interface_type;
    f.teid_gre_key   = teid_gre_key;
    f.ipv4_address   = ipv4_address;
    f.ipv6_address   = ipv6_address;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    auto nl_teid_gre_key = htonl(teid_gre_key);
    os.write(
        reinterpret_cast<const char*>(&nl_teid_gre_key),
        sizeof(nl_teid_gre_key));
    if (u1.bf.v4) {
      ipv4_address_dump_to(os, ipv4_address);
    }
    if (u1.bf.v6) {
      ipv6_address_dump_to(os, ipv6_address);
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    uint16_t check_length = 5;
    if (u1.bf.v4) check_length += 4;
    if (u1.bf.v6) check_length += 16;
    if (tlv.get_length() != check_length) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&teid_gre_key), sizeof(teid_gre_key));
    teid_gre_key = ntohl(teid_gre_key);
    if (u1.bf.v4) {
      ipv4_address_load_from(is, ipv4_address);
    }
    if (u1.bf.v6) {
      ipv6_address_load_from(is, ipv6_address);
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    fteid_t fteid = {};
    to_core_type(fteid);
    s.set(fteid, instance);
  }
};
//-------------------------------------
// 8.27 Delay Value
class gtpv2c_delay_value_ie : public gtpv2c_ie {
 public:
  uint8_t delay_value;

  //--------
  explicit gtpv2c_delay_value_ie(const delay_value_t& d)
      : gtpv2c_ie(GTP_IE_DELAY_VALUE) {
    tlv.length  = 1;
    delay_value = d.delay_value;
  }
  //--------
  gtpv2c_delay_value_ie() : gtpv2c_ie(GTP_IE_DELAY_VALUE) {
    tlv.length  = 1;
    delay_value = 0;
  }
  //--------
  explicit gtpv2c_delay_value_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    delay_value = 0;
  };
  //--------
  gtpv2c_delay_value_ie& operator=(gtpv2c_delay_value_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(delay_value, other.delay_value);
    return *this;
  }
  //--------
  void to_core_type(delay_value_t& d) {
    d             = {0};
    d.delay_value = delay_value;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&delay_value), sizeof(delay_value));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&delay_value), sizeof(delay_value));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    delay_value_t delay_value = {};
    to_core_type(delay_value);
    s.set(delay_value, instance);
  }
};

//-------------------------------------------------
// 8.44 UE Time Zone

class gtpv2c_ue_time_zone_ie : public gtpv2c_ie {
 public:
  uint8_t time_zone;
  union {
    struct {
      uint8_t daylight_saving_time : 2;
      uint8_t spare1 : 6;
    } bf;
    uint8_t b;
  } u1;

  //--------
  explicit gtpv2c_ue_time_zone_ie(const ue_time_zone_t& t)
      : gtpv2c_ie(GTP_IE_UE_TIME_ZONE) {
    tlv.length                 = 2;
    time_zone                  = t.time_zone;
    u1.bf.spare1               = 0;
    u1.bf.daylight_saving_time = t.daylight_saving_time;
  }
  //--------
  gtpv2c_ue_time_zone_ie() : gtpv2c_ie(GTP_IE_UE_TIME_ZONE) {
    tlv.length = 2;
    time_zone  = 0;
    u1.b       = 0;
  }
  //--------
  explicit gtpv2c_ue_time_zone_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    time_zone = 0;
    u1.b      = 0;
  };
  //--------
  gtpv2c_ue_time_zone_ie& operator=(gtpv2c_ue_time_zone_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    std::swap(time_zone, other.time_zone);
    return *this;
  }
  //--------
  void to_core_type(ue_time_zone_t& t) {
    t                      = {0};
    t.time_zone            = time_zone;
    t.spare1               = 0;
    t.daylight_saving_time = u1.bf.daylight_saving_time;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&time_zone), sizeof(time_zone));
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 2) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&time_zone), sizeof(time_zone));
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    ue_time_zone_t ue_time_zone = {};
    to_core_type(ue_time_zone);
    s.set(ue_time_zone, instance);
  }
};
//-------------------------------------
// 8.57 APN Restriction
class gtpv2c_apn_restriction_ie : public gtpv2c_ie {
 public:
  uint8_t restriction_type_value;

  //--------
  explicit gtpv2c_apn_restriction_ie(const access_point_name_restriction_t& a)
      : gtpv2c_ie(GTP_IE_APN_RESTRICTION) {
    tlv.length             = 1;
    restriction_type_value = a.restriction_type_value;
  }
  //--------
  gtpv2c_apn_restriction_ie() : gtpv2c_ie(GTP_IE_APN_RESTRICTION) {
    tlv.length             = 1;
    restriction_type_value = 0;
  }
  //--------
  explicit gtpv2c_apn_restriction_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    restriction_type_value = 0;
  };
  //--------
  gtpv2c_apn_restriction_ie& operator=(gtpv2c_apn_restriction_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(restriction_type_value, other.restriction_type_value);
    return *this;
  }
  //--------
  void to_core_type(access_point_name_restriction_t& a) {
    a                        = {0};
    a.restriction_type_value = restriction_type_value;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(
        reinterpret_cast<const char*>(&restriction_type_value),
        sizeof(restriction_type_value));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(
        reinterpret_cast<char*>(&restriction_type_value),
        sizeof(restriction_type_value));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    access_point_name_restriction_t access_point_name_restriction = {};
    to_core_type(access_point_name_restriction);
    s.set(access_point_name_restriction, instance);
  }
};
//-------------------------------------
// 8.58 Selection Mode
class gtpv2c_selection_mode_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t selec_mode : 2;
      uint8_t spare1 : 6;
    } bf;
    uint8_t b;
  } u1;

  //--------
  explicit gtpv2c_selection_mode_ie(const selection_mode_t& s)
      : gtpv2c_ie(GTP_IE_SELECTION_MODE) {
    tlv.length       = 1;
    u1.b             = 0;
    u1.bf.selec_mode = s.selec_mode;
  }
  //--------
  gtpv2c_selection_mode_ie() : gtpv2c_ie(GTP_IE_SELECTION_MODE) {
    tlv.length = 1;
    u1.b       = 0;
  }
  //--------
  explicit gtpv2c_selection_mode_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
  };
  //--------
  gtpv2c_selection_mode_ie& operator=(gtpv2c_selection_mode_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    return *this;
  }
  //--------
  void to_core_type(selection_mode_t& s) {
    s            = {0};
    s.selec_mode = u1.bf.selec_mode;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    selection_mode_t selection_mode = {};
    to_core_type(selection_mode);
    s.set(selection_mode, instance);
  }
};

//-------------------------------------
// 8.62 Fully qualified PDN Connection Set Identifier (FQ-CSID)
class gtpv2c_fq_csid_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t number_of_csids : 4;
      uint8_t node_id_type : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    struct in_addr unicast_ipv4;
    struct in6_addr unicast_ipv6;
    struct {
      uint16_t mcc;
      uint16_t mnc;
      uint16_t operator_specific_id;
    } exotic;
  } u2;
  std::vector<uint16_t> pdn_connection_set_identifier;
  //--------
  explicit gtpv2c_fq_csid_ie(const fq_csid_t& f) : gtpv2c_ie(GTP_IE_FQ_CSID) {
    tlv.length            = 1;
    u1.b                  = 0;
    u1.bf.node_id_type    = f.fq_csid_ie_hdr.node_id_type;
    u1.bf.number_of_csids = f.fq_csid_ie_hdr.number_of_csids;
    tlv.length += sizeof(uint16_t) * u1.bf.number_of_csids;
    switch (u1.bf.node_id_type) {
      case GLOBAL_UNICAST_IPv4:
        u2.unicast_ipv4 = f.fq_csid_ie_hdr.node_id.unicast_ipv4;
        tlv.length += 4;
        break;
      case GLOBAL_UNICAST_IPv6:
        u2.unicast_ipv6 = f.fq_csid_ie_hdr.node_id.unicast_ipv6;
        tlv.length += 16;
        break;
      case TYPE_EXOTIC:
        u2.exotic.mcc = f.fq_csid_ie_hdr.node_id.exotic.mcc;
        u2.exotic.mnc = f.fq_csid_ie_hdr.node_id.exotic.mnc;
        u2.exotic.operator_specific_id =
            f.fq_csid_ie_hdr.node_id.exotic.operator_specific_id;
        tlv.length += 4;
        break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_FQ_CSID, "node_id_type");
    }
    for (int i = 0; i < u1.bf.number_of_csids; i++) {
      pdn_connection_set_identifier.push_back(
          f.pdn_connection_set_identifier[i]);
    }
  }
  //--------
  gtpv2c_fq_csid_ie() : gtpv2c_ie(GTP_IE_FQ_CSID) {
    u1.b       = 0;
    tlv.length = 1;
    u2         = {0};
    pdn_connection_set_identifier.reserve(PDN_CONNECTION_SET_IDENTIFIER_MAX);
  }
  //--------
  explicit gtpv2c_fq_csid_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
    u2   = {0};
    pdn_connection_set_identifier.reserve(PDN_CONNECTION_SET_IDENTIFIER_MAX);
  };
  //--------
  gtpv2c_fq_csid_ie& operator=(gtpv2c_fq_csid_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    std::swap(u2, other.u2);
    std::swap(
        pdn_connection_set_identifier, other.pdn_connection_set_identifier);
    return *this;
  }
  //--------
  void to_core_type(fq_csid_t& f) {
    f                                = {0};
    f.fq_csid_ie_hdr.node_id_type    = u1.bf.node_id_type;
    f.fq_csid_ie_hdr.number_of_csids = u1.bf.number_of_csids;
    switch (u1.bf.node_id_type) {
      case GLOBAL_UNICAST_IPv4:
        f.fq_csid_ie_hdr.node_id.unicast_ipv4 = u2.unicast_ipv4;
        break;
      case GLOBAL_UNICAST_IPv6:
        f.fq_csid_ie_hdr.node_id.unicast_ipv6 = u2.unicast_ipv6;
        break;
      case TYPE_EXOTIC:
        f.fq_csid_ie_hdr.node_id.exotic.mcc = u2.exotic.mcc;
        f.fq_csid_ie_hdr.node_id.exotic.mnc = u2.exotic.mnc;
        f.fq_csid_ie_hdr.node_id.exotic.operator_specific_id =
            u2.exotic.operator_specific_id;
        break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_FQ_CSID, "node_id_type");
    }
    int i = 0;
    for (auto& x : pdn_connection_set_identifier) {
      f.pdn_connection_set_identifier[i++] = x;
    }
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    switch (u1.bf.node_id_type) {
      case GLOBAL_UNICAST_IPv4:
        ipv4_address_dump_to(os, u2.unicast_ipv4);
        break;
      case GLOBAL_UNICAST_IPv6:
        ipv6_address_dump_to(os, u2.unicast_ipv6);
        break;
      case TYPE_EXOTIC: {
        uint32_t exo = ((1000 * u2.exotic.mcc + u2.exotic.mnc) << 12) |
                       u2.exotic.operator_specific_id;
        exo = htonl(exo);
        os.write(reinterpret_cast<const char*>(&exo), sizeof(exo));
      } break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_FQ_CSID, "node_id_type");
    }
    for (auto& x : pdn_connection_set_identifier) {
      x = htons(x);
      os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    switch (u1.bf.node_id_type) {
      case GLOBAL_UNICAST_IPv4:
        if (tlv.get_length() < (5 + u1.bf.number_of_csids * 2)) {
          throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
        }
        ipv4_address_load_from(is, u2.unicast_ipv4);
        break;
      case GLOBAL_UNICAST_IPv6:
        if (tlv.get_length() < (17 + u1.bf.number_of_csids * 2)) {
          throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
        }
        ipv6_address_load_from(is, u2.unicast_ipv6);
        break;
      case TYPE_EXOTIC: {
        if (tlv.get_length() < (5 + u1.bf.number_of_csids * 2)) {
          throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
        }
        uint32_t exotic = 0;
        is.read(reinterpret_cast<char*>(&exotic), sizeof(exotic));
        exotic                         = ntohl(exotic);
        u2.exotic.operator_specific_id = exotic & 0x00000FFF;
        exotic                         = exotic >> 12;
        u2.exotic.mnc                  = exotic % 1000;
        u2.exotic.mcc                  = exotic / 1000;
      } break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_FQ_CSID, "node_id_type");
    }
    for (int i = 0; i < u1.bf.number_of_csids; i++) {
      uint16_t x;
      is.read(reinterpret_cast<char*>(&x), sizeof(x));
      x = ntohs(x);
      pdn_connection_set_identifier.push_back(x);
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    fq_csid_t fq_csid = {};
    to_core_type(fq_csid);
    s.set(fq_csid, instance);
  }
};

//-------------------------------------
// 8.65 Node Type
class gtpv2c_node_type_ie : public gtpv2c_ie {
 public:
  uint8_t node_type;

  //--------
  explicit gtpv2c_node_type_ie(const node_type_t& n)
      : gtpv2c_ie(GTP_IE_NODE_TYPE) {
    tlv.length = 1;
    node_type  = n.node_type;
  }
  //--------
  gtpv2c_node_type_ie() : gtpv2c_ie(GTP_IE_NODE_TYPE) {
    tlv.length = 1;
    node_type  = 0;
  }
  //--------
  explicit gtpv2c_node_type_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    node_type = 0;
  };
  //--------
  gtpv2c_node_type_ie& operator=(gtpv2c_node_type_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(node_type, other.node_type);
    return *this;
  }
  //--------
  void to_core_type(node_type_t& n) {
    n           = {0};
    n.node_type = node_type;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&node_type), sizeof(node_type));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&node_type), sizeof(node_type));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    node_type_t node_type = {};
    to_core_type(node_type);
    s.set(node_type, instance);
  }
};

//-------------------------------------
// 8.83 Node Features
class gtpv2c_node_features_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t prn : 1;
      uint8_t mabr : 1;
      uint8_t ntsr : 1;
      uint8_t ciot : 1;
      uint8_t s1un : 1;
      uint8_t spare1 : 3;
    } bf;
    uint8_t b;
  } u1;

  //--------
  explicit gtpv2c_node_features_ie(const node_features_t& s)
      : gtpv2c_ie(GTP_IE_NODE_FEATURES) {
    tlv.length = 1;
    u1.b       = 0;
    u1.bf.prn  = s.prn;
    u1.bf.mabr = s.mabr;
    u1.bf.ntsr = s.ntsr;
    u1.bf.ciot = s.ciot;
    u1.bf.s1un = s.s1un;
  }
  //--------
  gtpv2c_node_features_ie() : gtpv2c_ie(GTP_IE_NODE_FEATURES) {
    tlv.length = 1;
    u1.b       = 0;
  }
  //--------
  explicit gtpv2c_node_features_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t) {
    u1.b = 0;
  };
  //--------
  gtpv2c_node_features_ie& operator=(gtpv2c_node_features_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    return *this;
  }
  //--------
  void to_core_type(node_features_t& s) {
    s      = {0};
    s.prn  = u1.bf.prn;
    s.mabr = u1.bf.mabr;
    s.ntsr = u1.bf.ntsr;
    s.ciot = u1.bf.ciot;
    s.s1un = u1.bf.s1un;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    node_features_t v = {};
    to_core_type(v);
    s.set(v, instance);
  }
};

//-------------------------------------
// 8.103 RAN/NAS Cause
class gtpv2c_ran_nas_cause_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t cause_type : 4;
      uint8_t protocol_type : 4;
    } bf;
    uint8_t b;
  } u1;
  union {
    uint16_t s1ap;  // TODO
    uint8_t emm;
    uint8_t esm;
    uint16_t diameter;
    uint16_t ikev2;
  } cause_value;

  //--------
  explicit gtpv2c_ran_nas_cause_ie(const ran_nas_cause_t& c)
      : gtpv2c_ie(GTP_IE_RAN_NAS_CAUSE) {
    tlv.length          = 1;
    u1.bf.protocol_type = c.protocol_type;
    u1.bf.cause_type    = c.cause_type;
    switch (c.protocol_type) {
      case PROTOCOL_TYPE_E_S1AP:
        cause_value.s1ap = c.cause_value.s1ap;
        tlv.length += sizeof(cause_value.s1ap);
        break;
      case PROTOCOL_TYPE_E_EMM:
        cause_value.emm = c.cause_value.emm;
        tlv.length += sizeof(cause_value.emm);
        break;
      case PROTOCOL_TYPE_E_ESM:
        cause_value.esm = c.cause_value.esm;
        tlv.length += sizeof(cause_value.esm);
        break;
      case PROTOCOL_TYPE_E_DIAMETER:
        cause_value.diameter = c.cause_value.diameter;
        tlv.length += sizeof(cause_value.diameter);
        break;
      case PROTOCOL_TYPE_E_IKEV2:
        cause_value.ikev2 = c.cause_value.ikev2;
        tlv.length += sizeof(cause_value.ikev2);
        break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_RAN_NAS_CAUSE, "protocol_type");
    }
  }
  //--------
  gtpv2c_ran_nas_cause_ie() : gtpv2c_ie(GTP_IE_RAN_NAS_CAUSE), cause_value() {
    tlv.length = 1;
    u1.b       = 0;
  }
  //--------
  gtpv2c_ran_nas_cause_ie(const gtpv2c_ran_nas_cause_ie& i)
      : gtpv2c_ie(i), cause_value(i.cause_value) {
    u1.b = i.u1.b;
  }
  //--------
  explicit gtpv2c_ran_nas_cause_ie(const gtpv2c_tlv& t)
      : gtpv2c_ie(t), u1(), cause_value() {}

  gtpv2c_ran_nas_cause_ie& operator=(gtpv2c_ran_nas_cause_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    std::swap(cause_value, other.cause_value);
    return *this;
  }
  //--------
  void to_core_type(ran_nas_cause_t& c) {
    c               = {0};
    c.protocol_type = u1.bf.protocol_type;
    c.cause_type    = u1.bf.cause_type;
    switch (u1.bf.protocol_type) {
      case PROTOCOL_TYPE_E_S1AP:
        c.cause_value.s1ap = cause_value.s1ap;
        break;
      case PROTOCOL_TYPE_E_EMM:
        c.cause_value.emm = cause_value.emm;
        break;
      case PROTOCOL_TYPE_E_ESM:
        c.cause_value.esm = cause_value.esm;
        break;
      case PROTOCOL_TYPE_E_DIAMETER:
        c.cause_value.diameter = cause_value.diameter;
        break;
      case PROTOCOL_TYPE_E_IKEV2:
        c.cause_value.ikev2 = cause_value.ikev2;
        break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_RAN_NAS_CAUSE, "protocol_type");
    }
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    switch (u1.bf.protocol_type) {
      case PROTOCOL_TYPE_E_S1AP: {
        auto ns_s1ap = htons(cause_value.s1ap);
        os.write(reinterpret_cast<const char*>(&ns_s1ap), sizeof(ns_s1ap));
      } break;
      case PROTOCOL_TYPE_E_EMM:
        os.write(
            reinterpret_cast<const char*>(&cause_value.emm),
            sizeof(cause_value.emm));
        break;
      case PROTOCOL_TYPE_E_ESM:
        os.write(
            reinterpret_cast<const char*>(&cause_value.esm),
            sizeof(cause_value.esm));
        break;
      case PROTOCOL_TYPE_E_DIAMETER: {
        auto ns_diameter = htons(cause_value.diameter);
        os.write(
            reinterpret_cast<const char*>(&ns_diameter), sizeof(ns_diameter));
      } break;
      case PROTOCOL_TYPE_E_IKEV2: {
        auto ns_ikev2 = htons(cause_value.ikev2);
        os.write(reinterpret_cast<const char*>(&ns_ikev2), sizeof(ns_ikev2));
      } break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_RAN_NAS_CAUSE, "protocol_type");
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    switch (u1.bf.protocol_type) {
      case PROTOCOL_TYPE_E_S1AP:
        is.read(
            reinterpret_cast<char*>(&cause_value.s1ap),
            sizeof(cause_value.s1ap));
        cause_value.s1ap = ntohs(cause_value.s1ap);
        break;
      case PROTOCOL_TYPE_E_EMM:
        is.read(
            reinterpret_cast<char*>(&cause_value.emm), sizeof(cause_value.emm));
        break;
      case PROTOCOL_TYPE_E_ESM:
        is.read(
            reinterpret_cast<char*>(&cause_value.esm), sizeof(cause_value.esm));
        break;
      case PROTOCOL_TYPE_E_DIAMETER:
        is.read(
            reinterpret_cast<char*>(&cause_value.diameter),
            sizeof(cause_value.diameter));
        cause_value.diameter = ntohs(cause_value.diameter);
        break;
      case PROTOCOL_TYPE_E_IKEV2:
        is.read(
            reinterpret_cast<char*>(&cause_value.ikev2),
            sizeof(cause_value.ikev2));
        cause_value.ikev2 = ntohs(cause_value.ikev2);
        break;
      default:
        throw gtpc_ie_value_exception(GTP_IE_RAN_NAS_CAUSE, "protocol_type");
    }
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    ran_nas_cause_t ran_nas_cause = {};
    to_core_type(ran_nas_cause);
    s.set(ran_nas_cause, instance);
  }
};

//-------------------------------------
// 8.125 CIoT Optimizations Support Indication
class gtpv2c_ciot_optimizations_support_indication_ie : public gtpv2c_ie {
 public:
  union {
    struct {
      uint8_t sgnipdn : 1;
      uint8_t scnipdn : 1;
      uint8_t awopdn : 1;
      uint8_t ihcsi : 1;
      uint8_t spare1 : 4;
    } bf;
    uint8_t b;
  } u1;

  //--------
  explicit gtpv2c_ciot_optimizations_support_indication_ie(
      const ciot_optimizations_support_indication_t& s)
      : gtpv2c_ie(GTP_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION) {
    tlv.length    = 1;
    u1.b          = 0;
    u1.bf.ihcsi   = s.ihcsi;
    u1.bf.awopdn  = s.awopdn;
    u1.bf.scnipdn = s.scnipdn;
    u1.bf.sgnipdn = s.sgnipdn;
  }
  //--------
  gtpv2c_ciot_optimizations_support_indication_ie()
      : gtpv2c_ie(GTP_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION) {
    tlv.length = 1;
    u1.b       = 0;
  }
  //--------
  explicit gtpv2c_ciot_optimizations_support_indication_ie(const gtpv2c_tlv& t)
      : gtpv2c_ie(t) {
    u1.b = 0;
  };
  //--------
  gtpv2c_ciot_optimizations_support_indication_ie& operator=(
      gtpv2c_ciot_optimizations_support_indication_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(u1, other.u1);
    return *this;
  }
  //--------
  void to_core_type(ciot_optimizations_support_indication_t& s) {
    s         = {0};
    s.ihcsi   = u1.bf.ihcsi;
    s.awopdn  = u1.bf.awopdn;
    s.scnipdn = u1.bf.scnipdn;
    s.sgnipdn = u1.bf.sgnipdn;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() != 1) {
      throw gtpc_tlv_bad_length_exception(tlv.type, tlv.length);
    }
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    ciot_optimizations_support_indication_t
        ciot_optimizations_support_indication = {};
    to_core_type(ciot_optimizations_support_indication);
    s.set(ciot_optimizations_support_indication, instance);
  }
};

//-------------------------------------
// 8.128 Extended Protocol Configuration Options (ePCO)
class gtpv2c_epco_ie : public gtpv2c_ie {
 public:
  std::string extended_protocol_configuration_options;
  //--------
  explicit gtpv2c_epco_ie(const extended_protocol_configuration_options_t& e)
      : gtpv2c_ie(GTP_IE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS),
        extended_protocol_configuration_options(
            e.extended_protocol_configuration_options) {
    tlv.length = extended_protocol_configuration_options.size();
  };
  //--------
  gtpv2c_epco_ie()
      : gtpv2c_ie(GTP_IE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS){};
  //--------
  explicit gtpv2c_epco_ie(const gtpv2c_tlv& t) : gtpv2c_ie(t){};
  //--------
  gtpv2c_epco_ie& operator=(gtpv2c_epco_ie other) {
    this->gtpv2c_ie::operator=(other);
    std::swap(
        extended_protocol_configuration_options,
        other.extended_protocol_configuration_options);
    return *this;
  }
  //--------
  void to_core_type(extended_protocol_configuration_options_t& e) {
    e.extended_protocol_configuration_options =
        extended_protocol_configuration_options;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.length = extended_protocol_configuration_options.size();
    tlv.dump_to(os);
    os << extended_protocol_configuration_options;
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    char e[tlv.length];
    is.read(e, tlv.length);
    extended_protocol_configuration_options.assign(e, tlv.length);
  }
  //--------
  void to_core_type(gtpv2c_ies_container& s, const uint8_t instance) {
    extended_protocol_configuration_options_t
        extended_protocol_configuration_options = {};
    to_core_type(extended_protocol_configuration_options);
    s.set(extended_protocol_configuration_options, instance);
  }
};

}  // namespace gtpv2c

#endif /* FILE_3GPP_29_274_HPP_SEEN */
