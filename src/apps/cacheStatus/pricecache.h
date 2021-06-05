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
#include "cache.h"
#include "pricecacheitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPriceCache : public CCache {
  public:
    CPriceCacheItemArray items;

  public:
    CPriceCache(void);
    CPriceCache(const CPriceCache& pr);
    virtual ~CPriceCache(void);
    CPriceCache& operator=(const CPriceCache& pr);

    DECLARE_NODE(CPriceCache);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPriceCache& it) const;
    bool operator!=(const CPriceCache& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPriceCache& v1, const CPriceCache& v2);
    friend ostream& operator<<(ostream& os, const CPriceCache& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPriceCache& pr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPriceCache::CPriceCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPriceCache::CPriceCache(const CPriceCache& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPriceCache::~CPriceCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceCache::initialize(void) {
    CCache::initialize();

    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceCache::duplicate(const CPriceCache& pr) {
    clear();
    CCache::duplicate(pr);

    items = pr.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPriceCache& CPriceCache::operator=(const CPriceCache& pr) {
    duplicate(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPriceCache::operator==(const CPriceCache& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPriceCache& v1, const CPriceCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPriceCache> CPriceCacheArray;
extern CArchive& operator>>(CArchive& archive, CPriceCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CPriceCacheArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PRICECACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
