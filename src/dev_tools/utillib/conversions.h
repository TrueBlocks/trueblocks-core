#pragma once
#include <string>
#include <vector>
#include "sfos.h"

namespace qblocks {

using CStringArray = vector<string_q>;
using CStringBoolMap = map<string_q, bool>;
using CUintBoolMap = map<uint32_t, bool>;
extern uint64_t str_2_Uint(const string_q& str);
extern double str_2_Double(const string_q& str);
extern int64_t str_2_Ts(const string_q& str);
#define str_2_Enum(en, str) (en) str_2_Uint(str)
extern string_q bool_2_Str_t(bool num);
extern string_q double_2_Str(double f, size_t nDecimals = NOPOS);
extern string_q hex_2_Str(const string_q& inHex, size_t nBytes = NOPOS);
extern string_q hex_2_Pad64(const string_q& inHex);
extern string_q uint_2_Hex(uint64_t num);
extern bool isHexStr(const string_q& str);
extern bool isUnsigned(const string_q& in);
typedef enum { NODIR, PREV, NEXT } direction_t;
extern uint64_t verbose;

}  // namespace qblocks
