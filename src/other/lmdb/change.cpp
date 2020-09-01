/*
   Copyright 2020 The Silkworm Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "change.h"

#include <algorithm>
#include <boost/endian/conversion.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <cassert>
#include <cstring>
#include <silkworm/common/util.hpp>
#include <tuple>
#include <vector>

namespace {
using namespace silkworm;

constexpr uint64_t kDefaultIncarnation{1};

// size_t -> uint32_t
constexpr uint32_t kAddressLen{kAddressLength};
constexpr uint32_t kHashLen{kHashLength};

std::tuple<uint32_t, uint32_t> decode_account_meta(ByteView b) {
  using boost::endian::load_big_u32;

  if (b.length() < 4) {
    throw DecodingError("input too short");
  }

  uint32_t n{load_big_u32(&b[0])};

  uint32_t val_pos{4 + n * kAddressLen + 4 * n};
  if (b.length() < val_pos) {
    throw DecodingError("input too short");
  }

  uint32_t total_val_len{load_big_u32(&b[val_pos - 4])};
  if (b.length() < val_pos + total_val_len) {
    throw DecodingError("input too short");
  }

  return {n, val_pos};
}

std::tuple<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t> decode_storage_meta(ByteView b) {
  using boost::endian::load_big_u32;

  if (b.length() < 4) {
    throw DecodingError("input too short");
  }

  uint32_t num_of_contracts{load_big_u32(&b[0])};
  uint32_t pos{num_of_contracts * (kAddressLen + 4)};
  uint32_t num_of_entries{load_big_u32(&b[pos])};
  pos += 4;
  uint32_t num_of_non_default_incarnations{load_big_u32(&b[pos])};
  uint32_t incarnation_pos{pos + 4};
  uint32_t key_pos{incarnation_pos + num_of_non_default_incarnations * 12};
  uint32_t val_pos{key_pos + num_of_entries * kHashLen};

  return {num_of_contracts, num_of_non_default_incarnations, incarnation_pos, key_pos, val_pos};
}

ByteView account_address(ByteView b, uint32_t i) {
  return b.substr(4 + i * kAddressLen, kAddressLen);
};

ByteView storage_address(ByteView b, uint32_t i) {
  return b.substr(4 + i * (4 + kAddressLen), kAddressLen);
};

std::pair<uint32_t, uint32_t> account_indices(ByteView lengths, uint32_t i) {
  using boost::endian::load_big_u32;

  uint32_t idx0{0};
  if (i > 0) {
    idx0 = load_big_u32(&lengths[4 * (i - 1)]);
  }
  uint32_t idx1 = load_big_u32(&lengths[4 * i]);

  return {idx0, idx1};
}

ByteView find_value(ByteView b, uint32_t i) {
  using boost::endian::load_big_u16;
  using boost::endian::load_big_u32;

  uint32_t num_of_uint8{load_big_u32(&b[0])};
  uint32_t num_of_uint16{load_big_u32(&b[4])};
  uint32_t num_of_uint32{load_big_u32(&b[8])};
  b = b.substr(12);
  uint32_t val_pos{num_of_uint8 + num_of_uint16 * 2 + num_of_uint32 * 4};

  auto val_index{[=](uint32_t i) -> uint32_t {
    if (i < num_of_uint8) {
      return b[i];
    } else if (i < num_of_uint8 + num_of_uint16) {
      uint32_t pos{num_of_uint8 + (i - num_of_uint8) * 2};
      return load_big_u16(&b[pos]);
    } else {
      uint32_t pos{num_of_uint8 + num_of_uint16 * 2 + (i - num_of_uint8 - num_of_uint16) * 4};
      return load_big_u32(&b[pos]);
    }
  }};

  uint32_t start{i > 0 ? val_index(i - 1) : 0};
  uint32_t end{val_index(i)};

  return b.substr(val_pos + start, end - start);
}

struct Contract {
  ByteView address;
  uint64_t incarnation{0};
  std::vector<ByteView> keys;
  std::vector<ByteView> vals;
};
}  // namespace

namespace silkworm::db {

AccountChanges AccountChanges::decode(ByteView b) {
  if (b.empty()) {
    return {};
  }

  auto [n, val_pos]{decode_account_meta(b)};
  ByteView lengths{b.substr(4 + n * kAddressLen)};

  AccountChanges changes;
  for (uint32_t i{0}; i < n; ++i) {
    evmc::address key;
    std::memcpy(key.bytes, &b[4 + i * kAddressLen], kAddressLen);
    auto [idx0, idx1]{account_indices(lengths, i)};
    changes[key] = b.substr(val_pos + idx0, idx1 - idx0);
  }
  return changes;
}

std::optional<ByteView> AccountChanges::find(ByteView b, ByteView key) {
  using CI = boost::counting_iterator<uint32_t>;

  assert(key.length() == kAddressLen);

  if (b.empty()) {
    return {};
  }

  auto [n, val_pos]{decode_account_meta(b)};

  uint32_t i{*std::lower_bound(CI(0), CI(n), key, [b](uint32_t i, ByteView address) {
    return account_address(b, i) < address;
  })};

  if (i >= n || account_address(b, i) != key) {
    return {};
  }

  auto [idx0, idx1]{account_indices(b.substr(4 + n * kAddressLen), i)};

  return b.substr(val_pos + idx0, idx1 - idx0);
}

/*
StorageChanges are encoded in the following format:

num_of_contracts uint32
[num_of_contracts]{
  address                   bytes20
  cumulative_num_of_entries uint32
}
num_of_non_default_incarnations uint32
[num_of_non_default_incarnations]{
  contract_idx uint32
  incarnation uint64
}
[num_of_entries]keys   bytes32
num_of_uint8_val_lens  uint32
num_of_uint16_val_lens uint32
num_of_uint32_val_lens uint32
[num_of_uint8_val_lens]  cumulative_val_len uint8
[num_of_uint16_val_lens] cumulative_val_len uint16
[num_of_uint32_val_lens] cumulative_val_len uint32
followed by the values themselves.

All integers are stored as big-endian.
*/
Bytes StorageChanges::encode() const {
  using namespace boost::endian;

  uint32_t num_of_non_default_incarnations{0};
  Bytes non_default_incarnations{};

  uint32_t num_of_uint8_val_lens{0};
  uint32_t num_of_uint16_val_lens{0};
  uint32_t num_of_uint32_val_lens{0};
  uint32_t len_of_vals{0};
  Bytes val_lens{};

  std::vector<Contract> contracts{};

  ByteView prev{};
  for (const auto& entry : *this) {
    ByteView contract_key{entry.first.data(), kAddressLen + kIncarnationLength};
    if (contract_key != prev) {
      uint64_t incarnation{load_big_u64(&contract_key[kAddressLen])};
      if (incarnation != kDefaultIncarnation) {
        size_t pos{non_default_incarnations.size()};
        non_default_incarnations.resize(pos + kIncarnationLength + 4);
        store_big_u32(&non_default_incarnations[pos], static_cast<uint32_t>(contracts.size()));
        store_big_u64(&non_default_incarnations[pos + 4], incarnation);
        ++num_of_non_default_incarnations;
      }
      contracts.push_back({contract_key.substr(0, kAddressLen), incarnation, {}, {}});
      prev = contract_key;
    }
    contracts.back().keys.emplace_back(&entry.first[kAddressLen + kIncarnationLength], kHashLen);
    contracts.back().vals.emplace_back(entry.second);

    len_of_vals += static_cast<uint32_t>(entry.second.length());
    if (len_of_vals < 0x100) {
      val_lens.push_back(static_cast<uint8_t>(len_of_vals));
      ++num_of_uint8_val_lens;
    } else if (len_of_vals < 0x10000) {
      size_t pos{val_lens.size()};
      val_lens.resize(pos + 2);
      store_big_u16(&val_lens[pos], static_cast<uint16_t>(len_of_vals));
      ++num_of_uint16_val_lens;
    } else {
      size_t pos{val_lens.size()};
      val_lens.resize(pos + 4);
      store_big_u32(&val_lens[pos], len_of_vals);
      ++num_of_uint32_val_lens;
    }
  }

  Bytes out(4 + contracts.size() * (kAddressLen + 4) + 4 + non_default_incarnations.length() +
                size() * kHashLen + 3 * 4 + val_lens.length() + len_of_vals,
            '\0');

  store_big_u32(&out[0], static_cast<uint32_t>(contracts.size()));

  size_t pos{4};
  uint32_t num_of_keys{0};

  for (const auto& entry : contracts) {
    std::memcpy(&out[pos], &entry.address[0], kAddressLen);
    pos += kAddressLen;
    num_of_keys += static_cast<uint32_t>(entry.keys.size());
    store_big_u32(&out[pos], num_of_keys);
    pos += 4;
  }

  assert(num_of_keys == size());

  store_big_u32(&out[pos], num_of_non_default_incarnations);
  pos += 4;

  std::memcpy(&out[pos], &non_default_incarnations[0], non_default_incarnations.length());
  pos += non_default_incarnations.length();

  for (const auto& contract : contracts) {
    for (const auto& key : contract.keys) {
      std::memcpy(&out[pos], &key[0], kHashLen);
      pos += kHashLen;
    }
  }

  store_big_u32(&out[pos], num_of_uint8_val_lens);
  pos += 4;
  store_big_u32(&out[pos], num_of_uint16_val_lens);
  pos += 4;
  store_big_u32(&out[pos], num_of_uint32_val_lens);
  pos += 4;

  std::memcpy(&out[pos], &val_lens[0], val_lens.length());
  pos += val_lens.length();

  for (const auto& contract : contracts) {
    for (const auto& val : contract.vals) {
      std::memcpy(&out[pos], val.data(), val.length());
      pos += val.length();
    }
  }

  return out;
}

std::optional<ByteView> StorageChanges::find(ByteView b, ByteView composite_key) {
  using CI = boost::counting_iterator<uint32_t>;
  using boost::endian::load_big_u32;
  using boost::endian::load_big_u64;

  assert(composite_key.length() == kAddressLen + kIncarnationLength + kHashLen);

  if (b.empty()) {
    return {};
  }

  ByteView address{composite_key.substr(0, kAddressLen)};
  uint64_t incarnation{load_big_u64(&composite_key[kAddressLen])};
  ByteView key{composite_key.substr(kAddressLen + kIncarnationLength)};

  auto [num_of_contracts, num_of_non_default_incarnations, incarnation_pos, key_pos,
        val_pos]{decode_storage_meta(b)};

  uint32_t contract_idx{*std::lower_bound(
      CI(0), CI(num_of_contracts), address,
      [b](uint32_t i, ByteView address) { return storage_address(b, i) < address; })};

  if (contract_idx >= num_of_contracts || storage_address(b, contract_idx) != address) {
    return {};
  }

  uint32_t num_of_incarnations{1};
  while (contract_idx + num_of_incarnations < num_of_contracts &&
         storage_address(b, contract_idx + num_of_incarnations) == address) {
    ++num_of_incarnations;
  }

  if (incarnation > 0) {
    bool found{false};

    ByteView inc_view{b.substr(incarnation_pos)};
    auto incarnation_contract_idx{
        [inc_view](uint32_t i) { return load_big_u32(&inc_view[12 * i]); }};

    for (uint32_t i{0}; i < num_of_incarnations; ++i) {
      uint32_t incarnation_idx{
          *std::lower_bound(CI(0), CI(num_of_non_default_incarnations), contract_idx,
                            [&](uint32_t i, uint32_t contract_idx) {
                              return incarnation_contract_idx(i) < contract_idx;
                            })};

      uint64_t found_incarnation{kDefaultIncarnation};
      if (incarnation_idx < num_of_non_default_incarnations &&
          incarnation_contract_idx(incarnation_idx) == contract_idx) {
        found_incarnation = load_big_u64(&inc_view[12 * incarnation_idx + 4]);
      }

      if (found_incarnation == incarnation) {
        found = true;
        break;
      } else {
        ++contract_idx;
      }
    }

    if (!found) {
      return {};
    }
  }

  uint32_t from{0};
  if (contract_idx > 0) {
    from = load_big_u32(&b[contract_idx * (kAddressLen + 4)]);
  }
  uint32_t to{load_big_u32(&b[(contract_idx + 1) * (kAddressLen + 4)])};

  ByteView key_view{b.substr(key_pos)};
  uint32_t key_idx{*std::lower_bound(CI(from), CI(to), key, [key_view](uint32_t i, ByteView key) {
    return key_view.substr(i * kHashLen, kHashLen) < key;
  })};
  if (key_idx == to || key_view.substr(key_idx * kHashLen, kHashLen) != key) {
    return {};
  }

  return find_value(b.substr(val_pos), key_idx);
}
}  // namespace silkworm::db
