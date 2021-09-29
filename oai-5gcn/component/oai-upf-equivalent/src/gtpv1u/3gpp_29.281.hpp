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

/*! \file 3gpp_29.281.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_3GPP_29_281_HPP_SEEN
#define FILE_3GPP_29_281_HPP_SEEN

#include <arpa/inet.h>
#include <cstring>
#include <exception>
//#include <fmt/format.h> conflicts with spdlog
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <sys/socket.h>

#include "3gpp_29.281.h"
#include "msg_gtpv1u.hpp"
#include "logger.hpp"
#include "serializable.hpp"

namespace gtpv1u {

//------------------------------------------------------------------------------
class gtpv1u_tlv : public stream_serializable {
 public:
  static const uint16_t tlv_ie_length = 4;
  uint8_t type;
  uint16_t length;

  gtpv1u_tlv() : type(0), length(0) {}

  gtpv1u_tlv(uint8_t t, uint16_t l = 0) : type(t), length(l) {}

  //~gtpv1u_tlv() {};

  void set_type(const uint8_t& t) { type = t; }

  uint8_t get_type() { return type; }

  void set_length(const uint16_t& l) { length = l; }

  uint16_t get_length() { return length; }

  void dump_to(std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&type), sizeof(type));
    if (type & 0x80) {
      auto be_length = htobe16(length);
      os.write(reinterpret_cast<const char*>(&be_length), sizeof(be_length));
    }
  }
  void load_from(std::istream& is) {
    is.read(reinterpret_cast<char*>(&type), sizeof(type));
    if (type & 0x80) {
      is.read(reinterpret_cast<char*>(&length), sizeof(length));
      length = be16toh(length);
    } else {
      length = 0;
    }
  }
};

//------------------------------------------------------------------------------
class gtpv1u_ie : public stream_serializable {
 public:
  gtpv1u_tlv tlv;

  gtpv1u_ie() : tlv() {}

  explicit gtpv1u_ie(const gtpv1u_tlv& t) : tlv(t) {}

  explicit gtpv1u_ie(const uint8_t tlv_type) : tlv() { tlv.type = tlv_type; }

  virtual ~gtpv1u_ie(){};

  virtual void to_core_type(gtpv1u_ies_container& s) {
    throw gtpu_msg_illegal_ie_exception(s.msg_id, tlv.type);
  }

  virtual void dump_to(std::ostream& os) { tlv.dump_to(os); };

  virtual void load_from(std::istream& is) {
    throw gtpu_ie_unimplemented_exception(
        tlv.type); /* should not hapen of course*/
  };

  static gtpv1u_ie* new_gtpv1u_ie_from_stream(std::istream& is);
};
//------------------------------------------------------------------------------
class gtpv1u_msg_header : public stream_serializable {
 private:
#define GTPV1U_MSG_HEADER_MIN_SIZE 8
  //-----------------------
  // Always present fields:
  //-----------------------
  union {
    struct {
      // N-PDU Number flag (PN): This flag indicates the presence of a
      // meaningful value of the N-PDU Number field. When it is set to '0', the
      // N-PDU Number field either is not present, or, if present, shall not be
      // interpreted. When it is set to '1', the N-PDU Number field is present,
      // and shall be interpreted
      uint8_t pn : 1;
      // Sequence number flag (S): This flag indicates the presence of a
      // meaningful value of the Sequence Number field. When it is set to '0',
      // the Sequence Number field either is not present or, if present, shall
      // not be interpreted. When it is set to '1', the Sequence Number field is
      // present, and shall be interpreted. For the Echo Request, Echo Response,
      // Error Indication and Supported Extension Headers Notification messages,
      // the S flag shall be set to '1'. Since the use of Sequence Numbers is
      // optional for G-PDUs, the PGW, SGW, ePDG, eNodeB and TWAN should set the
      // flag to '0'. However, when a G-PDU (T-PDU+header) is being relayed by
      // the Indirect Data Forwarding for Inter RAT HO procedure, then if the
      // received G-PDU has the S flag set to '1', then the relaying entity
      // shall set S flag to '1' and forward the G-PDU (T-PDU+header). In an End
      // marker message the S flag shall be set to '0'.
      uint8_t s : 1;
      // Extension Header flag (E): This flag indicates the presence of a
      // meaningful value of the Next Extension Header field. When it is set to
      // '0', the Next Extension Header field either is not present or, if
      // present, shall not be interpreted. When it is set to '1', the Next
      // Extension Header field is present, and shall be interpreted
      uint8_t e : 1;
      uint8_t spare : 1;
      // Protocol Type (PT): This bit is used as a protocol discriminator
      // between GTP (when PT is '1') and GTP' (when PT is '0')
      uint8_t pt : 1;
      // Version field: This field is used to determine the version of the GTP-U
      // protocol. The version number shall be set to '1'.
      uint8_t version : 3;
    } bf;
    uint8_t b;
  } u1;
  // Message Type: This field indicates the type of GTP-U message.
  uint8_t message_type;
  // Length: This field indicates the length in octets of the payload, i.e. the
  // rest of the packet following the mandatory part of the GTP header (that is
  // the first 8 octets). The Sequence Number, the N-PDU Number or any Extension
  // headers shall be considered to be part of the payload, i.e. included in the
  // length count.
  uint16_t message_length;
  // Tunnel Endpoint Identifier (TEID): This field unambiguously identifies a
  // tunnel endpoint in the receiving GTP-U protocol entity. The receiving end
  // side of a GTP tunnel locally assigns the TEID value the transmitting side
  // has to use. The TEID value shall be assigned in a non-predictable manner
  // for PGW S5/S8/S2a/S2b interfaces (see 3GPP TS 33.250 [32]). The TEID shall
  // be used by the receiving entity to find the PDP context, except for the
  // following cases:
  // -) The Echo Request/Response and Supported Extension Headers notification
  // messages, where the Tunnel
  //    Endpoint Identifier shall be set to all zeroes
  // -) The Error Indication message where the Tunnel Endpoint Identifier shall
  // be set to all zeros.
  uint32_t teid;
  //------------------
  // Optional fields:
  //------------------
  // Sequence Number: If Sequence Number field is used for G-PDUs
  // (T-PDUs+headers), an increasing sequence number for T-PDUs is transmitted
  // via GTP-U tunnels, when transmission order must be preserved. For Supported
  // Extension Headers Notification and Error Indication messages, the Sequence
  // Number shall be ignored by the receiver, even though the S flag is set to
  // '1'.
  uint16_t sequence_number;
  // N-PDU Number: This field is used at the Inter SGSN Routeing Area Update
  // procedure and some inter-system handover procedures (e.g. between 2G and 3G
  // radio access networks). This field is used to co-ordinate the data
  // transmission for acknowledged mode of communication between the MS and the
  // SGSN. The exact meaning of this field depends upon the scenario. (For
  // example, for GSM/GPRS to GSM/GPRS, the SNDCP N-PDU number is present in
  // this field).
  uint8_t npdu_number;
  // Next Extension Header Type: This field defines the type of Extension Header
  // that follows this field in the GTP-PDU.
  uint8_t next_extension_header_type;

  bool has_teid_;

 public:
  gtpv1u_msg_header() {
    u1.b                       = 0;
    message_type               = 0;
    message_length             = 0;  //
    teid                       = 0;
    sequence_number            = 0;
    npdu_number                = 0;
    next_extension_header_type = 0;
    u1.bf.version              = 1;
    u1.bf.pt                   = 1;
  }

  gtpv1u_msg_header(const gtpv1u_msg_header& h) {
    u1.b                       = h.u1.b;
    message_type               = h.message_type;
    message_length             = h.message_length;
    teid                       = h.teid;
    sequence_number            = h.sequence_number;
    npdu_number                = h.npdu_number;
    next_extension_header_type = h.next_extension_header_type;
    has_teid_                  = h.has_teid_;
  }

  gtpv1u_msg_header& operator=(gtpv1u_msg_header other) {
    std::swap(u1, other.u1);
    std::swap(message_type, other.message_type);
    std::swap(message_length, other.message_length);
    std::swap(teid, other.teid);
    std::swap(sequence_number, other.sequence_number);
    std::swap(npdu_number, other.npdu_number);
    std::swap(next_extension_header_type, other.next_extension_header_type);
    return *this;
  }

  void set_teid(const uint32_t& tid) { teid = tid; }

  uint32_t get_teid() const { return teid; }

  bool get_sequence_number(uint16_t& vsequence_number) const {
    if (u1.bf.s) {  // if inter rat then check in more details
      vsequence_number = sequence_number;
      return true;
    }
    // if (...) {
    //   vsequence_number = 0;
    //  return true;
    // }
    return false;
  }

  void set_message_type(const uint8_t& t) { message_type = t; }

  uint8_t get_message_type() const { return message_type; }

  void set_message_length(const uint16_t& l) { message_length = l; }

  uint16_t get_message_length() const { return message_length; }

  // get payload length without extra header length
  uint16_t get_message_length_wo_xheader() const {
    uint16_t ml = message_length;
    if (u1.bf.s) {
      ml -= sizeof(sequence_number);
    }
    return ml;
  }

  void set_sequence_number(const uint16_t& s) {
    message_length += sizeof(sequence_number);
    sequence_number = s;
    u1.bf.s         = 1;
  }

  uint16_t get_sequence_number() const { return sequence_number; }

  virtual void dump_to(std::ostream& os) {
    u1.bf.spare = 0;
    os.write(reinterpret_cast<const char*>(&u1.b), sizeof(u1.b));
    os.write(
        reinterpret_cast<const char*>(&message_type), sizeof(message_type));
    auto be_message_length = htobe16(message_length);
    os.write(
        reinterpret_cast<const char*>(&be_message_length),
        sizeof(be_message_length));
    auto be_teid = htobe32(teid);
    os.write(reinterpret_cast<const char*>(&be_teid), sizeof(be_teid));

    if (u1.bf.s) {
      auto be_sequence_number = htobe16(sequence_number);
      os.write(
          reinterpret_cast<const char*>(&be_sequence_number),
          sizeof(be_sequence_number));
    }
    if (u1.b & 0x05) {
      os.write(
          reinterpret_cast<const char*>(&npdu_number), sizeof(npdu_number));
      os.write(
          reinterpret_cast<const char*>(&next_extension_header_type),
          sizeof(next_extension_header_type));
    }
  }
  virtual void load_from(std::istream& is) {
    // ! Control Plane GTP header length shall be a multiple of 4 octets.
    is.read(reinterpret_cast<char*>(&u1.b), sizeof(u1.b));
    is.read(reinterpret_cast<char*>(&message_type), sizeof(message_type));
    is.read(reinterpret_cast<char*>(&message_length), sizeof(message_length));
    message_length = be16toh(message_length);
    is.read(reinterpret_cast<char*>(&teid), sizeof(teid));
    teid = be32toh(teid);
    //     if (message_length >= sizeof(teid)){
    //       switch (message_type) {
    //         case GTPU_ECHO_REQUEST:
    //         case GTPU_ECHO_RESPONSE:
    //         case GTPU_ERROR_INDICATION:
    //         case GTPU_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION:
    //           break;
    //         default:;
    //       }
    //     }
    if (u1.bf.s) {
      is.read(
          reinterpret_cast<char*>(&sequence_number), sizeof(sequence_number));
      sequence_number = be16toh(sequence_number);
    }

    if (u1.b & 0x05) {
      is.read(reinterpret_cast<char*>(&npdu_number), sizeof(npdu_number));
      is.read(
          reinterpret_cast<char*>(&next_extension_header_type),
          sizeof(next_extension_header_type));
    }
  }
};
//------------------------------------------------------------------------------
class gtpv1u_msg : public gtpv1u_msg_header {
 public:
  struct sockaddr_storage r_endpoint;
  socklen_t r_endpoint_addr_len;

  std::vector<std::shared_ptr<gtpv1u_ie>> ies;

  gtpv1u_msg()
      : gtpv1u_msg_header(), r_endpoint(), r_endpoint_addr_len(0), ies() {}

  gtpv1u_msg(const gtpv1u_msg& m)
      : gtpv1u_msg_header(m),
        r_endpoint(m.r_endpoint),
        r_endpoint_addr_len(m.r_endpoint_addr_len),
        ies(m.ies) {}

  gtpv1u_msg& operator=(gtpv1u_msg other) {
    std::swap(r_endpoint, other.r_endpoint);
    std::swap(r_endpoint_addr_len, other.r_endpoint_addr_len);
    std::swap(ies, other.ies);
    return *this;
  }

  explicit gtpv1u_msg(const gtpv1u_msg_header& hdr)
      : gtpv1u_msg_header(hdr), r_endpoint(), r_endpoint_addr_len(0), ies() {}
  explicit gtpv1u_msg(const gtpv1u_echo_request& gtp_ies);
  explicit gtpv1u_msg(const gtpv1u_echo_response& gtp_ies);
  explicit gtpv1u_msg(const gtpv1u_error_indication& gtp_ies);
  explicit gtpv1u_msg(
      const gtpv1u_supported_extension_headers_notification& gtp_ies);
  explicit gtpv1u_msg(const gtpv1u_end_marker& gtp_ies);

  ~gtpv1u_msg() { ies.clear(); }

  void add_ie(std::shared_ptr<gtpv1u_ie> ie) {
    ies.push_back(ie);
    // std::cout << std::dec<< " add_ie   = " << get_message_length() << " ->
    // "<< get_message_length() + gtpv1u_tlv::tlv_ie_length +
    // ie.get()->tlv.get_length() << std::endl;
    set_message_length(
        get_message_length() + gtpv1u_tlv::tlv_ie_length +
        ie.get()->tlv.get_length());
  }

  void to_core_type(gtpv1u_echo_request& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s);
    }
  }
  void to_core_type(gtpv1u_echo_response& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s);
    }
  }
  void to_core_type(gtpv1u_error_indication& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s);
    }
  }
  void to_core_type(gtpv1u_supported_extension_headers_notification& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s);
    }
  }
  void to_core_type(gtpv1u_end_marker& s) {
    for (auto i : ies) {
      i.get()->to_core_type(s);
    }
  }

  void dump_to(std::ostream& os) {
    gtpv1u_msg_header::dump_to(os);
    for (auto i : ies) {
      i.get()->dump_to(os);
    }
  }
  void load_from(std::istream& is) {
    gtpv1u_msg_header::load_from(is);

    uint16_t check_msg_length =
        get_message_length_wo_xheader();  // total length of message - fixed
                                          // part of the gtpu header
                                          // (GTPV1U_MSG_HEADER_MIN_SIZE)
    gtpv1u_ie* ie       = nullptr;
    uint16_t ies_length = 0;
    // std::cout << std::dec<< " check_msg_length  = " << check_msg_length <<
    // std::endl;
    do {
      ie = gtpv1u_ie::new_gtpv1u_ie_from_stream(is);
      if (ie) {
        ies_length += (gtpv1u_tlv::tlv_ie_length + ie->tlv.get_length());
        ies.push_back(std::shared_ptr<gtpv1u_ie>(ie));
        // std::cout << std::dec << " ies length  = " << ies_length << " IE
        // length = " << ie->tlv.get_length() << std::endl;
      }
    } while ((ie) && (ies_length < check_msg_length));

    if (ies_length > check_msg_length) {  // should be <= (padding)
      // std::cout << " check_msg_length  = " << check_msg_length << "
      // ies_length  = " << ies_length << std::endl;
      throw gtpu_msg_bad_length_exception(
          get_message_type(), get_message_length());
    }
  }
};

inline void ipv6_address_dump_to(
    std::ostream& os, const struct in6_addr& ipv6_address) {
  os.write(reinterpret_cast<const char*>(ipv6_address.s6_addr), 16);
}

inline void ipv6_address_load_from(
    std::istream& is, struct in6_addr& ipv6_address) {
  is.read(reinterpret_cast<char*>(ipv6_address.s6_addr), 16);
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
class gtpv1u_recovery_ie : public gtpv1u_ie {
 public:
  uint8_t restart_counter;

  //--------
  explicit gtpv1u_recovery_ie(const recovery_t& i)
      : gtpv1u_ie(GTPU_IE_RECOVERY) {
    // avoid using b[]
    restart_counter = i.restart_counter;
    tlv.set_length(1);
  };
  //--------
  explicit gtpv1u_recovery_ie() : gtpv1u_ie(GTPU_IE_RECOVERY) {
    restart_counter = 0;
    tlv.set_length(1);
  };
  //--------
  explicit gtpv1u_recovery_ie(const gtpv1u_tlv& t) : gtpv1u_ie(t) {
    restart_counter = 0;
  };
  //--------
  void to_core_type(recovery_t& i) { i.restart_counter = restart_counter; };

  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    os.write(
        reinterpret_cast<const char*>(&restart_counter),
        sizeof(restart_counter));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(reinterpret_cast<char*>(&restart_counter), sizeof(restart_counter));
  }

  void to_core_type(gtpv1u_ies_container& s) {
    recovery_t v = {};
    to_core_type(v);
    s.set(v);
  }
};
//------------------------------------------------------------------------------
class gtpv1u_tunnel_endpoint_identifier_data_i_ie : public gtpv1u_ie {
 public:
  uint32_t tunnel_endpoint_identifier_data_i;

  //--------
  explicit gtpv1u_tunnel_endpoint_identifier_data_i_ie(
      const tunnel_endpoint_identifier_data_i_t& i)
      : gtpv1u_ie(GTPU_IE_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I) {
    tunnel_endpoint_identifier_data_i = i.tunnel_endpoint_identifier_data_i;
  }
  //--------
  gtpv1u_tunnel_endpoint_identifier_data_i_ie()
      : gtpv1u_ie(GTPU_IE_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I) {
    tunnel_endpoint_identifier_data_i = 0;
  };
  //--------
  explicit gtpv1u_tunnel_endpoint_identifier_data_i_ie(const gtpv1u_tlv& t)
      : gtpv1u_ie(t) {
    tunnel_endpoint_identifier_data_i = 0;
  };
  void to_core_type(tunnel_endpoint_identifier_data_i_t& c) {
    c.tunnel_endpoint_identifier_data_i = tunnel_endpoint_identifier_data_i;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.dump_to(os);
    auto be_tunnel_endpoint_identifier_data_i =
        htobe32(tunnel_endpoint_identifier_data_i);
    os.write(
        reinterpret_cast<const char*>(&be_tunnel_endpoint_identifier_data_i),
        sizeof(be_tunnel_endpoint_identifier_data_i));
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(
        reinterpret_cast<char*>(&tunnel_endpoint_identifier_data_i),
        sizeof(tunnel_endpoint_identifier_data_i));
    tunnel_endpoint_identifier_data_i =
        be32toh(tunnel_endpoint_identifier_data_i);
  }

  void to_core_type(gtpv1u_ies_container& s) {
    tunnel_endpoint_identifier_data_i_t v = {};
    to_core_type(v);
    s.set(v);
  }
};

//------------------------------------------------------------------------------
class gtpv1u_gtp_u_peer_address_ie : public gtpv1u_ie {
 public:
  bool is_v4;
  struct in6_addr ipv6_address;
  struct in_addr ipv4_address;
  //--------
  explicit gtpv1u_gtp_u_peer_address_ie(const gtp_u_peer_address_t& p)
      : gtpv1u_ie(GTPU_IE_GTP_U_PEER_ADDRESS) {
    is_v4 = p.is_v4;
    if (is_v4) {
      ipv4_address = p.ipv4_address;
      tlv.set_length(4);
    } else {
      ipv6_address = p.ipv6_address;
      tlv.set_length(16);
    }
  }
  //--------
  gtpv1u_gtp_u_peer_address_ie() : gtpv1u_ie(GTPU_IE_GTP_U_PEER_ADDRESS) {
    is_v4               = true;
    ipv4_address.s_addr = INADDR_ANY;
    ipv6_address        = in6addr_any;
    tlv.set_length(4);
  }
  //--------
  explicit gtpv1u_gtp_u_peer_address_ie(const gtpv1u_tlv& t) : gtpv1u_ie(t) {
    is_v4               = true;
    ipv4_address.s_addr = INADDR_ANY;
    ipv6_address        = in6addr_any;
  };
  //--------
  void to_core_type(gtp_u_peer_address_t& p) {
    if (is_v4) {
      p.ipv4_address = ipv4_address;
    } else {
      p.ipv6_address = ipv6_address;
    }
  }
  //--------
  void dump_to(std::ostream& os) {
    if (is_v4) {
      tlv.set_length(4);
    } else {
      tlv.set_length(16);
    }
    tlv.dump_to(os);
    if (is_v4) {
      ipv4_address_dump_to(os, ipv4_address);
    } else {
      ipv6_address_dump_to(os, ipv6_address);
    }
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    if (tlv.get_length() == 4) {
      ipv4_address_load_from(is, ipv4_address);
    } else if (tlv.get_length() == 16) {
      ipv6_address_load_from(is, ipv6_address);
    } else {
      throw gtpu_tlv_bad_length_exception(
          GTPU_IE_GTP_U_PEER_ADDRESS, tlv.length);
    }
  }
  //--------
  void to_core_type(gtpv1u_ies_container& s) {
    gtp_u_peer_address_t v = {};
    to_core_type(v);
    s.set(v);
  }
};
//------------------------------------------------------------------------------
class gtpv1u_private_extension_ie : public gtpv1u_ie {
 public:
  uint16_t extension_identifier;
  std::string extension_value;

  //--------
  explicit gtpv1u_private_extension_ie(const private_extension_t& i)
      : gtpv1u_ie(GTPU_IE_PRIVATE_EXTENSION),
        extension_identifier(i.enterprise_id),
        extension_value(i.proprietary_value) {
    tlv.set_length(sizeof(extension_identifier) + extension_value.size());
  }
  //--------
  gtpv1u_private_extension_ie()
      : gtpv1u_ie(GTPU_IE_PRIVATE_EXTENSION),
        extension_identifier(0),
        extension_value() {
    tlv.set_length(sizeof(extension_identifier));
  };
  //--------
  explicit gtpv1u_private_extension_ie(const gtpv1u_tlv& t)
      : gtpv1u_ie(t), extension_identifier(0), extension_value() {}

  void to_core_type(private_extension_t& c) {
    c.enterprise_id     = extension_identifier;
    c.proprietary_value = extension_value;
  }
  //--------
  void dump_to(std::ostream& os) {
    tlv.set_length(sizeof(extension_identifier) + extension_value.size());
    tlv.dump_to(os);
    auto be_extension_identifier = htobe16(extension_identifier);
    os.write(
        reinterpret_cast<const char*>(&be_extension_identifier),
        sizeof(be_extension_identifier));
    os << extension_value;
  }
  //--------
  void load_from(std::istream& is) {
    // tlv.load_from(is);
    is.read(
        reinterpret_cast<char*>(&extension_identifier),
        sizeof(extension_identifier));
    extension_identifier = be16toh(extension_identifier);

    uint16_t length = tlv.get_length();
    if (length > 2) {
      char apn[length - 2];
      is.read(apn, length - 2);
      extension_value.assign(apn, length - 2);
    } else {
      extension_value = {};
    }
  }

  void to_core_type(gtpv1u_ies_container& s) {
    private_extension_t v = {};
    to_core_type(v);
    s.set(v);
  }
};

}  // namespace gtpv1u

#endif /* FILE_3GPP_29_281_HPP_SEEN */
