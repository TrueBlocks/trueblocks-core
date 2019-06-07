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
#include "basetypes.h"
#include "basenode.h"
#include "sfarchive.h"

namespace qblocks {

// EXISTING_CODE
enum cache_t { CT_BLOCKS = 0, CT_BLOOMS, CT_TXS, CT_TRACES, CT_ACCTS, CT_MONITORS, CT_INDEX };
// EXISTING_CODE

//--------------------------------------------------------------------------
class CCacheEntry : public CBaseNode {
public:
    uint64_t type;
    string_q extra;
    bool cached;
    string_q path;

public:
    CCacheEntry(void);
    CCacheEntry(const CCacheEntry& ca);
    virtual ~CCacheEntry(void);
    CCacheEntry& operator=(const CCacheEntry& ca);

    DECLARE_NODE(CCacheEntry);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCacheEntry& item) const;
    bool operator!=(const CCacheEntry& item) const { return !operator==(item); }
    friend bool operator<(const CCacheEntry& v1, const CCacheEntry& v2);
    friend ostream& operator<<(ostream& os, const CCacheEntry& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCacheEntry& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCacheEntry::CCacheEntry(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCacheEntry::CCacheEntry(const CCacheEntry& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCacheEntry::~CCacheEntry(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCacheEntry::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCacheEntry::initialize(void) {
    CBaseNode::initialize();

    type = 0;
    extra = "";
    cached = 0;
    path = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCacheEntry::duplicate(const CCacheEntry& ca) {
    clear();
    CBaseNode::duplicate(ca);

    type = ca.type;
    extra = ca.extra;
    cached = ca.cached;
    path = ca.path;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCacheEntry& CCacheEntry::operator=(const CCacheEntry& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCacheEntry::operator==(const CCacheEntry& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCacheEntry& v1, const CCacheEntry& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCacheEntry> CCacheEntryArray;
extern CArchive& operator>>(CArchive& archive, CCacheEntryArray& array);
extern CArchive& operator<<(CArchive& archive, const CCacheEntryArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<uint64_t, CCacheEntry> CCacheEntryMap;
// EXISTING_CODE
}  // namespace qblocks

