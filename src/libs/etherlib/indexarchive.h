#pragma once
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
#include "etherlib.h"
#include "appearance.h"
#include "bloom.h"
#include "indexheader.h"
#include "indexedaddress.h"
#include "indexedappearance.h"

namespace qblocks {

typedef struct CReverseAppMapEntry {
  public:
    uint32_t n;
    uint32_t blk;
    uint32_t tx;
} CReverseAppMapEntry;

//---------------------------------------------------------------------------
class CIndexArchive : public CArchive {
  public:
    CIndexHeader* header;
    uint64_t nAddrs;
    CIndexedAddress* addresses;
    CBlockRangeArray reverseAddrRanges;
    uint64_t nApps;
    CIndexedAppearance* appearances;
    CReverseAppMapEntry* reverseAppMap{nullptr};

    explicit CIndexArchive(bool mode);
    ~CIndexArchive(void);
    bool ReadIndexFromBinary(const string_q& fn);
    bool ReadIndexHeader(const string_q& fn, CIndexHeader& header);
    bool LoadReverseMaps(const blkrange_t& range);

  private:
    char* rawData;
    CIndexArchive(void) : CArchive(READING_ARCHIVE) {
    }
};

//-----------------------------------------------------------------------
#define MAGIC_NUMBER ((uint32_t)str_2_Uint("0xdeadbeef"))
extern hash_t versionHash;
//--------------------------------------------------------------
typedef bool (*INDEXCHUNKFUNC)(CIndexArchive& chunk, void* data);
typedef bool (*INDEXBLOOMFUNC)(CBloomArray& blooms, void* data);
typedef bool (*ADDRESSFUNC)(const address_t& addr, void* data);
class CChunkVisitor {
  public:
    INDEXCHUNKFUNC indexFunc = nullptr;
    ADDRESSFUNC addrFunc = nullptr;
    void* callData = nullptr;
    blkrange_t range = make_pair(0, NOPOS);
};
extern bool bloomsAreInitalized(void);
extern bool readIndexHeader(const string_q& inFn, CIndexHeader& header);
}  // namespace qblocks

#if 0
// extern size_t readIndexFromBinary(const string_q& inFn, uint64_t& nApps, const CStringArray& lines);
// extern bool forEveryIndexChunk(INDEXCHUNKFUNC func, void* data);
// extern bool forEveryAddressInIndex(ADDRESSFUNC func, const blkrange_t& range, void* data);
// extern bool forEverySmartContractInIndex(ADDRESSFUNC func, void* data);
// extern bool chunksAreInitalized(void);
#endif
