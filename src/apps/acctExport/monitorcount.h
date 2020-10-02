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
class CMonitorCount : public CBaseNode {
  public:
    address_t address;
    uint64_t nRecords;
    uint64_t fileSize;

  public:
    CMonitorCount(void);
    CMonitorCount(const CMonitorCount& mo);
    virtual ~CMonitorCount(void);
    CMonitorCount& operator=(const CMonitorCount& mo);

    DECLARE_NODE(CMonitorCount);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMonitorCount& app) const;
    bool operator!=(const CMonitorCount& app) const {
        return !operator==(app);
    }
    friend bool operator<(const CMonitorCount& v1, const CMonitorCount& v2);
    friend ostream& operator<<(ostream& os, const CMonitorCount& app);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMonitorCount& mo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMonitorCount::CMonitorCount(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitorCount::CMonitorCount(const CMonitorCount& mo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMonitorCount::~CMonitorCount(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCount::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCount::initialize(void) {
    CBaseNode::initialize();

    address = "";
    nRecords = 0;
    fileSize = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitorCount::duplicate(const CMonitorCount& mo) {
    clear();
    CBaseNode::duplicate(mo);

    address = mo.address;
    nRecords = mo.nRecords;
    fileSize = mo.fileSize;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitorCount& CMonitorCount::operator=(const CMonitorCount& mo) {
    duplicate(mo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMonitorCount::operator==(const CMonitorCount& app) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMonitorCount& v1, const CMonitorCount& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMonitorCount> CMonitorCountArray;
extern CArchive& operator>>(CArchive& archive, CMonitorCountArray& array);
extern CArchive& operator<<(CArchive& archive, const CMonitorCountArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MONITORCOUNT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
