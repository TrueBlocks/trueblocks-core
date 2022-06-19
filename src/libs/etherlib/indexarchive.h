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

typedef enum {
    IP_NONE = 0,
    IP_LEAVEOPEN = (1 << 1),
    IP_HEADER = (1 << 2),
    IP_ADDRS = ((1 << 3) | IP_HEADER),
    IP_APPS = ((1 << 4) | IP_HEADER),
    IP_HEADEROPEN = (IP_HEADER | IP_LEAVEOPEN),
    IP_ALL = (IP_HEADER | IP_ADDRS | IP_APPS),
} indexpart_t;

//---------------------------------------------------------------------------
class CIndexArchive : public CArchive {
    CIndexedAddress* addresses2;
    CIndexedAppearance* appearances2;

  public:
    CIndexHeader header;

    explicit CIndexArchive(bool mode);
    virtual ~CIndexArchive(void);
    bool ReadIndexFromBinary(const string_q& fn, indexpart_t parts);
    blkrange_t getAppRangeForAddrAt(size_t i) const {
        ASSERT(addresses2 && i < header.nAddrs);
        blkrange_t r;
        r.first = addresses2[i].offset;
        r.second = r.first + addresses2[i].cnt - 1;
        return r;
    }
    CIndexedAppearance* getAppearanceAt(size_t i) {
        ASSERT(appearances2 && i < header.nApps);
        return &appearances2[i];
    }
    CIndexedAddress* getAddressAt(size_t i) {
        ASSERT(addresses2 && i < header.nAddrs);
        return &addresses2[i];
    }

  protected:
    char* rawData;
    CIndexArchive(void) : CArchive(READING_ARCHIVE) {
    }
    void clean(void);
};

//-----------------------------------------------------------------------
typedef bool (*INDEXCHUNKFUNC)(CIndexArchive& chunk, void* data);
extern bool forEveryIndexChunk(INDEXCHUNKFUNC func, void* data);
class CIndexChunkVisitor {
  public:
    indexpart_t parts{IP_HEADEROPEN};
    blkrange_t range = make_pair(0, NOPOS);
    INDEXCHUNKFUNC indexFunc = nullptr;
    void* callData = nullptr;
};

//-----------------------------------------------------------------------
#define MAGIC_NUMBER ((uint32_t)str_2_Uint("0xdeadbeef"))
extern hash_t versionHash;

}  // namespace qblocks
