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

/*! \file smf_paa_dynamic.hpp
 * \brief
 * \author Lionel Gauthier
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_SMF_PAA_DYNAMIC_HPP_SEEN
#define FILE_SMF_PAA_DYNAMIC_HPP_SEEN

#include <bitset>
#include <map>

#include "logger.hpp"
#include "string.hpp"
#include <boost/algorithm/string.hpp>

class ipv6_pool {
 public:
  struct in6_addr prefix;
  int prefix_len;

  ipv6_pool() : prefix(), prefix_len(0) {}

  ipv6_pool(const struct in6_addr prfix, const int prfix_len) {
    prefix     = prfix;
    prefix_len = prfix_len;
  }

  ipv6_pool(const ipv6_pool& p) : prefix(p.prefix), prefix_len(p.prefix_len) {}

  bool alloc_address(struct in6_addr& allocated) {
    allocated = prefix;
    return true;
  }

  void free_address(const struct in_addr& allocated) {}
};

class ipv4_pool {
 protected:
  struct in_addr start;
  int num;
  std::map<int, uint32_t> alloc;

  bool alloc_free_bit(int& bit_pos) {
    bit_pos = 0;
    for (int i = 0; i < alloc.size(); ++i) {
      if (alloc[i] != std::numeric_limits<uint32_t>::max()) {
        std::bitset<32> bs(alloc[i]);
        int word_bit_pos = 0;
        while (bs[word_bit_pos]) {
          bit_pos++;
          word_bit_pos++;
        }
        bs.set(word_bit_pos);
        alloc[i] = bs.to_ulong();
        return true;
      }
      bit_pos += 32;
    }
    return false;
  }

  bool free_bit(const int bit_pos) {
    if (bit_pos < num) {
      int bit_pos32    = bit_pos >> 5;
      int word_bit_pos = bit_pos & 0x0000001F;
      std::bitset<32> bs(alloc[bit_pos32]);
      bs.reset(word_bit_pos);
      alloc[bit_pos32] = bs.to_ulong();
      return true;
    }
    return false;
  }

 public:
  ipv4_pool() : num(0), alloc() { start.s_addr = 0; };

  ipv4_pool(const struct in_addr first, const uint32_t range) : alloc() {
    start.s_addr = first.s_addr;
    num          = range;
    int range32  = range >> 5;
    int i        = 0;
    for (i = 0; i < range32; ++i) {
      alloc[i] = 0;
    }
    if (range & 0x0000001F) {
      alloc[i] = std::numeric_limits<uint32_t>::max() << (range & 0x0000001F);
    }
  };

  ipv4_pool(const ipv4_pool& p) : num(p.num), alloc(p.alloc) {
    start.s_addr = p.start.s_addr;
  };

  bool alloc_address(struct in_addr& allocated) {
    int bit_pos = 0;
    if (alloc_free_bit(bit_pos)) {
      allocated.s_addr = be32toh(start.s_addr) + bit_pos;  // overflow
      allocated.s_addr = htobe32(allocated.s_addr);
      return true;
    }
    allocated.s_addr = 0;
    return false;
  }

  bool free_address(const struct in_addr& allocated) {
    if (in_pool(allocated)) {
      int bit_pos = be32toh(allocated.s_addr) - be32toh(start.s_addr);
      return free_bit(bit_pos);
    }
    return false;
  }

  bool in_pool(const struct in_addr& a) const {
    int addr_start = be32toh(start.s_addr);
    int addr       = be32toh(a.s_addr);
    return ((addr - addr_start) < num);
  }
};

class dnn_dynamic_pools {
 public:
  std::vector<uint32_t> ipv4_pool_ids;
  std::vector<uint32_t> ipv6_pool_ids;

  dnn_dynamic_pools() : ipv4_pool_ids(), ipv6_pool_ids() {}

  void add_ipv4_pool_id(const uint32_t id) { ipv4_pool_ids.push_back(id); }
  void add_ipv6_pool_id(const uint32_t id) { ipv6_pool_ids.push_back(id); }
};

class paa_dynamic {
 private:
  std::map<int32_t, ipv4_pool> ipv4_pools;
  std::map<int32_t, ipv6_pool> ipv6_pools;

  std::map<std::string, dnn_dynamic_pools> dnns;

  mutable std::shared_mutex m_ipv4_pools;

  paa_dynamic() : ipv4_pools(), ipv6_pools(), dnns(), m_ipv4_pools(){};

 public:
  static paa_dynamic& get_instance() {
    static paa_dynamic instance;
    return instance;
  }

  paa_dynamic(paa_dynamic const&) = delete;
  void operator=(paa_dynamic const&) = delete;

  void add_pool(
      const std::string& dnn_label, const int pool_id,
      const struct in_addr& first, const int range) {
    if (pool_id >= 0) {
      uint32_t uint32pool_id = uint32_t(pool_id);
      std::unique_lock lock(m_ipv4_pools);
      if (!ipv4_pools.count(uint32pool_id)) {
        ipv4_pool pool(first, range);
        ipv4_pools[uint32pool_id] = pool;
      }

      dnn_dynamic_pools adp = {};
      if (dnns.count(dnn_label)) {
        adp = dnns[dnn_label];
      }
      adp.add_ipv4_pool_id(uint32pool_id);
      dnns[dnn_label] = adp;
    }
  }

  void add_pool(
      const std::string& dnn_label, const int pool_id,
      const struct in6_addr& prefix, const int prefix_len) {
    if (pool_id >= 0) {
      uint32_t uint32pool_id = uint32_t(pool_id);
      if (!ipv6_pools.count(uint32pool_id)) {
        ipv6_pool pool(prefix, prefix_len);
        ipv6_pools[uint32pool_id] = pool;
      }

      dnn_dynamic_pools adp = {};
      if (dnns.count(dnn_label)) {
        adp = dnns[dnn_label];
      }
      adp.add_ipv6_pool_id(uint32pool_id);
      dnns[dnn_label] = adp;
    }
  }

  bool get_free_paa(const std::string& dnn_label, paa_t& paa) {
    if (dnns.count(dnn_label)) {
      dnn_dynamic_pools& dnn_pool = dnns[dnn_label];
      if (paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV4) {
        std::unique_lock lock(m_ipv4_pools);
        for (std::vector<uint32_t>::const_iterator it4 =
                 dnn_pool.ipv4_pool_ids.begin();
             it4 != dnn_pool.ipv4_pool_ids.end(); ++it4) {
          if (ipv4_pools[*it4].alloc_address(paa.ipv4_address)) {
            return true;
          }
        }
        Logger::smf_app().warn(
            "Could not get PAA PDU_SESSION_TYPE_E_IPV4 for DNN %s",
            dnn_label.c_str());
        return false;
      } else if (
          paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV4V6) {
        bool success                              = false;
        std::vector<uint32_t>::const_iterator it4 = {};
        uint32_t ipv4_pool_id                     = 0;
        for (it4 = dnn_pool.ipv4_pool_ids.begin();
             it4 != dnn_pool.ipv4_pool_ids.end(); ++it4) {
          if (ipv4_pools[*it4].alloc_address(paa.ipv4_address)) {
            success      = true;
            ipv4_pool_id = *it4;
            break;
          }
        }
        if (success) {
          for (std::vector<uint32_t>::const_iterator it6 =
                   dnn_pool.ipv6_pool_ids.begin();
               it6 != dnn_pool.ipv6_pool_ids.end(); ++it6) {
            if (ipv6_pools[*it6].alloc_address(paa.ipv6_address)) {
              return true;
            }
          }
          ipv4_pools[ipv4_pool_id].free_address(paa.ipv4_address);
        }
        Logger::smf_app().warn(
            "Could not get PAA PDU_SESSION_TYPE_E_IPV4V6 for DNN %s",
            dnn_label.c_str());
        return false;
      } else if (
          paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV6) {
        for (std::vector<uint32_t>::const_iterator it6 =
                 dnn_pool.ipv6_pool_ids.begin();
             it6 != dnn_pool.ipv6_pool_ids.end(); ++it6) {
          if (ipv6_pools[*it6].alloc_address(paa.ipv6_address)) {
            return true;
          }
        }
        Logger::smf_app().warn(
            "Could not get PAA PDU_SESSION_TYPE_E_IPV6 for DNN %s",
            dnn_label.c_str());
        return false;
      }
    }
    Logger::smf_app().warn("Could not get PAA for DNN %s", dnn_label.c_str());
    return false;
  }

  bool release_paa(const std::string& dnn_label, const paa_t& paa) {
    if (dnns.count(dnn_label)) {
      dnn_dynamic_pools& dnn_pool = dnns[dnn_label];
      if (paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV4) {
        std::unique_lock lock(m_ipv4_pools);
        for (std::vector<uint32_t>::const_iterator it4 =
                 dnn_pool.ipv4_pool_ids.begin();
             it4 != dnn_pool.ipv4_pool_ids.end(); ++it4) {
          if (ipv4_pools[*it4].free_address(paa.ipv4_address)) {
            return true;
          }
        }
        return false;
      } else if (
          paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV4V6) {
        bool success                              = false;
        std::vector<uint32_t>::const_iterator it4 = {};
        for (it4 = dnn_pool.ipv4_pool_ids.begin();
             it4 != dnn_pool.ipv4_pool_ids.end(); ++it4) {
          if (ipv4_pools[*it4].free_address(paa.ipv4_address)) {
            return true;
          }
        }
        return false;
      } else if (
          paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV6) {
        Logger::smf_app().debug("IPv6 is not fully supported yet!");
        return true;
      }
    }
    Logger::smf_app().warn(
        "Could not release PAA for DNN %s", dnn_label.c_str());
    return false;
  }

  bool release_paa(
      const std::string& dnn_label, const struct in_addr& ipv4_address) {
    std::unique_lock lock(m_ipv4_pools);
    if (dnns.count(dnn_label)) {
      dnn_dynamic_pools& dnn_pool = dnns[dnn_label];
      for (std::vector<uint32_t>::const_iterator it4 =
               dnn_pool.ipv4_pool_ids.begin();
           it4 != dnn_pool.ipv4_pool_ids.end(); ++it4) {
        if (ipv4_pools[*it4].free_address(ipv4_address)) {
          return true;
        }
      }
    }
    Logger::smf_app().warn(
        "Could not release PAA for DNN %s", dnn_label.c_str());
    return false;
  }
};

#endif /* FILE_SMF_PAA_DYNAMIC_HPP_SEEN */
