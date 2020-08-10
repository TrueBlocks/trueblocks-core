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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "indexarchive.h"

namespace qblocks {

//-----------------------------------------------------------------------
typedef map<blknum_t, string_q> CIndexHashMap;

//-----------------------------------------------------------------------
#define MAGIC_NUMBER ((uint32_t)str_2_Uint("0xdeadbeef"))
extern hash_t versionHash;
extern void writeIndexAsAscii(const string_q& outFn, const CStringArray& lines);
extern void writeIndexAsBinary(const string_q& outFn, const CStringArray& lines);
extern size_t readIndexFromBinary(const string_q& inFn, uint64_t& nAppearances, const CStringArray& lines);
extern bool readIndexHeaderFromBinary(const string_q& inFn, CHeaderRecord_base* header);
    //--------------------------------------------------------------
    typedef bool (*INDEXCHUNKFUNC)(CIndexArchive& chunk, void* data);
    class CChunkVisitor {
    public:
        INDEXCHUNKFUNC callFunc;
        void *callData;
    };

}  // namespace qblocks
