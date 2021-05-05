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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
typedef vector<uint32_t> CUint32Array;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CIndexCacheItem : public CBaseNode {
  public:
    string_q type;
    uint32_t nAddresses;
    uint32_t nAppearances;
    blknum_t firstAppearance;
    blknum_t latestAppearance;
    timestamp_t firstTs;
    timestamp_t latestTs;
    string_q filename;
    time_q fileDate;
    uint32_t indexSizeBytes;
    ipfshash_t index_hash;
    uint32_t bloomSizeBytes;
    ipfshash_t bloom_hash;

  public:
    CIndexCacheItem(void);
    CIndexCacheItem(const CIndexCacheItem& in);
    virtual ~CIndexCacheItem(void);
    CIndexCacheItem& operator=(const CIndexCacheItem& in);

    DECLARE_NODE(CIndexCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CIndexCacheItem& it) const;
    bool operator!=(const CIndexCacheItem& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CIndexCacheItem& v1, const CIndexCacheItem& v2);
    friend ostream& operator<<(ostream& os, const CIndexCacheItem& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIndexCacheItem& in);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIndexCacheItem::CIndexCacheItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIndexCacheItem::CIndexCacheItem(const CIndexCacheItem& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIndexCacheItem::~CIndexCacheItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIndexCacheItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIndexCacheItem::initialize(void) {
    CBaseNode::initialize();

    type = "";
    nAddresses = 0;
    nAppearances = 0;
    firstAppearance = 0;
    latestAppearance = 0;
    firstTs = 0;
    latestTs = 0;
    filename = "";
    fileDate = earliestDate;
    indexSizeBytes = 0;
    index_hash = "";
    bloomSizeBytes = 0;
    bloom_hash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIndexCacheItem::duplicate(const CIndexCacheItem& in) {
    clear();
    CBaseNode::duplicate(in);

    type = in.type;
    nAddresses = in.nAddresses;
    nAppearances = in.nAppearances;
    firstAppearance = in.firstAppearance;
    latestAppearance = in.latestAppearance;
    firstTs = in.firstTs;
    latestTs = in.latestTs;
    filename = in.filename;
    fileDate = in.fileDate;
    indexSizeBytes = in.indexSizeBytes;
    index_hash = in.index_hash;
    bloomSizeBytes = in.bloomSizeBytes;
    bloom_hash = in.bloom_hash;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIndexCacheItem& CIndexCacheItem::operator=(const CIndexCacheItem& in) {
    duplicate(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIndexCacheItem::operator==(const CIndexCacheItem& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIndexCacheItem& v1, const CIndexCacheItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIndexCacheItem> CIndexCacheItemArray;
extern CArchive& operator>>(CArchive& archive, CIndexCacheItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CIndexCacheItemArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_INDEXCACHEITEM;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
