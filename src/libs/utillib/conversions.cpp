/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "biglib.h"
#include "conversions.h"
#include "logging.h"

// TODO(tjayrush): inline these conversions

namespace qblocks {

extern uint64_t verbose;
extern uint64_t hex_2_Uint64(const string_q& str);
extern int64_t hex_2_Int64(const string_q& str) {
    return (int64_t)hex_2_Uint64(str);
}
extern biguint_t exp_2_BigUint(const string_q& str);

//--------------------------------------------------------------------------------
string_q chr_2_HexStr(const string_q& str) {
    if (startsWith(str, "0x"))
        return str;
    ostringstream os;
    os << "0x";
    for (auto elem : str)
        os << hex << (unsigned int)elem;
    return os.str();
}

//--------------------------------------------------------------------------------
string_q bnu_2_Hex(const biguint_t& i) {
    return string(BigUnsignedInABase(i, 16));
}

//--------------------------------------------------------------------------------
string_q uint_2_Hex(uint64_t num) {
    biguint_t bn = num;
    return toLower("0x" + bnu_2_Hex(bn));
}

//--------------------------------------------------------------------------------
string_q str_2_Ether(const string_q& _value, uint64_t decimals) {
    // Make sure the wei number is at least 'decimals' characters long. Once it is,
    // reverse it, put a decimal at the 'decimals'th position, reverse it back,
    // trim leading '0's except the tens digit.
    string_q ret = _value;
    if (ret.length() < decimals)
        ret = substitute(padLeft(_value, decimals), " ", "0");
    reverse(ret);
    ret = extract(ret, 0, decimals) + "." + extract(ret, decimals);
    reverse(ret);
    ret = trimLeading(ret, '0');
    if (startsWith(ret, '.'))
        ret = "0" + ret;
    if (contains(ret, "0-")) {
        ret = "-" + substitute(ret, "0-", "0");
    }
    ret = substitute(ret, "-.", "-0.");
    return ret;
}

//--------------------------------------------------------------------------------
string_q byte_2_Bits(uint8_t ch) {
    bitset<8> bits(ch);
    return bits.to_string();
}

//--------------------------------------------------------------------------------
bool str_2_Bool(const string_q& str) {
    return static_cast<bool>(containsI(str, "true") || str_2_Int(str) != 0);
}

//--------------------------------------------------------------------------------
int64_t str_2_Int(const string_q& str) {
    return (int64_t)(isHexStr(str) ? hex_2_Int64(str) : strtol(str.c_str(), NULL, 10));
}

//--------------------------------------------------------------------------------
uint64_t str_2_Uint(const string_q& str) {
    return (uint64_t)(isHexStr(str) ? hex_2_Uint64(str) : strtoul(str.c_str(), NULL, 10));
}

//--------------------------------------------------------------------------------
string_q str_2_Hex(const string_q& str) {
    if (str == "null")
        return str;
    if (str.empty())
        return "0x0";
    biguint_t bn = str_2_Wei(str);
    return toLower("0x" + bnu_2_Hex(bn));
}

//--------------------------------------------------------------------------------
gas_t str_2_Gas(const string_q& str) {
    return str_2_Uint(str);
}

//--------------------------------------------------------------------------------
double str_2_Double(const string_q& str) {
    return static_cast<double>(strtold(str.c_str(), NULL));
}

//--------------------------------------------------------------------------------
inline bigint_t str_2_BigInt_nonhex(const string_q& s) {
    biguint_t val;
    if (s[0] == '-') {
        string_q ss = s.substr(1, s.length());
        val = str_2_BigUint(ss);
        return bigint_t(val, -1);
    } else if (s[0] == '+') {
        string_q ss = s.substr(1, s.length());
        val = str_2_BigUint(ss);
        return bigint_t(val, 1);
    }
    return str_2_BigUint(s);
}

//--------------------------------------------------------------------------------
inline string_q getMax(size_t bits) {
    typedef map<size_t, string_q> sizeMap;
    static sizeMap map;
    if (map.size() == 0) {
        map[256] = "115792089237316195423570985008687907853269984665640564039457584007913129639935";
        map[248] = "452312848583266388373324160190187140051835877600158453279131187530910662655";
        map[240] = "1766847064778384329583297500742918515827483896875618958121606201292619775";
        map[232] = "6901746346790563787434755862277025452451108972170386555162524223799295";
        map[224] = "26959946667150639794667015087019630673637144422540572481103610249215";
        map[216] = "105312291668557186697918027683670432318895095400549111254310977535";
        map[208] = "411376139330301510538742295639337626245683966408394965837152255";
        map[200] = "1606938044258990275541962092341162602522202993782792835301375";
        map[192] = "6277101735386680763835789423207666416102355444464034512895";
        map[184] = "24519928653854221733733552434404946937899825954937634815";
        map[176] = "95780971304118053647396689196894323976171195136475135";
        map[168] = "374144419156711147060143317175368453031918731001855";
        map[160] = "1461501637330902918203684832716283019655932542975";
        map[152] = "5708990770823839524233143877797980545530986495";
        map[144] = "22300745198530623141535718272648361505980415";
        map[136] = "87112285931760246646623899502532662132735";
        map[128] = "340282366920938463463374607431768211455";
        map[120] = "1329227995784915872903807060280344575";
        map[112] = "5192296858534827628530496329220095";
        map[104] = "20282409603651670423947251286015";
        map[96] = "79228162514264337593543950335";
        map[88] = "309485009821345068724781055";
        map[80] = "1208925819614629174706175";
        map[72] = "4722366482869645213695";
        map[64] = "18446744073709551615";
        map[56] = "72057594037927935";
        map[48] = "281474976710655";
        map[40] = "1099511627775";
        map[32] = "4294967295";
        map[24] = "16777215";
        map[16] = "65535";
        map[8] = "255";
    }
    return map[bits];
}

// #define NO_STR_CONVERT_FAST
//--------------------------------------------------------------------------------
bigint_t str_2_BigInt(const string_q& s, size_t bits) {
#ifdef NO_STR_CONVERT_FAST
    return 0;
#else
    if (s.empty() || s == "0x")
        return 0;

    if (!isHexStr(s))
        return str_2_BigInt_nonhex(s);

    biguint_t uValIn = str_2_BigUint(s);

    string_q ss = substitute(s, "0x", "");
    ss = trimLeading(ss, '0');
    size_t len = ss.length();

    if (bits != 257 && len < bits / 4)
        ss = padLeft(ss, bits / 4, '0');
    else
        bits = min((size_t)256, len * 4);

    string_q maxStr = getMax(bits);
    if (maxStr.empty())
        return uValIn;
    bigint_t maxInt = bigint_t(str_2_BigUint(maxStr), 1);
    bigint_t sVal = bigint_t(uValIn, 1);
    if (sVal > (maxInt / 2))       // If it's bigger than half, we have to wrap
        sVal = sVal - maxInt - 1;  // wrap if larger than half of max int256

    return sVal;
#endif
}

//--------------------------------------------------------------------------------
biguint_t str_2_BigUint(const string_q& str, size_t bits) {
#ifdef NO_STR_CONVERT_FAST
    return 0;
#else
    if (str.empty() || str == "0x")
        return 0;

    string_q ss = substitute(str, "0x", "");
    ss = trimLeading(ss, '0');

    biguint_t ret;
    if (isHexStr(str)) {
        size_t lenInBits = ss.length() * 4;
        if (lenInBits > bits && bits != 257) {
            reverse(ss);
            ss = ss.substr(0, bits / 4);
            reverse(ss);
        }
        ret = str_2_Wei("0x" + ss);
    } else {
        ret = biguint_t(BigUnsignedInABase(ss, 10));
    }

    if (bits == 257)
        return ret;

    string_q maxStr = getMax(bits);
    if (maxStr.empty())
        return ret;
    biguint_t maxInt = biguint_t(BigUnsignedInABase(maxStr, 10));

    if (ret > maxInt)  // If it's bigger than the max size, we have to wrap
        ret = (ret % maxInt);

    return ret;
#endif
}

//--------------------------------------------------------------------------------
address_t str_2_Addr(const string_q& str) {
    if (isZeroAddr(str))
        return "0x0";

    string_q ret = substitute(str, "0x", "");
    if (ret.length() == 64) {
        string_q leading(64 - 40, '0');
        if (startsWith(ret, leading))
            replace(ret, leading, "");
    }

    return "0x" + toLower(padLeft(ret, 20 * 2, '0'));
}

//--------------------------------------------------------------------------------
hash_t str_2_Hash(const string_q& str) {
    if (isZeroHash(str))
        return "0x0";
    string_q ret = substitute(str, "0x", "");
    return toLower("0x" + padLeft(ret, 32 * 2, '0'));
}

//--------------------------------------------------------------------------------
biguint_t str_2_Wei(const string_q& str) {
    if (contains(str, "0x"))
        return biguint_t(BigUnsignedInABase(extract(str, 2).c_str(), 16));
    if (contains(str, "e"))
        return exp_2_BigUint(str.c_str());
    return str_2_BigUint(str);
}

//--------------------------------------------------------------------------------
string_q bool_2_Str(bool num) {
    if (!num)
        return "false";
    return "true";
}

//--------------------------------------------------------------------------------
string_q bool_2_Str_t(bool num) {
    if (isApiMode())
        return bool_2_Str(num);
    if (!num)
        return "";
    return "true";
}

//--------------------------------------------------------------------------------
string_q int_2_Str(int64_t i) {
    ostringstream os;
    os << i;
    return os.str();
}

//--------------------------------------------------------------------------------
string_q uint_2_Str(uint64_t i) {
    ostringstream os;
    os << i;
    return os.str();
}

//--------------------------------------------------------------------------------
string_q gas_2_Str(const gas_t& gas) {
    return uint_2_Str(gas);
}

//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
string_q bnu_2_Str(const biguint_t& num) {
    return string(BigUnsignedInABase(num, 10));
}

//--------------------------------------------------------------------------------
string_q addr_2_Str(const address_t& addr) {
    return (addr.empty() ? "0x0" : addr);
}

//--------------------------------------------------------------------------------
string_q hash_2_Str(const hash_t& hash) {
    return (hash.empty() ? "0x0" : hash);
}

//--------------------------------------------------------------------------------
biguint_t topic_2_BigUint(const topic_t& topic) {
    return str_2_Wei(topic);
}

//--------------------------------------------------------------------------------
bool isZeroHash(const hash_t& hash) {
    if (!isNumeral(hash) && !isHexStr(hash))
        return false;
    return (str_2_Wei(hash) == 0);
}

//--------------------------------------------------------------------------------
bool isZeroAddr(const address_t& addr) {
    return isZeroHash(addr);
}

//--------------------------------------------------------------------------------
bool isNumeral(const string_q& test) {
    for (size_t i = 0; i < test.length(); i++)
        if (!isdigit(test[i]))
            return false;
    return true;
}

//--------------------------------------------------------------------------------
bool isDouble(const string_q& test) {
    for (size_t i = 0; i < test.length(); i++)
        if (!isdigit(test[i]) && test[i] != '.')
            return false;
    return true;
}

//--------------------------------------------------------------------------------
bool isHexStr(const string_q& str) {
    if (!startsWith(str, "0x") || str.size() < 2)
        return false;
    for (auto ch : extract(str, 2))
        if (!isxdigit(ch))
            return false;
    return true;
}

//--------------------------------------------------------------------------------
bool isAddress(const string_q& addrIn) {
    return (addrIn.length() == 42 && isHexStr(addrIn));
}

//--------------------------------------------------------------------------------
bool isTimestamp(const string_q& ts) {
    // this is hokey, but it works for our purposes
    return (isNumeral(ts) && str_2_Uint(ts) > 1438315200 && str_2_Uint(ts) <= 2542852800);
}

//--------------------------------------------------------------------------------
bool isDate(const string_q& date) {
    // this is hokey, but it works for our purposes
    if (str_2_Uint(date) < 2015 || str_2_Uint(date) > 2050)
        return false;
    return (containsAny(date, ":- ") && countOf(date, '-'));
}

//--------------------------------------------------------------------------------
bool isHash(const hash_t& hashIn) {
    return (hashIn.length() == 66 && isHexStr(hashIn));
}

//--------------------------------------------------------------------------------
bool isFourByte(const fourbyte_t& fourByteIn) {
    return (fourByteIn.length() == 10 && isHexStr(fourByteIn));
}

//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
timestamp_t str_2_Ts(const string_q& str) {
    return str_2_Int(str);
}

//----------------------------------------------------------------------------------------------------
timestamp_t date_2_Ts(const time_q& timeIn) {
    time_q jan1970(1970, 1, 1, 0, 0, 0);
    if (timeIn < jan1970)
        return 0;

    int64_t j70 = jan1970.GetTotalSeconds();
    int64_t t = timeIn.GetTotalSeconds();
    return (t - j70);
}

//--------------------------------------------------------------------------------
string_q ts_2_Str(timestamp_t ts) {
    return int_2_Str(ts);
}

//----------------------------------------------------------------------------------------------------
time_q ts_2_Date(const timestamp_t& tsIn) {
    time_t utc = tsIn;
    tm unused;
    struct tm* ret = gmtime_r(&utc, &unused);

    char retStr[40];
    strftime(retStr, sizeof(retStr), "%Y-%m-%d %H:%M:%S UTC", ret);

    string_q str = retStr;
    uint32_t y = (uint32_t)str_2_Uint(nextTokenClear(str, '-'));
    uint32_t m = (uint32_t)str_2_Uint(nextTokenClear(str, '-'));
    uint32_t d = (uint32_t)str_2_Uint(nextTokenClear(str, ' '));
    uint32_t h = (uint32_t)str_2_Uint(nextTokenClear(str, ':'));
    uint32_t mn = (uint32_t)str_2_Uint(nextTokenClear(str, ':'));
    uint32_t s = (uint32_t)str_2_Uint(nextTokenClear(str, ' '));
    return time_q(y, m, d, h, mn, s);
}

//
// NOT IN HEADER
//
//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
biguint_t exp_2_BigUint(const string_q& s) {
    string_q exponent = s.c_str();
    string_q decimals = nextTokenClear(exponent, 'e');
    string_q num = nextTokenClear(decimals, '.');
    uint64_t nD = decimals.length();
    uint64_t e = str_2_Uint(exponent);
    biguint_t ee = 1;
    uint64_t power = e - nD;
    for (uint64_t i = 0; i < power; i++)
        ee *= 10;
    num += decimals;
    return str_2_BigUint(num) * ee;
}

//----------------------------------------------------------------
hashbytes_t hash_2_Bytes(const hash_t& hashIn) {
    vector<uint8_t> ret;
    string_q str = substitute(hashIn, "0x", "");
    for (size_t i = 0; i < str.size(); i += 2)
        ret.push_back(hex_2_Ascii(str[i], str[i + 1]));
    return ret;
}

//----------------------------------------------------------------
hash_t bytes_2_Hash(uint8_t const bytes[32]) {
    ostringstream os;
    os << "0x";
    for (size_t i = 0; i < 32; i++)
        os << toLower(padLeft(bnu_2_Hex(bytes[i]), 2, '0'));
    return os.str();
}

//----------------------------------------------------------------
addrbytes_t addr_2_Bytes(const address_t& addrIn) {
    vector<uint8_t> ret;
    string_q str = substitute(addrIn, "0x", "");
    for (size_t i = 0; i < str.size(); i += 2)
        ret.push_back(hex_2_Ascii(str[i], str[i + 1]));
    return ret;
}

//----------------------------------------------------------------
address_t bytes_2_Addr(uint8_t const bytes[20]) {
    ostringstream os;
    os << "0x";
    for (size_t i = 0; i < 20; i++)
        os << toLower(padLeft(bnu_2_Hex(bytes[i]), 2, '0'));
    return os.str();
}

//----------------------------------------------------------------------------
uchar_t hex_2_Ascii(char c1, char c2) {
    uchar_t c;
    c = (uchar_t)((c1 >= 'A' ? ((c1 & 0xDF) - 'A') + 10 : (c1 - '0')));
    c *= 16;
    c = (uchar_t)(c + (c2 >= 'A' ? ((c2 & 0xDF) - 'A') + 10 : (c2 - '0')));
    return c;
}

//---------------------------------------------------------------------------
string_q hex_2_Pad64(const address_t& inHex) {
    return padLeft(substitute(inHex, "0x", ""), 64, '0');
}

//----------------------------------------------------------------------------
string_q hex_2_Str(const string_q& inHex, size_t nBytes) {
    string_q in = (startsWith(inHex, "0x") ? extract(inHex, 2) : inHex);
    if (nBytes != NOPOS)
        in = in.substr(0, nBytes * 2);
    string_q ret;
    while (!in.empty() && in.size() >= 2) {
        string_q nibble = extract(in, 0, 2);
        in = extract(in, 2);
        char ch = static_cast<char>(hex_2_Ascii(nibble[0], nibble[1]));
        if (ch != '\"')
            ret += static_cast<char>(ch);
    }
    return ret;
}

//----------------------------------------------------------------------------
bool rangesIntersect(const blkrange_t& r1, const blkrange_t& r2) {
    return !(r1.second < r2.first || r1.first > r2.second);
}

//--------------------------------------------------------------------------------
timestamp_t date_2_Ts(const string_q& str) {
    return date_2_Ts(str_2_Date(str));
}

//--------------------------------------------------------------------------------
// Date and time values are ordered from the largest to smallest unit of time: year,
// month (or week), day, hour, minute, second, and fraction of second. The lexicographical
// order of the representation thus corresponds to chronological order, except for date
// representations involving negative years. This allows dates to be naturally sorted by,
// for example, file systems.
//
// Each date and time value has a fixed number of digits that must be padded with leading zeros.
//
// Representations can be done in one of two formats – a basic format with a minimal number
// of separators or an extended format with separators added to enhance human readability.
// The standard notes that "The basic format should be avoided in plain text."[15] The
// separator used between date values (year, month, week, and day) is the hyphen, while
// the colon is used as the separator between time values (hours, minutes, and seconds).
// For example, the 6th day of the 1st month of the year 2009 may be written as "2009-01-06"
// in the extended format or simply as "20090106" in the basic format without ambiguity.
//
// For reduced accuracy,[16] any number of values may be dropped from any of the date and
// time representations, but in the order from the least to the most significant. For
// example, "2004-05" is a valid ISO 8601 date, which indicates May (the fifth month)
// 2004. This format will never represent the 5th day of an unspecified month in 2004,
// nor will it represent a time-span extending from 2004 into 2005.
//
// If necessary for a particular application, the standard supports the addition of a
// decimal fraction to the smallest time value in the representation.
//
time_q str_2_Date(const string_q& strIn) {
    if (strIn.empty())
        return earliestDate;

    string_q str = strIn;
    str = nextTokenClear(str, '.');
    string_q check = substitute(str, "UTC", "");
    string_q valid = "0123456789T:-";
    for (auto ch : valid)
        replaceAll(check, string_q(1, ch), "");
    if (!check.empty()) {
        LOG_WARN("str_2_Date: Invalid date string '", strIn, "'");
        return earliestDate;
    }

    replaceAny(str, "T:-", "");
    // clang-format off
    if (str.length() == 4)  str += "0101000000";       // YYYY NOLINT
    else if (str.length() == 6)  str += "01000000";    // YYYYMM
    else if (str.length() == 8)  str += "000000";      // YYYYMMDD
    else if (str.length() == 10) str += "0000";        // YYYYMMDDHH
    else if (str.length() == 12) str += "00";          // YYYYMMDDHHMM
    else if (str.length() == 14) str += "";            // YYYYMMDDHHMMSS
    else { LOG_WARN("str_2_Date: Invalid date string '", strIn, "'"); }  // NOLINT
                                                                         // clang-format off

#define NP ((uint32_t)-1)
#define str_2_Int32u(a) (uint32_t) str_2_Uint((a))
    uint32_t y, m, d, h, mn, s;
    y = m = d = h = mn = s = NP;
    if (isUnsigned(extract(str, 0, 4))) {
        y = str_2_Int32u(extract(str, 0, 4));
    }
    if (isUnsigned(extract(str, 4, 2))) {
        m = str_2_Int32u(extract(str, 4, 2));
    }
    if (isUnsigned(extract(str, 6, 2))) {
        d = str_2_Int32u(extract(str, 6, 2));
    }
    if (isUnsigned(extract(str, 8, 2))) {
        h = str_2_Int32u(extract(str, 8, 2));
    }
    if (isUnsigned(extract(str, 10, 2))) {
        mn = str_2_Int32u(extract(str, 10, 2));
    }
    if (isUnsigned(extract(str, 12, 2))) {
        s = str_2_Int32u(extract(str, 12, 2));
    }
    if (y == NP || m == NP || d == NP || h == NP || mn == NP || s == NP) {
        LOG_WARN("str_2_Date: Invalid date string '", strIn, "'");
        return earliestDate;
    }

    if (m < 1 || m > 12)
        return earliestDate;
    if (d < 1 || d > 31)
        return earliestDate;
    if (h > 23)
        return earliestDate;
    if (mn > 59)
        return earliestDate;
    if (s > 59)
        return earliestDate;

    return time_q(y, m, d, h, mn, s);
}

}  // namespace qblocks
