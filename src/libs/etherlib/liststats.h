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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CListStats : public CBaseNode {
  public:
    address_t address;
    uint64_t nRecords;
    uint64_t fileSize;

  public:
    CListStats(void);
    CListStats(const CListStats& li);
    virtual ~CListStats(void);
    CListStats& operator=(const CListStats& li);

    DECLARE_NODE(CListStats);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CListStats& it) const;
    bool operator!=(const CListStats& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CListStats& v1, const CListStats& v2);
    friend ostream& operator<<(ostream& os, const CListStats& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CListStats& li);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CListStats::CListStats(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CListStats::CListStats(const CListStats& li) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(li);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CListStats::~CListStats(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CListStats::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CListStats::initialize(void) {
    CBaseNode::initialize();

    address = "";
    nRecords = 0;
    fileSize = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CListStats::duplicate(const CListStats& li) {
    clear();
    CBaseNode::duplicate(li);

    address = li.address;
    nRecords = li.nRecords;
    fileSize = li.fileSize;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CListStats& CListStats::operator=(const CListStats& li) {
    duplicate(li);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CListStats::operator==(const CListStats& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CListStats& v1, const CListStats& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CListStats> CListStatsArray;
extern CArchive& operator>>(CArchive& archive, CListStatsArray& array);
extern CArchive& operator<<(CArchive& archive, const CListStatsArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CListStats& lis);
extern CArchive& operator>>(CArchive& archive, CListStats& lis);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_LISTSTATS;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
