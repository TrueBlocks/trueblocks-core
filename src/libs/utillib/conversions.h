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
#include "sftime.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    using timestamp_t = int64_t;
    using blknum_t    = uint64_t;
    using txnum_t     = uint64_t;
    using gas_t       = uint64_t;
    using address_t   = string_q;
    using hash_t      = string_q;
    using bloom_t     = SFUintBN;
    using wei_t       = SFUintBN;
    using topic_t     = SFUintBN;

    //-------------------------------------------------------------------------
    using CStringArray   = vector < string_q  >;
    using CBlockNumArray = vector < uint64_t  >;
    using CUintArray     = vector < uint64_t  >;
    using CIntArray      = vector < int64_t   >;
    using CBigUintArray  = vector < SFUintBN  >;
    using CBigIntArray   = vector < SFIntBN   >;
    using CAddressArray  = vector < address_t >;
    using CBloomArray    = vector < bloom_t   >;
    using CTopicArray    = vector < topic_t   >;

    //-------------------------------------------------------------------------
    extern bool        str_2_Bool   (const string_q& str);
    extern int64_t     str_2_Int    (const string_q& str);
    extern uint64_t    str_2_Uint   (const string_q& str);
    extern string_q    str_2_Hex    (const string_q& str);
    extern gas_t       str_2_Gas    (const string_q& str);
    extern double      str_2_Double (const string_q& str);
    extern SFIntBN     str_2_BigInt (const string_q& str);
    extern SFUintBN    str_2_BigUint(const string_q& str);
    extern address_t   str_2_Addr   (const string_q& str);
    extern hash_t      str_2_Hash   (const string_q& str);
    extern SFUintBN    str_2_Wei    (const string_q& str);
    extern bloom_t     str_2_Bloom  (const string_q& str);
    extern topic_t     str_2_Topic  (const string_q& str);
    extern timestamp_t str_2_Ts     (const string_q& str);

    //-------------------------------------------------------------------------
    extern string_q    bool_2_Str   (bool num);
    extern string_q    int_2_Str    (int64_t num);
    extern string_q    uint_2_Str   (uint64_t num);
    extern string_q    gas_2_Str    (const gas_t& gas);
    extern string_q    double_2_Str (double f, size_t nDecimals = 10);
    extern string_q    bni_2_Str    (const SFIntBN& bn);
    extern string_q    bnu_2_Str    (const SFUintBN& bu);
    extern string_q    addr_2_Str   (const address_t& addr);
    extern string_q    hash_2_Str   (const hash_t& hash);
    extern string_q    wei_2_Str    (const wei_t& wei);
    extern string_q    bloom_2_Str  (const bloom_t& bloom);
    extern string_q    topic_2_Str  (const topic_t& topic);
    extern string_q    ts_2_Str     (timestamp_t ts);

    //-------------------------------------------------------------------------
    extern string_q    chr_2_HexStr (const string_q& str);
    extern string_q    bnu_2_Hex    (const SFUintBN& bu);
    extern string_q    uint_2_Hex   (uint64_t num);
    extern string_q    wei_2_Ether  (const string_q& str);
    extern string_q    wei_2_Ether  (SFUintBN val);
    extern string_q    bloom_2_Bytes(const bloom_t& bl);
    extern string_q    bloom_2_Bits (const bloom_t& bl);

    //--------------------------------------------------------------------
    class time_q;
    extern timestamp_t date_2_Ts(const time_q&   timeIn);
    extern time_q      ts_2_Date(timestamp_t tsIn);

    //--------------------------------------------------------------------
    extern bool        isZeroHash   (const hash_t& hash);
    extern bool        isZeroAddr   (const address_t& addr);
    extern bool        isNumeral    (const string_q& test);
    extern bool        isHexStr     (const string_q& str);
    extern bool        isAddress    (const address_t& addr);
    extern bool        isHash       (const hash_t& hashIn);
    extern bool        isUnsigned   (const string_q& in);

}  // namespace qblocks
