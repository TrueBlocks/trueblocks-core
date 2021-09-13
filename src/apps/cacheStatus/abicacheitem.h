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
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "cachebase.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAbiCacheItem : public CCacheBase {
  public:
    string_q type;
    uint64_t nFunctions;
    uint64_t nEvents;
    uint64_t nOther;

  public:
    CAbiCacheItem(void);
    CAbiCacheItem(const CAbiCacheItem& ab);
    virtual ~CAbiCacheItem(void);
    CAbiCacheItem& operator=(const CAbiCacheItem& ab);

    DECLARE_NODE(CAbiCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAbiCacheItem& it) const;
    bool operator!=(const CAbiCacheItem& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAbiCacheItem& v1, const CAbiCacheItem& v2);
    friend ostream& operator<<(ostream& os, const CAbiCacheItem& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAbiCacheItem& ab);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbiCacheItem::CAbiCacheItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbiCacheItem::CAbiCacheItem(const CAbiCacheItem& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbiCacheItem::~CAbiCacheItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiCacheItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiCacheItem::initialize(void) {
    CCacheBase::initialize();

    type = "";
    nFunctions = 0;
    nEvents = 0;
    nOther = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiCacheItem::duplicate(const CAbiCacheItem& ab) {
    clear();
    CCacheBase::duplicate(ab);

    type = ab.type;
    nFunctions = ab.nFunctions;
    nEvents = ab.nEvents;
    nOther = ab.nOther;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbiCacheItem& CAbiCacheItem::operator=(const CAbiCacheItem& ab) {
    duplicate(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAbiCacheItem::operator==(const CAbiCacheItem& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAbiCacheItem& v1, const CAbiCacheItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAbiCacheItem> CAbiCacheItemArray;
extern CArchive& operator>>(CArchive& archive, CAbiCacheItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CAbiCacheItemArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAbiCacheItem& abi);
extern CArchive& operator>>(CArchive& archive, CAbiCacheItem& abi);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ABICACHEITEM;

//---------------------------------------------------------------------------
extern string_q nextCachebaseChunk(const string_q& fieldIn, const void* data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
