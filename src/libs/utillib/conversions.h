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

namespace qblocks {

#define toTopic(a)      canonicalWei((a))
#define toBloom(a)      canonicalWei((a))
#define toWei(a)        canonicalWei((a))
#define hex2BN(a)       canonicalWei((a))

#define fromAddress(a)  ((a).empty() ? "0x0" : (a))
#define fromHash(a)     ((a).empty() ? "0x0" : (a))
#define fromWei(a)      to_string((a)).c_str()
#define fromTopic(a)    ("0x" + padLeft(toLower(string_q(bnu_2_Hex((a)).c_str())), 64, '0'))
#define fromGas(a)      toStringU(a)

    //-------------------------------------------------------------------------
    using timestamp_t = int64_t;
    using blknum_t    = uint64_t;
    using txnum_t     = uint64_t;
    using SFGas       = uint64_t;
    using SFAddress   = string_q;
    using SFHash      = string_q;
    using SFBloom     = SFUintBN;
    using SFWei       = SFUintBN;

    //-------------------------------------------------------------------------
    using CStringArray   = vector < string_q  >;
    using CBlockNumArray = vector < uint64_t  >;
    using SFUintArray    = vector < uint64_t  >;
    using SFIntArray     = vector < int64_t   >;
    using SFBigUintArray = vector < SFUintBN  >;
    using SFBigIntArray  = vector < SFIntBN   >;
    using SFAddressArray = vector < SFAddress >;
    using SFBloomArray   = vector < SFBloom   >;

    //-------------------------------------------------------------------------
    extern int64_t   str_2_Int    (const string_q& str);
    extern uint64_t  str_2_Uint   (const string_q& str);
    inline SFGas     str_2_Gas    (const string_q& str) { return str_2_Uint(str); }
    extern SFUintBN  str_2_BigUint(const string_q& str);
    extern SFIntBN   str_2_BigInt (const string_q& str);
    extern bool      str_2_Bool   (const string_q& str);
    extern double    str_2_Double (const string_q& str);
    extern string_q  chr_2_HexStr (const string_q& str);
    extern string_q  toHex        (const string_q& str);
    extern string_q  bnu_2_Hex    (const SFUintBN& bu);
    extern string_q  uint_2_Hex   (uint64_t num);

    //-------------------------------------------------------------------------
    extern string_q  to_string    (const SFUintBN& bu);
    extern string_q  to_string2   (const SFIntBN&  bi);
    extern SFAddress toAddress    (const string_q& str);
    extern SFHash    toHash       (const string_q& str);

    //--------------------------------------------------------------------
    extern SFUintBN  canonicalWei (const string_q& str);
    extern SFUintBN  canonicalWei (uint64_t val);
    extern string_q  wei2Ether    (const string_q& str);
    extern string_q  wei2Ether    (SFUintBN val);

    //-------------------------------------------------------------------------
    extern SFUintBN  hex2BigUint  (const string& s);

    //--------------------------------------------------------------------
    extern string_q  bloom2Bytes  (const SFBloom& bl);
    extern string_q  bloom2Bits   (const SFBloom& b);

    //--------------------------------------------------------------------
    extern string_q  toString     (int64_t i);
    extern string_q  toStringU    (uint64_t i);
    extern string_q  toStringBN   (const SFUintBN& bu);
    extern string_q  toStringBN   (const SFIntBN& bn);

    //--------------------------------------------------------------------
    extern string_q  double2Str   (double f, size_t nDecimals = 10);

    //--------------------------------------------------------------------
    extern bool      isZeroAddr   (const SFAddress& addr);
    extern bool      isNumeral    (const string_q& test);
    extern bool      isHexStr     (const string_q& str);
    extern bool      isAddress    (const SFAddress& addrIn);
    extern bool      isHash       (const SFHash& hashIn);
    extern bool      isUnsigned   (const string_q& in);

}  // namespace qblocks
