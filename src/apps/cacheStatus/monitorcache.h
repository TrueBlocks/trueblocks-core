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
#include "monitorcacheitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMonitorCache : public CCache {
  public:
    CAddressArray addrs;
    CMonitorCacheItemArray items;

  public:
    CMonitorCache(void);
    CMonitorCache(const CMonitorCache& mo);
    virtual ~CMonitorCache(void);
    CMonitorCache& operator=(const CMonitorCache& mo);

    DECLARE_NODE(CMonitorCache);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;
    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMonitorCache& it) const;
    bool operator!=(const CMonitorCache& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CMonitorCache& v1, const CMonitorCache& v2);
    friend ostream& operator<<(ostream& os, const CMonitorCache& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMonitorCache& mo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMonitorCache::CMonitorCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitorCache::CMonitorCache(const CMonitorCache& mo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMonitorCache::~CMonitorCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCache::initialize(void) {
    CCache::initialize();

    addrs.clear();
    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCache::duplicate(const CMonitorCache& mo) {
    clear();
    CCache::duplicate(mo);

    addrs = mo.addrs;
    items = mo.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitorCache& CMonitorCache::operator=(const CMonitorCache& mo) {
    duplicate(mo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMonitorCache::operator==(const CMonitorCache& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMonitorCache& v1, const CMonitorCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMonitorCache> CMonitorCacheArray;
extern CArchive& operator>>(CArchive& archive, CMonitorCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CMonitorCacheArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MONITORCACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
