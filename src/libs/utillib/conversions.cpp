/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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

// TODO(tjayrush): inline these conversions

namespace qblocks {

    extern uint64_t hex_2_Uint64(const string_q& str);
    extern int64_t  hex_2_Int64(const string_q& str) { return (int64_t)hex_2_Uint64(str); }
    extern SFUintBN exp2BigUint (const string_q &str);

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
    string_q bnu_2_Hex(const SFUintBN& i) {
        return string(BigUnsignedInABase(i, 16));
    }

    //--------------------------------------------------------------------------------
    string_q uint_2_Hex(uint64_t num) {
        SFUintBN bn = num;
        return toLower("0x" + bnu_2_Hex(bn));
    }

    //--------------------------------------------------------------------------------
    string_q wei_2_Ether(const string_q& _value) {
        // Make sure the wei number is at least 18 characters long. Once it is,
        // reverse it, put a decimal at the 18th position, reverse it back,
        // trim leading '0's except the tens digit.
        string_q ret = _value;
        if (ret.length() < 18)
            ret = substitute(padLeft(_value, 18), " ", "0");
        reverse(ret);
        ret = extract(ret, 0, 18) + "." + extract(ret, 18);
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

    //-----------------------------------------------------------------------
    string_q wei_2_Ether(SFUintBN in) {
        string_q ret = wei_2_Ether(bnu_2_Str(in));
        if (contains(ret, "."))
            ret = trimTrailing(ret,'0');
        return trimTrailing(ret,'.');
    }

    //--------------------------------------------------------------------------------
    class bloom_tHex : public BigNumStore<unsigned char> {
    public:
        explicit bloom_tHex(const SFUintBN& numIn);
        string_q str;
    };

    //--------------------------------------------------------------------------------
    string_q bloom_2_Bytes(const bloom_t& bl) {
        if (bl == 0)
            return "0x0";
        bloom_tHex b2(bl);
        return ("0x" + padLeft(toLower(b2.str), 512, '0'));
    }

    //--------------------------------------------------------------------------------
    string_q bloom_2_Bits(const bloom_t& b) {
        string_q ret = substitute(bloom_2_Bytes(b), "0x", "");
        replaceAll(ret, "0", "0000");
        replaceAll(ret, "1", "0001");
        replaceAll(ret, "2", "0010");
        replaceAll(ret, "3", "0011");
        replaceAll(ret, "4", "0100");
        replaceAll(ret, "5", "0101");
        replaceAll(ret, "6", "0110");
        replaceAll(ret, "7", "0111");
        replaceAll(ret, "8", "1000");
        replaceAll(ret, "9", "1001");
        replaceAll(ret, "a", "1010");
        replaceAll(ret, "b", "1011");
        replaceAll(ret, "c", "1100");
        replaceAll(ret, "d", "1101");
        replaceAll(ret, "e", "1110");
        replaceAll(ret, "f", "1111");
        return ret;
    }

    //--------------------------------------------------------------------------------
    bool str_2_Bool(const string_q& str) {
        return static_cast<bool>(str % "true" || str_2_Int(str) != 0);
    }

    //--------------------------------------------------------------------------------
    int64_t str_2_Int(const string_q& str) {
        return (int64_t)(startsWith(str, "0x") ? hex_2_Int64(str) : strtol(str.c_str(), NULL, 10));
    }

    //--------------------------------------------------------------------------------
    uint64_t str_2_Uint(const string_q& str) {
        return (uint64_t)(startsWith(str, "0x") ? hex_2_Uint64(str) : strtoul(str.c_str(), NULL, 10));
    }

    //--------------------------------------------------------------------------------
    string_q str_2_Hex(const string_q& str) {
        if (str == "null")
            return str;
        if (str.empty())
            return "0x0";
        SFUintBN bn = str_2_Wei(str);
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
    SFIntBN str_2_BigInt(const string_q& s) {
        return (s[0] == '-') ? SFIntBN(str_2_BigUint(extract(s, 1, s.length() - 1)), -1)
        : (s[0] == '+') ? SFIntBN(str_2_BigUint(extract(s, 1, s.length() - 1)))
        : SFIntBN(str_2_BigUint(s));
    }

    //--------------------------------------------------------------------------------
    SFUintBN str_2_BigUint(const string_q& str) {
        if (startsWith(str, "0x"))
            return str_2_Wei(str);
        return SFUintBN(BigUnsignedInABase(str, 10));
    }

    //--------------------------------------------------------------------------------
    address_t str_2_Addr(const string_q& str) {
        if (isZeroAddr(str))
            return "0x0";

        string_q ret = substitute(str, "0x", "");
        if (ret.length() == 64) {
            string_q leading('0', 64-40);
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
    SFUintBN str_2_Wei(const string_q& str) {
        if (contains(str, "0x"))
            return SFUintBN(BigUnsignedInABase(extract(str, 2).c_str(), 16));
        if (contains(str, "e"))
            return exp2BigUint(str.c_str());
        return str_2_BigUint(str);
    }

    //--------------------------------------------------------------------------------
    bloom_t str_2_Bloom(const string_q& str) {
        return str_2_Wei(str);
    }

    //--------------------------------------------------------------------------------
    topic_t str_2_Topic(const string_q& str) {
        return str_2_Wei(str);
    }

    //--------------------------------------------------------------------------------
    string_q bool_2_Str(bool num) {
        return int_2_Str(num);
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
        char s[100], r[100];
        memset(s, '\0', 100);
        memset(r, '\0', 100);
        sprintf(s, "%.*g", (int)nDecimals, ((int64_t)(  pow(10, nDecimals) * (  fabs(f) - labs( (int64_t) f )  )  + 0.5)) / pow(10,nDecimals));  // NOLINT
        if (strchr(s, 'e'))
            s[1] = '\0';
        sprintf(r, "%d%s", static_cast<int>(f), s+1);  // NOLINT
        return string_q(r);
    }

    //--------------------------------------------------------------------------------
    string_q bni_2_Str(const SFIntBN& num) {
        return (num.isNegative() ? string("-") : "") + bnu_2_Str(num.getMagnitude());
    }

    //--------------------------------------------------------------------------------
    string_q bnu_2_Str(const SFUintBN& num) {
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
    string_q wei_2_Str(const wei_t& wei) {
        return bnu_2_Str(wei);
    }

    //--------------------------------------------------------------------------------
    string_q bloom_2_Str(const bloom_t& bloom) {
        return bnu_2_Str(bloom);
    }

    //--------------------------------------------------------------------------------
    string_q topic_2_Str(const topic_t& topic) {
        return ("0x" + padLeft(toLower(bnu_2_Hex(topic)), 64, '0'));
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
        for (size_t i = 0 ; i < test.length() ; i++)
            if (!isdigit(test[i]))
                return false;
        return true;
    }

    //--------------------------------------------------------------------------------
    bool isHexStr(const string_q& str) {
        if (!startsWith(str, "0x"))
            return false;
        for (size_t i = 2 ; i < str.length() ; i++)
            if (!isxdigit(str[i]))
                return false;
        return true;
    }

    //--------------------------------------------------------------------------------
    bool isAddress(const address_t& addrIn) {
        return (addrIn.length() == 42 && isHexStr(addrIn));
    }

    //--------------------------------------------------------------------------------
    bool isHash(const hash_t& hashIn) {
        return (hashIn.length() == 66 && isHexStr(hashIn));
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
        time_q  jan1970(1970, 1, 1, 0, 0, 0);
        if (timeIn < jan1970)
            return 0;

        int64_t j70 = jan1970.GetTotalSeconds();
        int64_t t   = timeIn.GetTotalSeconds();
        return (t - j70);
    }

    //--------------------------------------------------------------------------------
    string_q ts_2_Str(timestamp_t ts) {
        return int_2_Str(ts);
    }

    //----------------------------------------------------------------------------------------------------
    time_q ts_2_Date(timestamp_t tsIn) {
        time_t utc = tsIn;
        tm unused;
        struct tm *ret = gmtime_r(&utc, &unused);

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

        const char *s = hex.c_str();
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
    SFUintBN exp2BigUint(const string_q &s) {
        string_q exponent = s.c_str();
        string_q decimals = nextTokenClear(exponent, 'e');
        string_q num = nextTokenClear(decimals, '.');
        uint64_t nD = decimals.length();
        uint64_t e = str_2_Uint(exponent);
        SFUintBN ee = 1;
        uint64_t power = e - nD;
        for (uint64_t i = 0 ; i < power ; i++)
            ee *= 10;
        num += decimals;
        return str_2_BigUint(num) * ee;
    }

    //--------------------------------------------------------------------------------
    bloom_tHex::bloom_tHex(const SFUintBN& numIn) {

        len = 1024;
        allocate(1024);

        SFUintBN x2(numIn);
        unsigned int nDigits = 0;
        while (x2.len != 0) {
            SFUintBN lastDigit(x2);
            lastDigit.divide(16, x2);
            blk[nDigits] = (unsigned char)lastDigit.to_uint();
            nDigits++;
        }
        len = nDigits;

        char s[1024+1];
        memset(s, '\0', sizeof(s));
        for (unsigned int p = 0 ; p < len ; p++) {
            unsigned short c = blk[len-1-p];  // NOLINT
            s[p] = ((c < 10) ? char('0' + c) : char('A' + c - 10));  // NOLINT
        }
        str = s;
    }

}  // namespace qblocks
