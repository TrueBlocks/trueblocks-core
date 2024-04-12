#include "conversions.h"

namespace qblocks {

extern uint64_t verbose;
extern uint64_t hex_2_Uint64(const string_q& str);
extern int64_t hex_2_Int64(const string_q& str) {
    return (int64_t)hex_2_Uint64(str);
}

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

double str_2_Double(const string_q& str) {
    return static_cast<double>(strtold(str.c_str(), NULL));
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
