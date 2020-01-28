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
#include "cache.h"
#include "abicacheitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAbiCache : public CCache {
  public:
    CAbiCacheItemArray items;

  public:
    CAbiCache(void);
    CAbiCache(const CAbiCache& ab);
    virtual ~CAbiCache(void);
    CAbiCache& operator=(const CAbiCache& ab);

    DECLARE_NODE(CAbiCache);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAbiCache& item) const;
    bool operator!=(const CAbiCache& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CAbiCache& v1, const CAbiCache& v2);
    friend ostream& operator<<(ostream& os, const CAbiCache& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAbiCache& ab);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbiCache::CAbiCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbiCache::CAbiCache(const CAbiCache& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbiCache::~CAbiCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiCache::initialize(void) {
    CCache::initialize();

    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiCache::duplicate(const CAbiCache& ab) {
    clear();
    CCache::duplicate(ab);

    items = ab.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbiCache& CAbiCache::operator=(const CAbiCache& ab) {
    duplicate(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAbiCache::operator==(const CAbiCache& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAbiCache& v1, const CAbiCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAbiCache> CAbiCacheArray;
extern CArchive& operator>>(CArchive& archive, CAbiCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CAbiCacheArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ABICACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
