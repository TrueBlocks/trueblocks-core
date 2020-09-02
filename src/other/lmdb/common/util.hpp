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

#ifndef SILKWORM_COMMON_UTIL_H_
#define SILKWORM_COMMON_UTIL_H_

//#include "common/base.hpp"
#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace silkworm {

using uint256be = bytes32;

// If a given view is shorter than min_size,
// pads it to the left with 0s up to min_size.
// Otherwise returns unmodified view.
//
// Might return a view of a thread-local buffer,
// which must be consumed prior to the next invocation.
// However, the same view may be padded repeatedly.
ByteView left_pad(ByteView view, size_t min_size);

// If a given view is shorter than min_size,
// pads it to the right with 0s up to min_size.
// Otherwise returns unmodified view.
//
// Might return a view of a thread-local buffer,
// which must be consumed prior to the next invocation.
// However, the same view may be padded repeatedly.
ByteView right_pad(ByteView view, size_t min_size);

// Converts bytes to hash; input is cropped if necessary.
// Short inputs are left-padded with 0s.
evmc::bytes32 to_hash(ByteView bytes);

template <unsigned N>
ByteView full_view(const uint8_t (&bytes)[N]) {
  return {bytes, N};
}

inline ByteView full_view(const evmc::address& address) { return {address.bytes, kAddressLength}; }
inline ByteView full_view(const evmc::bytes32& hash) { return {hash.bytes, kHashLength}; }

// Leading zero bytes are stripped
ByteView zeroless_view(const evmc::bytes32& hash);

std::string to_hex(const evmc::address& address);
std::string to_hex(const evmc::bytes32& hash);
std::string to_hex(ByteView bytes);

Bytes from_hex(std::string_view hex);

// TODO[C++20] replace by starts_with
inline bool has_prefix(ByteView s, ByteView prefix) { return s.substr(0, prefix.size()) == prefix; }

// The length of the longest common prefix of a and b.
size_t prefix_length(ByteView a, ByteView b);

// TODO[C++20] replace by std::popcount
inline int popcount(unsigned x) {
#ifdef _MSC_VER
  return __popcnt(x);
#else
  return __builtin_popcount(x);
#endif
}
}  // namespace silkworm

#endif  // SILKWORM_COMMON_UTIL_H_
