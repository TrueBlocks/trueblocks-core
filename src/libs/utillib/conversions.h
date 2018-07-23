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
    extern string_q  toHex        (const string_q& str);

    //-------------------------------------------------------------------------
    extern string_q  chr_2_HexStr (const string_q& str);
    extern string_q  bnu_2_Hex    (const SFUintBN& bu);
    extern string_q  uint_2_Hex   (uint64_t num);
    extern string_q  wei_2_Ether  (const string_q& str);
    extern string_q  wei_2_Ether  (SFUintBN val);
    extern string_q  bloom_2_Bytes(const SFBloom& bl);
    extern string_q  bloom_2_Bits (const SFBloom& bl);

    //-------------------------------------------------------------------------
    extern bool      str_2_Bool   (const string_q& str);
    extern int64_t   str_2_Int    (const string_q& str);
    extern uint64_t  str_2_Uint   (const string_q& str);
    inline SFGas     str_2_Gas    (const string_q& str) { return str_2_Uint(str); }
    extern double    str_2_Double (const string_q& str);
    extern SFIntBN   str_2_BigInt (const string_q& str);
    extern SFUintBN  str_2_BigUint(const string_q& str);
    extern SFAddress str_2_Addr   (const string_q& str);
    extern SFHash    str_2_Hash   (const string_q& str);
    extern SFUintBN  str_2_Wei    (const string_q& str);
    inline SFBloom   str_2_Bloom  (const string_q& str) { return str_2_Wei(str); }
    inline SFUintBN  str_2_Topic  (const string_q& str) { return str_2_Wei(str); }

    //-------------------------------------------------------------------------
    extern string_q  int_2_Str    (int64_t num);
    extern string_q  uint_2_Str   (uint64_t num);
    inline string_q  gas_2_Str    (const SFGas& gas)      { return uint_2_Str(gas); }
    extern string_q  double_2_Str (double f, size_t nDecimals = 10);
    extern string_q  bni_2_Str    (const SFIntBN& bn);
    extern string_q  bnu_2_Str    (const SFUintBN& bu);
    inline string_q  addr_2_Str   (const SFAddress& addr) { return (addr.empty() ? "0x0" : addr); }
    inline string_q  hash_2_Str   (const SFHash& hash)    { return (hash.empty() ? "0x0" : hash); }
    inline string_q  wei_2_Str    (const SFWei& wei)      { return bnu_2_Str(wei); }
    inline string_q  bloom_2_Str  (const SFBloom& bloom)  { return bnu_2_Str(bloom); }
    inline string_q  topic_2_Str  (const SFUintBN& topic) { return ("0x" + padLeft(toLower(bnu_2_Hex(topic)), 64, '0')); }

    //--------------------------------------------------------------------
    extern bool      isZeroHash   (const SFHash& hash);
    inline bool      isZeroAddr   (const SFAddress& addr) { return isZeroHash(addr); };
    extern bool      isNumeral    (const string_q& test);
    extern bool      isHexStr     (const string_q& str);
    extern bool      isAddress    (const SFAddress& addr);
    extern bool      isHash       (const SFHash& hashIn);
    extern bool      isUnsigned   (const string_q& in);

}  // namespace qblocks
