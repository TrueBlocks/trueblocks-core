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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CEntityCacheItem : public CAccountName {
  public:
    string_q type;

  public:
    CEntityCacheItem(void);
    CEntityCacheItem(const CEntityCacheItem& en);
    virtual ~CEntityCacheItem(void);
    CEntityCacheItem& operator=(const CEntityCacheItem& en);

    DECLARE_NODE(CEntityCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEntityCacheItem& it) const;
    bool operator!=(const CEntityCacheItem& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CEntityCacheItem& v1, const CEntityCacheItem& v2);
    friend ostream& operator<<(ostream& os, const CEntityCacheItem& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEntityCacheItem& en);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEntityCacheItem::CEntityCacheItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEntityCacheItem::CEntityCacheItem(const CEntityCacheItem& en) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(en);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEntityCacheItem::~CEntityCacheItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntityCacheItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntityCacheItem::initialize(void) {
    CAccountName::initialize();

    type = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntityCacheItem::duplicate(const CEntityCacheItem& en) {
    clear();
    CAccountName::duplicate(en);

    type = en.type;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEntityCacheItem& CEntityCacheItem::operator=(const CEntityCacheItem& en) {
    duplicate(en);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEntityCacheItem::operator==(const CEntityCacheItem& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEntityCacheItem& v1, const CEntityCacheItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEntityCacheItem> CEntityCacheItemArray;
extern CArchive& operator>>(CArchive& archive, CEntityCacheItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CEntityCacheItemArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ENTITYCACHEITEM;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
