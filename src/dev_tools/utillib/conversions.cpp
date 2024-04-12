#include "conversions.h"

namespace qblocks {

extern uint64_t verbose;
extern uint64_t hex_2_Uint64(const string_q& str);
extern int64_t hex_2_Int64(const string_q& str) {
    return (int64_t)hex_2_Uint64(str);
}

uint64_t str_2_Uint(const string_q& str) {
    return (uint64_t)(isHexStr(str) ? hex_2_Uint64(str) : strtoul(str.c_str(), NULL, 10));
}

double str_2_Double(const string_q& str) {
    return static_cast<double>(strtold(str.c_str(), NULL));
}

string_q double_2_Str(double f, size_t nDecimals) {
    // if no nDecimals specified, default to 10 with trailing zero trunc cation
    bool trunc = false;
    if (nDecimals == NOPOS) {
        nDecimals = 10;
        trunc = true;
    }

    stringstream stream;
    stream << fixed << setprecision(static_cast<int>(nDecimals)) << f;
    string_q str = stream.str();
    if (trunc) {
        str.erase(str.find_last_not_of('0') + 1, string_q::npos);
        // if all decimals gone, trunc period
        str.erase(str.find_last_not_of('.') + 1, string_q::npos);
    }
    return str;
}

bool isHexStr(const string_q& str) {
    if (!startsWith(str, "0x") || str.size() < 2)
        return false;
    for (auto ch : extract(str, 2))
        if (!isxdigit(ch))
            return false;
    return true;
}

bool isUnsigned(const string_q& in) {
    // Empty string is not valid...
    if (in.empty())
        return false;
    // ...first character must be 0-9 (no negatives)...
    if (!isdigit(in[0]))
        return false;
    // ...or first two must be '0x' and the third must be non negative hex digit
    if (startsWith(in, "0x") && in.length() > 2)
        return isxdigit(in.at(2));
    return true;
}

uint64_t hex_2_Uint64(const string_q& str) {
    string_q hex = toLower(startsWith(str, "0x") ? extract(str, 2) : str);
    reverse(hex);

    const char* s = hex.c_str();
    uint64_t ret = 0, mult = 1;
    while (*s) {
        int val = *s - '0';
        if (*s >= 'a' && *s <= 'f')
            val = *s - 'a' + 10;
        ret += (mult * (uint64_t)val);
        s++;
        mult *= 16;
    }

    return ret;
}

uint64_t verbose = false;

}  // namespace qblocks
