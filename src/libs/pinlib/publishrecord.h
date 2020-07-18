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
#include "acctlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPublishRecord : public CBaseNode {
  public:
    string_q filename;
    ipfshash_t index_hash;
    bool index_pinned;
    ipfshash_t bloom_hash;
    bool bloom_pinned;

  public:
    CPublishRecord(void);
    CPublishRecord(const CPublishRecord& pu);
    virtual ~CPublishRecord(void);
    CPublishRecord& operator=(const CPublishRecord& pu);

    DECLARE_NODE(CPublishRecord);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPublishRecord& item) const;
    bool operator!=(const CPublishRecord& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CPublishRecord& v1, const CPublishRecord& v2);
    friend ostream& operator<<(ostream& os, const CPublishRecord& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPublishRecord& pu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPublishRecord::CPublishRecord(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPublishRecord::CPublishRecord(const CPublishRecord& pu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPublishRecord::~CPublishRecord(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPublishRecord::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPublishRecord::initialize(void) {
    CBaseNode::initialize();

    filename = "";
    index_hash = "";
    index_pinned = false;
    bloom_hash = "";
    bloom_pinned = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPublishRecord::duplicate(const CPublishRecord& pu) {
    clear();
    CBaseNode::duplicate(pu);

    filename = pu.filename;
    index_hash = pu.index_hash;
    index_pinned = pu.index_pinned;
    bloom_hash = pu.bloom_hash;
    bloom_pinned = pu.bloom_pinned;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPublishRecord& CPublishRecord::operator=(const CPublishRecord& pu) {
    duplicate(pu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPublishRecord::operator==(const CPublishRecord& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPublishRecord& v1, const CPublishRecord& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPublishRecord> CPublishRecordArray;
extern CArchive& operator>>(CArchive& archive, CPublishRecordArray& array);
extern CArchive& operator<<(CArchive& archive, const CPublishRecordArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PUBLISHRECORD;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef bool (*IPFSVISITFUNC)(CPublishRecord& log, void* data);
bool publishNotPinned(const string_q& filename, CPublishRecord& result);
bool publishPinned(const string_q& filename, CPublishRecord& result);
bool forEveryPinnedItem(LOGVISITFUNC func, void* data);
bool forEveryIPFSItem(LOGVISITFUNC func, void* data);
// EXISTING_CODE
}  // namespace qblocks
