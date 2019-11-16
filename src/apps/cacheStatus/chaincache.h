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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CChainCache : public CCache {
public:
    CChainCache(void);
    CChainCache(const CChainCache& ch);
    virtual ~CChainCache(void);
    CChainCache& operator=(const CChainCache& ch);

    DECLARE_NODE(CChainCache);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChainCache& item) const;
    bool operator!=(const CChainCache& item) const { return !operator==(item); }
    friend bool operator<(const CChainCache& v1, const CChainCache& v2);
    friend ostream& operator<<(ostream& os, const CChainCache& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChainCache& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChainCache::CChainCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChainCache::CChainCache(const CChainCache& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChainCache::~CChainCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChainCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChainCache::initialize(void) {
    CCache::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChainCache::duplicate(const CChainCache& ch) {
    clear();
    CCache::duplicate(ch);

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChainCache& CChainCache::operator=(const CChainCache& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChainCache::operator==(const CChainCache& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChainCache& v1, const CChainCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChainCache> CChainCacheArray;
extern CArchive& operator>>(CArchive& archive, CChainCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CChainCacheArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CHAINCACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

