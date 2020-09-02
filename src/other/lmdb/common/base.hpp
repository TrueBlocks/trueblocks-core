#ifndef SILKWORM_COMMON_BASE_H_
#define SILKWORM_COMMON_BASE_H_

// The most common and basic types and constants.

#include <stddef.h>
#include <stdint.h>

//#include <evmc/evmc.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include "util.hpp"

namespace evmc {
    namespace literals {
        namespace internal {
            constexpr int from_hex(char c) noexcept {
                return (c >= 'a' && c <= 'f') ? c - ('a' - 10) : (c >= 'A' && c <= 'F') ? c - ('A' - 10) : c - '0';
            }
            constexpr uint8_t byte(const char* s, size_t i) noexcept {
                return static_cast<uint8_t>((from_hex(s[2 * i]) << 4) | from_hex(s[2 * i + 1]));
            }
            template <typename T>
            T from_hex(const char*) noexcept;
            template <>
            constexpr bytes32 from_hex<bytes32>(const char* s) noexcept {
                return {{{
                    byte(s, 0),  byte(s, 1),  byte(s, 2),  byte(s, 3),  byte(s, 4),  byte(s, 5),  byte(s, 6), byte(s, 7),
                    byte(s, 8),  byte(s, 9),  byte(s, 10), byte(s, 11), byte(s, 12), byte(s, 13), byte(s, 14), byte(s, 15),
                    byte(s, 16), byte(s, 17), byte(s, 18), byte(s, 19), byte(s, 20), byte(s, 21), byte(s, 22), byte(s, 23),
                    byte(s, 24), byte(s, 25), byte(s, 26), byte(s, 27), byte(s, 28), byte(s, 29), byte(s, 30), byte(s, 31)}}};
                }
            template <>
            constexpr address from_hex<address>(const char* s) noexcept {
                return {{{
                    byte(s, 0),  byte(s, 1),  byte(s, 2),  byte(s, 3),  byte(s, 4),  byte(s, 5),  byte(s, 6), byte(s, 7),
                    byte(s, 8),  byte(s, 9),  byte(s, 10), byte(s, 11), byte(s, 12), byte(s, 13), byte(s, 14), byte(s, 15),
                    byte(s, 16), byte(s, 17), byte(s, 18), byte(s, 19)}}};
                }
            template <typename T, char... c>
            constexpr T from_literal() noexcept {
                constexpr auto size = sizeof...(c);
                constexpr char literal[] = {c...};
                constexpr bool is_simple_zero = size == 1 && literal[0] == '0';
                static_assert(is_simple_zero || (literal[0] == '0' && literal[1] == 'x'),"literal must be in hexadecimal notation");
                static_assert(is_simple_zero || size == 2 * sizeof(T) + 2,"literal must match the result type size");
                return is_simple_zero ? T{} : from_hex<T>(&literal[2]);
            }
        }  // namespace internal
        template <char... c>
        constexpr address operator""_address() noexcept {
            return internal::from_literal<address, c...>();
        }
        template <char... c>
        constexpr bytes32 operator""_bytes32() noexcept {
            return internal::from_literal<bytes32, c...>();
        }
    }  // namespace literals
} // namespace evmc

namespace silkworm {

using namespace evmc::literals;

using Bytes = std::basic_string<uint8_t>;
using ByteView = std::basic_string_view<uint8_t>;

class DecodingError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

//constexpr size_t kAddressLength{20};
//constexpr size_t kHashLength{32};

constexpr size_t kIncarnationLength{8};
static_assert(kIncarnationLength == sizeof(uint64_t));

constexpr evmc::bytes32 kEmptyHash{
    0xc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470_bytes32};

constexpr evmc::bytes32 kEmptyRoot{
    0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421_bytes32};

constexpr uint64_t kGiga{1'000'000'000};   // = 10^9
constexpr uint64_t kEther{kGiga * kGiga};  // = 10^18

}  // namespace silkworm

#endif  // SILKWORM_COMMON_BASE_H_
