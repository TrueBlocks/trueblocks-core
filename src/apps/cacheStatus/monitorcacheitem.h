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
#include "etherlib.h"
#include "accountwatch.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMonitorCacheItem : public CAccountWatch {
public:
    string_q type;

public:
    CMonitorCacheItem(void);
    CMonitorCacheItem(const CMonitorCacheItem& mo);
    virtual ~CMonitorCacheItem(void);
    CMonitorCacheItem& operator=(const CMonitorCacheItem& mo);

    DECLARE_NODE(CMonitorCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMonitorCacheItem& item) const;
    bool operator!=(const CMonitorCacheItem& item) const { return !operator==(item); }
    friend bool operator<(const CMonitorCacheItem& v1, const CMonitorCacheItem& v2);
    friend ostream& operator<<(ostream& os, const CMonitorCacheItem& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMonitorCacheItem& mo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMonitorCacheItem::CMonitorCacheItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitorCacheItem::CMonitorCacheItem(const CMonitorCacheItem& mo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMonitorCacheItem::~CMonitorCacheItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCacheItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCacheItem::initialize(void) {
    CAccountWatch::initialize();

    type = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCacheItem::duplicate(const CMonitorCacheItem& mo) {
    clear();
    CAccountWatch::duplicate(mo);

    type = mo.type;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitorCacheItem& CMonitorCacheItem::operator=(const CMonitorCacheItem& mo) {
    duplicate(mo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMonitorCacheItem::operator==(const CMonitorCacheItem& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMonitorCacheItem& v1, const CMonitorCacheItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMonitorCacheItem> CMonitorCacheItemArray;
extern CArchive& operator>>(CArchive& archive, CMonitorCacheItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CMonitorCacheItemArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CMonitorCacheItem& mon);
extern CArchive& operator>>(CArchive& archive, CMonitorCacheItem& mon);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

