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

#include "util.hpp"

#include <algorithm>
#include <boost/algorithm/hex.hpp>
#include <cassert>
#include <cstring>
#include <iterator>

namespace silkworm {

ByteView left_pad(ByteView view, size_t min_size) {
  if (view.size() >= min_size) {
    return view;
  }

  thread_local Bytes padded;

  if (padded.size() < min_size) {
    padded.resize(min_size);
  }

  assert(view.size() < min_size);
  size_t prefix_len{min_size - view.size()};

  std::memmove(padded.data() + prefix_len, view.data(), view.size());

  padded.resize(min_size);
  std::fill_n(padded.data(), prefix_len, '\0');

  return padded;
}

ByteView right_pad(ByteView view, size_t min_size) {
  if (view.size() >= min_size) {
    return view;
  }

  thread_local Bytes padded;

  if (padded.size() < view.size()) {
    padded.resize(view.size());
  }

  std::memmove(padded.data(), view.data(), view.size());

  assert(view.size() < min_size);
  padded.resize(view.size());
  padded.resize(min_size);

  return padded;
}

evmc::bytes32 to_hash(ByteView bytes) {
  evmc::bytes32 out;
  size_t n{std::min(bytes.length(), kHashLength)};
  std::memcpy(out.bytes + kHashLength - n, bytes.data(), n);
  return out;
}

ByteView zeroless_view(const evmc::bytes32& hash) {
  unsigned zero_bytes{0};
  while (zero_bytes < kHashLength && hash.bytes[zero_bytes] == 0) {
    ++zero_bytes;
  }
  return {hash.bytes + zero_bytes, kHashLength - zero_bytes};
}

std::string to_hex(const evmc::address& address) { return to_hex(full_view(address)); }

std::string to_hex(const evmc::bytes32& hash) { return to_hex(full_view(hash)); }

std::string to_hex(ByteView bytes) {
  std::string out{};
  out.reserve(2 * bytes.length());
  boost::algorithm::hex_lower(bytes.begin(), bytes.end(), std::back_inserter(out));
  return out;
}

Bytes from_hex(std::string_view hex) {
  if (hex.length() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
    hex.remove_prefix(2);
  }
  Bytes out{};
  out.reserve(hex.length() / 2);
  boost::algorithm::unhex(hex.begin(), hex.end(), std::back_inserter(out));
  return out;
}

size_t prefix_length(ByteView a, ByteView b) {
  size_t len{std::min(a.length(), b.length())};
  for (size_t i{0}; i < len; ++i) {
    if (a[i] != b[i]) {
      return i;
    }
  }
  return len;
}
}  // namespace silkworm
