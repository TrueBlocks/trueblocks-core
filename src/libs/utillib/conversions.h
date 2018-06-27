#pragma once
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
#include <string>
#include <vector>
#include "sfos.h"
#include "biglib.h"
#include "conversions_base.h"

namespace qblocks {

    //--------------------------------------------------------------------
    inline string_q asString(int64_t i) {
        ostringstream os;
        os << i;
        return os.str().c_str();
    }

    //--------------------------------------------------------------------
    inline string_q asStringU(uint64_t i) {
        ostringstream os;
        os << i;
        return os.str().c_str();
    }

    //--------------------------------------------------------------------
    inline string_q padNum2(uint64_t n) { return padLeft(asStringU((n)), 2, '0'); }
    inline string_q padNum3(uint64_t n) { return padLeft(asStringU((n)), 3, '0'); }
    inline string_q padNum4(uint64_t n) { return padLeft(asStringU((n)), 4, '0'); }
    inline string_q padNum5(uint64_t n) { return padLeft(asStringU((n)), 5, '0'); }
    inline string_q padNum6(uint64_t n) { return padLeft(asStringU((n)), 6, '0'); }
    inline string_q padNum7(uint64_t n) { return padLeft(asStringU((n)), 7, '0'); }
    inline string_q padNum8(uint64_t n) { return padLeft(asStringU((n)), 8, '0'); }
    inline string_q padNum9(uint64_t n) { return padLeft(asStringU((n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline string_q padNum3T(uint64_t n) { return padLeft(asStringU((n)), 3); }
    inline string_q padNum4T(uint64_t n) { return padLeft(asStringU((n)), 4); }
    inline string_q padNum5T(uint64_t n) { return padLeft(asStringU((n)), 5); }
    inline string_q padNum6T(uint64_t n) { return padLeft(asStringU((n)), 6); }
    inline string_q padNum7T(uint64_t n) { return padLeft(asStringU((n)), 7); }
    inline string_q padNum8T(uint64_t n) { return padLeft(asStringU((n)), 8); }
    inline string_q padNum9T(uint64_t n) { return padLeft(asStringU((n)), 9); }

    //--------------------------------------------------------------------
    inline string_q padNum2 (int64_t n) { return padLeft(asString((n)), 2, '0'); }
    inline string_q padNum3i(int64_t n) { return padLeft(asString((n)), 3, '0'); }
    inline string_q padNum4 (int64_t n) { return padLeft(asString((n)), 4, '0'); }
    inline string_q padNum5 (int64_t n) { return padLeft(asString((n)), 5, '0'); }
    inline string_q padNum6 (int64_t n) { return padLeft(asString((n)), 6, '0'); }
    inline string_q padNum7 (int64_t n) { return padLeft(asString((n)), 7, '0'); }
    inline string_q padNum8 (int64_t n) { return padLeft(asString((n)), 8, '0'); }
    inline string_q padNum9 (int64_t n) { return padLeft(asString((n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline string_q padNum3T(int64_t n) { return padLeft(asString((n)), 3); }
    inline string_q padNum5T(int64_t n) { return padLeft(asString((n)), 5); }
    inline string_q padNum7T(int64_t n) { return padLeft(asString((n)), 7); }
    inline string_q padNum8T(int64_t n) { return padLeft(asString((n)), 8); }

#define toEther wei2Ether
    inline string_q wei2Ether(const string_q& _value) {
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

    //-------------------------------------------------------------------------
    inline int64_t  toLong   (const string_q& str) { return (int64_t) strtol (str.c_str(), NULL, 10); }

    extern string to_string(const SFUintBN& bu);
    extern string to_hex(const SFUintBN& bu);
    extern string to_string(const SFIntBN&  bi);

    extern SFUintBN hex2BigUint(const string& s);
    extern SFIntBN hex2BigInt(const string& s);
    extern SFUintBN str2BigUint(const string &s);
    #define toBigUint str2BigUint

    extern SFIntBN exp2BigInt(const string& s);
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
    inline SFIntBN str2BigInt(const string &s) {
        return (s[0] == '-') ? SFIntBN(str2BigUint(extract(s, 1, s.length() - 1)), -1)
        : (s[0] == '+') ? SFIntBN(str2BigUint(extract(s, 1, s.length() - 1)))
        : SFIntBN(str2BigUint(s));
    }

    //-------------------------------------------------------------------------
    inline SFUintBN canonicalWei(uint64_t _value) {
        return SFUintBN(_value);
    }

    //--------------------------------------------------------------------------------
    inline SFUintBN canonicalWei(const string_q& _value) {
        if (contains(_value, "0x"))
            return hex2BigUint(extract(_value, 2).c_str());
        if (contains(_value, "e"))
            return exp2BigUint(_value.c_str());
        return str2BigUint(_value);
    }

    //--------------------------------------------------------------------------------
    inline string_q asStringBN(const SFUintBN& bu) {
        return to_string(bu).c_str();
    }

    //--------------------------------------------------------------------------------
    inline string_q asStringBN(const SFIntBN& bn) {
        return to_string(bn).c_str();
    }

#define SFAddress      string_q
#define SFHash         string_q
#define SFWei          SFUintBN
#define SFBloom        SFUintBN
#define SFGas          uint64_t
#define blknum_t       uint64_t
#define txnum_t        uint64_t
    typedef vector<string_q> SFAddressArray;
    typedef vector<SFBloom> SFBloomArray;

#define toHash(a)      toLower(a)
#define toTopic(a)     canonicalWei(a)
#define toBloom(a)     canonicalWei(a)
#define toWei(a)       canonicalWei(a)
#define addr2BN        toWei
#define hex2BN         toWei

#define fromAddress(a)  ((a).empty() ? "0x0" : (a))
#define fromHash(a)     ((a).empty() ? "0x0" : (a))
#define fromWei(a)      to_string((a)).c_str()
#define fromTopic(a)    ("0x" + padLeft(toLower(string_q(to_hex((a)).c_str())), 64, '0'))
#define fromGas(a)      asStringU(a)

    //-------------------------------------------------------------------------
    inline string_q toHex(const string_q& str) {
        if (str == "null")
            return str;
        SFUintBN bn = canonicalWei(str);
        return toLower("0x" + string_q(to_hex(bn).c_str()));
    }

    //-------------------------------------------------------------------------
    inline string_q toHex(uint64_t num) {
        SFUintBN bn = num;
        return toLower("0x" + string_q(to_hex(bn).c_str()));
    }

    extern string_q bloom2Bytes(const SFBloom& bl);
    extern string_q bloom2Bits(const SFBloom& b);

    //----------------------------------------------------------------------------------
    inline string_q fixHash(const string_q& hashIn) {
        string_q ret = startsWith(hashIn, "0x") ? extract(hashIn, 2) : hashIn;
        return "0x" + padLeft(ret, 32 * 2, '0');
    }

    //----------------------------------------------------------------------------------
    inline bool zeroAddr(const SFAddress& addr) {
        return (addr2BN(addr) == 0);
    }

    //----------------------------------------------------------------------------------
    inline string_q fixAddress(const SFAddress& addrIn) {
        string_q ret = startsWith(addrIn, "0x") ? extract(addrIn, 2) : addrIn;
        return "0x" + ret;
    }

    //----------------------------------------------------------------------------------
    inline bool isHexStr(const string_q& str) {
        if (!startsWith(str, "0x"))
            return false;
        for (size_t i = 2 ; i < str.length() ; i++)
            if (!isxdigit(str[i]))
                return false;
        return true;
    }

    //----------------------------------------------------------------------------
    inline bool isNumeral(const string_q& test) {
        for (size_t i = 0 ; i < test.length() ; i++)
            if (!isdigit(test[i]))
                return false;
        return true;
    }

    //----------------------------------------------------------------------------------
    inline bool isAddress(const SFAddress& addrIn) {
        return (addrIn.length() == 42 && isHexStr(addrIn));
    }

    //----------------------------------------------------------------------------------
    inline bool isHash(const SFHash& hashIn) {
        return (hashIn.length() == 66 && isHexStr(hashIn));
    }

    //-------------------------------------------------------------------------
    inline bool isUnsigned(const string_q& in) {
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

    //------------------------------------------------------
    inline SFAddress toAddress(const SFAddress& strIn) {
        // trim it if it's there. We will put it back
        string_q ret = substitute(strIn, "0x", "");

        // Shorten, but only if all leading zeros
        string_q leading('0', 64-40);
        if (ret.length() == 64 && startsWith(ret, leading))
            replace(ret, leading, "");

        // Special case
        if (ret.empty())
            ret = "0";

        return "0x" + toLower(ret);
    }

    //--------------------------------------------------------------------
    inline string_q double2Str(double f, size_t nDecimals = 10) {
        char s[100], r[100];
        memset(s, '\0', 100);
        memset(r, '\0', 100);
        sprintf(s, "%.*g", (int)nDecimals, ((int64_t)(  pow(10, nDecimals) * (  fabs(f) - labs( (int64_t) f )  )  + 0.5)) / pow(10,nDecimals));  // NOLINT
        if (strchr(s, 'e'))
            s[1] = '\0';
        sprintf(r, "%d%s", static_cast<int>(f), s+1);  // NOLINT
        return string_q(r);
    }

    typedef vector<string_q> CStringArray;
    typedef vector<uint64_t> SFUintArray;
    typedef vector<int64_t>  SFIntArray;
    typedef SFArrayBase<SFUintBN> SFBigUintArray;
    typedef vector<SFIntBN> SFBigIntArray;

    #define CBlockNumArray SFUintArray

}  // namespace qblocks
