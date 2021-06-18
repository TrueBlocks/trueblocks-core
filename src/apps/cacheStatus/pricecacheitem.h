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
class CPriceCacheItem : public CCacheBase {
  public:
    string_q type;
    string_q pair;

  public:
    CPriceCacheItem(void);
    CPriceCacheItem(const CPriceCacheItem& pr);
    virtual ~CPriceCacheItem(void);
    CPriceCacheItem& operator=(const CPriceCacheItem& pr);

    DECLARE_NODE(CPriceCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPriceCacheItem& it) const;
    bool operator!=(const CPriceCacheItem& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPriceCacheItem& v1, const CPriceCacheItem& v2);
    friend ostream& operator<<(ostream& os, const CPriceCacheItem& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPriceCacheItem& pr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPriceCacheItem::CPriceCacheItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPriceCacheItem::CPriceCacheItem(const CPriceCacheItem& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPriceCacheItem::~CPriceCacheItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceCacheItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceCacheItem::initialize(void) {
    CCacheBase::initialize();

    type = "";
    pair = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceCacheItem::duplicate(const CPriceCacheItem& pr) {
    clear();
    CCacheBase::duplicate(pr);

    type = pr.type;
    pair = pr.pair;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPriceCacheItem& CPriceCacheItem::operator=(const CPriceCacheItem& pr) {
    duplicate(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPriceCacheItem::operator==(const CPriceCacheItem& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPriceCacheItem& v1, const CPriceCacheItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPriceCacheItem> CPriceCacheItemArray;
extern CArchive& operator>>(CArchive& archive, CPriceCacheItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CPriceCacheItemArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PRICECACHEITEM;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
