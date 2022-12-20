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
class CAppearanceCount : public CBaseNode {
  public:
    address_t address;
    uint64_t nRecords;
    uint64_t fileSize;

  public:
    CAppearanceCount(void);
    CAppearanceCount(const CAppearanceCount& li);
    virtual ~CAppearanceCount(void);
    CAppearanceCount& operator=(const CAppearanceCount& li);

    DECLARE_NODE(CAppearanceCount);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAppearanceCount& it) const;
    bool operator!=(const CAppearanceCount& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAppearanceCount& v1, const CAppearanceCount& v2);
    friend ostream& operator<<(ostream& os, const CAppearanceCount& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAppearanceCount& li);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAppearanceCount::CAppearanceCount(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearanceCount::CAppearanceCount(const CAppearanceCount& li) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(li);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAppearanceCount::~CAppearanceCount(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearanceCount::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearanceCount::initialize(void) {
    CBaseNode::initialize();

    address = "";
    nRecords = 0;
    fileSize = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearanceCount::duplicate(const CAppearanceCount& li) {
    clear();
    CBaseNode::duplicate(li);

    address = li.address;
    nRecords = li.nRecords;
    fileSize = li.fileSize;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearanceCount& CAppearanceCount::operator=(const CAppearanceCount& li) {
    duplicate(li);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAppearanceCount::operator==(const CAppearanceCount& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAppearanceCount& v1, const CAppearanceCount& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAppearanceCount> CAppearanceCountArray;
extern CArchive& operator>>(CArchive& archive, CAppearanceCountArray& array);
extern CArchive& operator<<(CArchive& archive, const CAppearanceCountArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAppearanceCount& lis);
extern CArchive& operator>>(CArchive& archive, CAppearanceCount& lis);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_APPEARANCECOUNT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
