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
#include "indexcacheitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CIndexCache : public CCache {
public:
    CIndexCacheItemArray items;

public:
    CIndexCache(void);
    CIndexCache(const CIndexCache& in);
    virtual ~CIndexCache(void);
    CIndexCache& operator=(const CIndexCache& in);

    DECLARE_NODE(CIndexCache);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CIndexCache& item) const;
    bool operator!=(const CIndexCache& item) const { return !operator==(item); }
    friend bool operator<(const CIndexCache& v1, const CIndexCache& v2);
    friend ostream& operator<<(ostream& os, const CIndexCache& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIndexCache& in);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIndexCache::CIndexCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIndexCache::CIndexCache(const CIndexCache& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIndexCache::~CIndexCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIndexCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIndexCache::initialize(void) {
    CCache::initialize();

    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIndexCache::duplicate(const CIndexCache& in) {
    clear();
    CCache::duplicate(in);

    items = in.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIndexCache& CIndexCache::operator=(const CIndexCache& in) {
    duplicate(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIndexCache::operator==(const CIndexCache& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIndexCache& v1, const CIndexCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIndexCache> CIndexCacheArray;
extern CArchive& operator>>(CArchive& archive, CIndexCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CIndexCacheArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CIndexCache& ind);
extern CArchive& operator>>(CArchive& archive, CIndexCache& ind);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_INDEXCACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

