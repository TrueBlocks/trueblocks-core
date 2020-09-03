#ifndef SILKWORM_COMMON_UTIL_H_
#define SILKWORM_COMMON_UTIL_H_

//#include "common/base.hpp"
#ifdef _MSC_VER
#include <intrin.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <string>
#include "util.hpp"

typedef struct evmc_bytes32 {
    uint8_t bytes[32];
} evmc_bytes32;

typedef struct evmc_address {
    uint8_t bytes[20];
} evmc_address;

namespace evmc {
    struct address : evmc_address {
        constexpr address(evmc_address init = {}) noexcept : evmc_address{init} {}
        constexpr explicit address(uint64_t v) noexcept
        : evmc_address{{0,0,0,0,0,0,0,0,0,0,0,0,
            static_cast<uint8_t>(v >> 56),static_cast<uint8_t>(v >> 48),
            static_cast<uint8_t>(v >> 40),static_cast<uint8_t>(v >> 32),
            static_cast<uint8_t>(v >> 24),static_cast<uint8_t>(v >> 16),
            static_cast<uint8_t>(v >> 8),static_cast<uint8_t>(v >> 0)}} {}
        constexpr inline explicit operator bool() const noexcept;
    };
    struct bytes32 : evmc_bytes32 {
        constexpr bytes32(evmc_bytes32 init = {}) noexcept : evmc_bytes32{init} {}
        constexpr explicit bytes32(uint64_t v) noexcept
        : evmc_bytes32{{0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
            static_cast<uint8_t>(v >> 56),static_cast<uint8_t>(v >> 48),
            static_cast<uint8_t>(v >> 40),static_cast<uint8_t>(v >> 32),
            static_cast<uint8_t>(v >> 24),static_cast<uint8_t>(v >> 16),
            static_cast<uint8_t>(v >> 8),static_cast<uint8_t>(v >> 0)}} {}
        constexpr inline explicit operator bool() const noexcept;
    };
}

namespace silkworm {
    using uint256be = evmc::bytes32;
    using Bytes = std::basic_string<uint8_t>;
    using ByteView = std::basic_string_view<uint8_t>;
    constexpr size_t kAddressLength{20};
    constexpr size_t kHashLength{32};

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
