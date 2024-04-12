#pragma once
#include <string>
#include <vector>
#include "sfos.h"

namespace qblocks {

using CStringArray = vector<string_q>;
using CStringBoolMap = map<string_q, bool>;
using CUintBoolMap = map<uint32_t, bool>;
extern bool str_2_Bool(const string_q& str);
extern int64_t str_2_Int(const string_q& str);
extern uint64_t str_2_Uint(const string_q& str);
extern double str_2_Double(const string_q& str);
extern int64_t str_2_Ts(const string_q& str);
#define str_2_Enum(en, str) (en) str_2_Uint(str)
extern string_q bool_2_Str(bool num);
extern string_q bool_2_Str_t(bool num);
extern string_q double_2_Str(double f, size_t nDecimals = NOPOS);
extern string_q hex_2_Str(const string_q& inHex, size_t nBytes = NOPOS);
extern string_q hex_2_Pad64(const string_q& inHex);
extern string_q chr_2_HexStr(const string_q& str);
extern string_q uint_2_Hex(uint64_t num);
extern string_q str_2_Ether(const string_q& str, uint64_t decimals);
extern bool isNumeral(const string_q& test);
extern bool isDouble(const string_q& test);
extern bool isHexStr(const string_q& str);
extern bool isAddress(const string_q& addr);
extern bool isUnsigned(const string_q& in);
typedef enum { NODIR, PREV, NEXT } direction_t;
inline string_q getEnvStr(const string_q& name) {
    char* sss = getenv(name.c_str());
    return (sss ? string_q(sss) : string_q(""));
}
inline uint64_t getEnvUint(const string_q& name) {
    char* sss = getenv(name.c_str());
    return (sss ? str_2_Uint(sss) : NOPOS);
}
inline bool isTestMode(void) {
    static uint64_t test_mode = NOPOS;
    if (test_mode == NOPOS)
        test_mode = getEnvStr("TEST_MODE") == "true";
    return test_mode;
}
inline bool isApiMode(void) {
    static uint64_t api_mode = NOPOS;
    if (api_mode == NOPOS)
        api_mode = getEnvStr("CPP_API_MODE") == "true";
    return api_mode;
}
extern uint64_t verbose;

}  // namespace qblocks
