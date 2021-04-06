#if 0
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
#include "utillib.h"
#include "silkworm/db/lmdbstat.h"
#include "silkworm/db/lmdbrecord.h"
#include "silkworm/db/lmdbval.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTableEntry : public CBaseNode {
  public:
    uint64_t id;
    uint32_t silky;
    string_q name;
    string_q longName;
    string_q description;
    uint64_t freelist;
    CLmdbStat stat;

  public:
    CTableEntry(void);
    CTableEntry(const CTableEntry& ta);
    virtual ~CTableEntry(void);
    CTableEntry& operator=(const CTableEntry& ta);

    DECLARE_NODE(CTableEntry);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTableEntry& it) const;
    bool operator!=(const CTableEntry& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTableEntry& v1, const CTableEntry& v2);
    friend ostream& operator<<(ostream& os, const CTableEntry& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTableEntry& ta);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTableEntry::CTableEntry(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTableEntry::CTableEntry(const CTableEntry& ta) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ta);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTableEntry::~CTableEntry(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTableEntry::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTableEntry::initialize(void) {
    CBaseNode::initialize();

    id = 0;
    silky = 0;
    name = "";
    longName = "";
    description = "";
    freelist = 0;
    stat = CLmdbStat();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTableEntry::duplicate(const CTableEntry& ta) {
    clear();
    CBaseNode::duplicate(ta);

    id = ta.id;
    silky = ta.silky;
    name = ta.name;
    longName = ta.longName;
    description = ta.description;
    freelist = ta.freelist;
    stat = ta.stat;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTableEntry& CTableEntry::operator=(const CTableEntry& ta) {
    duplicate(ta);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTableEntry::operator==(const CTableEntry& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTableEntry& v1, const CTableEntry& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTableEntry> CTableEntryArray;
extern CArchive& operator>>(CArchive& archive, CTableEntryArray& array);
extern CArchive& operator<<(CArchive& archive, const CTableEntryArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTableEntry& tab);
extern CArchive& operator>>(CArchive& archive, CTableEntry& tab);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TABLEENTRY;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
#endif
