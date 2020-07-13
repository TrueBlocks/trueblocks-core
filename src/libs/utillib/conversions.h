#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
using blknum_t = uint64_t;
using blkrange_t = pair<blknum_t, blknum_t>;
using txnum_t = uint64_t;
using gas_t = uint64_t;
using address_t = string_q;
using hash_t = string_q;
using ipfshash_t = string_q;
using wei_t = biguint_t;
using topic_t = string_q;
using uchar_t = unsigned char;
using addrbytes_t = vector<uint8_t>;
using hashbytes_t = vector<uint8_t>;
using addr_wei_mp = map<address_t, wei_t>;

//-------------------------------------------------------------------------
using CStringArray = vector<string_q>;
using CBlockNumArray = vector<uint64_t>;
using CBlockRangeArray = vector<blkrange_t>;
using CUintArray = vector<uint64_t>;
using CIntArray = vector<int64_t>;
using CBigUintArray = vector<biguint_t>;
using CBigIntArray = vector<bigint_t>;
using CAddressArray = vector<address_t>;
using CTopicArray = vector<topic_t>;

//-------------------------------------------------------------------------
extern bool str_2_Bool(const string_q& str);
extern int64_t str_2_Int(const string_q& str);
extern uint64_t str_2_Uint(const string_q& str);
extern string_q str_2_Hex(const string_q& str);
extern gas_t str_2_Gas(const string_q& str);
extern double str_2_Double(const string_q& str);
extern bigint_t str_2_BigInt(const string_q& str, size_t bits = 257);
extern biguint_t str_2_BigUint(const string_q& str, size_t bits = 257);
extern biguint_t topic_2_BigUint(const topic_t& topic);
extern address_t str_2_Addr(const string_q& str);
extern hash_t str_2_Hash(const string_q& str);
extern biguint_t str_2_Wei(const string_q& str);
inline topic_t str_2_Topic(const string_q& str) {
    return str;
}
extern timestamp_t str_2_Ts(const string_q& str);
#define str_2_Enum(en, str) (en) str_2_Uint(str)

//-------------------------------------------------------------------------
extern string_q bool_2_Str(bool num);
extern string_q bool_2_Str_t(bool num);
extern string_q int_2_Str(int64_t num);
extern string_q uint_2_Str(uint64_t num);
extern string_q gas_2_Str(const gas_t& gas);
extern string_q double_2_Str(double f, size_t nDecimals = NOPOS);
extern string_q bni_2_Str(const bigint_t& bn);
extern string_q bnu_2_Str(const biguint_t& bu);
extern string_q addr_2_Str(const address_t& addr);
extern string_q hash_2_Str(const hash_t& hash);
extern string_q wei_2_Str(const wei_t& wei);
inline string_q topic_2_Str(const topic_t& topic) {
    return topic;
}
extern string_q ts_2_Str(timestamp_t ts);

//----------------------------------------------------------------------------
extern string_q hex_2_Str(const string_q& inHex, size_t nBytes = NOPOS);
extern uchar_t hex_2_Ascii(char c1, char c2);
extern hashbytes_t hash_2_Bytes(const hash_t& in);
extern addrbytes_t addr_2_Bytes(const address_t& in);
extern hash_t bytes_2_Hash(uint8_t const bytes[32]);
extern address_t bytes_2_Addr(uint8_t const bytes[20]);

//-------------------------------------------------------------------------
extern string_q chr_2_HexStr(const string_q& str);
extern string_q bnu_2_Hex(const biguint_t& bu);
extern string_q uint_2_Hex(uint64_t num);
extern string_q wei_2_Ether(const string_q& str);
extern string_q wei_2_Ether(biguint_t val);
extern string_q byte_2_Bits(uint8_t ch);

//--------------------------------------------------------------------
class time_q;
extern timestamp_t date_2_Ts(const time_q& timeIn);
extern time_q ts_2_Date(const timestamp_t& tsIn);
extern time_q str_2_Date(const string_q& str);

//--------------------------------------------------------------------
extern bool isZeroHash(const hash_t& hash);
extern bool isZeroAddr(const address_t& addr);
extern bool isNumeral(const string_q& test);
extern bool isHexStr(const string_q& str);
extern bool isAddress(const string_q& addr);
extern bool isDate(const string_q& date);
extern bool isHash(const hash_t& hashIn);
extern bool isUnsigned(const string_q& in);

//--------------------------------------------------------------------
extern bool rangesIntersect(const blkrange_t& r1, const blkrange_t& r2);

typedef enum { NODIR, PREV, NEXT } direction_t;

}  // namespace qblocks
