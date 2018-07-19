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

namespace qblocks {

    extern uint64_t hex_2_Uint64(const string_q& str);

    // TODO(tjayrush): inline these conversions
    //----------------------------------------------------------------------------
    double str2Double(const string_q& str) {
        return static_cast<double>(strtold(str.c_str(), NULL));
    }

    //----------------------------------------------------------------------------
    bool str2Bool(const string_q& str) {
        return static_cast<bool>(str % "true" || toLong(str) != 0);
    }

    //----------------------------------------------------------------------------------------------------
    timestamp_t toTimestamp(const string_q& str) {
        return (timestamp_t)toLongU(str);
    }

    //----------------------------------------------------------------------------
    uint64_t toLongU(const string_q& str) {
        return (uint64_t)(startsWith(str, "0x") ? hex_2_Uint64(str) : strtoul(str.c_str(), NULL, 10));
    }

    //----------------------------------------------------------------------------
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

    //----------------------------------------------------------------------------------------------------
    string_q fromTimestamp(timestamp_t ts) {
        return asString(ts);
    }

    //----------------------------------------------------------------------------
    string_q str_2_Hex(const string_q& str) {

        if (startsWith(str, "0x"))
            return str.c_str();

        if (str.empty())
            return "0x";

        string_q ret;
        for (size_t i = 0 ; i < str.length() ; i++) {
            ostringstream os;
            os << hex << (unsigned int)str[i];
            ret = (ret + os.str().c_str());
        }
        return ("0x" + ret);
    }

    //--------------------------------------------------------------------------------
    inline SFUintBN exp2BigUint(const string &s) {
        string_q exponent = s.c_str();
        string_q decimals = nextTokenClear(exponent, 'e');
        string_q num = nextTokenClear(decimals, '.');
        uint64_t nD = decimals.length();
        uint64_t e = toLongU(exponent);
        SFUintBN ee = 1;
        uint64_t power = e - nD;
        for (uint64_t i = 0 ; i < power ; i++)
            ee *= 10;
        num += decimals;
        return str2BigUint(num) * ee;
    }

    //--------------------------------------------------------------------------------
    SFUintBN canonicalWei(const string_q& str) {
        if (contains(str, "0x"))
            return hex2BigUint(extract(str, 2).c_str());
        if (contains(str, "e"))
            return exp2BigUint(str.c_str());
        return str2BigUint(str);
    }

    //------------------------------------------------------------------
    class SFBloomHex : public SFBigNumStore<unsigned char> {
    public:
        explicit SFBloomHex(const SFUintBN& numIn);
        string_q str;
    };

    //------------------------------------------------------------------
    SFBloomHex::SFBloomHex(const SFUintBN& numIn) {

        len = 1024;
        allocate(1024);

        SFUintBN x2(numIn);
        unsigned int nDigits = 0;
        while (x2.len != 0) {
            SFUintBN lastDigit(x2);
            lastDigit.divide(16, x2);
            blk[nDigits] = (unsigned char)lastDigit.to_ushort();
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

    //------------------------------------------------------------------
    string_q bloom2Bytes(const SFBloom& bl) {
        if (bl == 0)
            return "0x0";
        SFBloomHex b2(bl);
        return ("0x" + padLeft(toLower(b2.str), 512, '0'));
    }

    //-------------------------------------------------------------------------
    string_q bloom2Bits(const SFBloom& b) {
        string_q ret = substitute(bloom2Bytes(b), "0x", "");
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
}  // namespace qblocks
