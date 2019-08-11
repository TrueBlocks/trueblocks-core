#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
class CAddressCache : public CCache {
public:
    CAddressCache(void);
    CAddressCache(const CAddressCache& ad);
    virtual ~CAddressCache(void);
    CAddressCache& operator=(const CAddressCache& ad);

    DECLARE_NODE(CAddressCache);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAddressCache& item) const;
    bool operator!=(const CAddressCache& item) const { return !operator==(item); }
    friend bool operator<(const CAddressCache& v1, const CAddressCache& v2);
    friend ostream& operator<<(ostream& os, const CAddressCache& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAddressCache& ad);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAddressCache::CAddressCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAddressCache::CAddressCache(const CAddressCache& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAddressCache::~CAddressCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddressCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddressCache::initialize(void) {
    CCache::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddressCache::duplicate(const CAddressCache& ad) {
    clear();
    CCache::duplicate(ad);

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAddressCache& CAddressCache::operator=(const CAddressCache& ad) {
    duplicate(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAddressCache::operator==(const CAddressCache& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAddressCache& v1, const CAddressCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAddressCache> CAddressCacheArray;
extern CArchive& operator>>(CArchive& archive, CAddressCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CAddressCacheArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAddressCache& add);
extern CArchive& operator>>(CArchive& archive, CAddressCache& add);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

