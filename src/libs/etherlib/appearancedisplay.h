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
class CAppearanceDisplay : public CBaseNode {
  public:
    address_t address;
    blknum_t blockNumber;
    blknum_t transactionIndex;

  public:
    CAppearanceDisplay(void);
    CAppearanceDisplay(const CAppearanceDisplay& ap);
    virtual ~CAppearanceDisplay(void);
    CAppearanceDisplay& operator=(const CAppearanceDisplay& ap);

    DECLARE_NODE(CAppearanceDisplay);

    // EXISTING_CODE
    CAppearanceDisplay(address_t a, blknum_t b, blknum_t t) : address(a), blockNumber(b), transactionIndex(t) {
    }
    // EXISTING_CODE
    bool operator==(const CAppearanceDisplay& it) const;
    bool operator!=(const CAppearanceDisplay& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAppearanceDisplay& v1, const CAppearanceDisplay& v2);
    friend ostream& operator<<(ostream& os, const CAppearanceDisplay& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAppearanceDisplay& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAppearanceDisplay::CAppearanceDisplay(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearanceDisplay::CAppearanceDisplay(const CAppearanceDisplay& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAppearanceDisplay::~CAppearanceDisplay(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearanceDisplay::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearanceDisplay::initialize(void) {
    CBaseNode::initialize();

    address = "";
    blockNumber = 0;
    transactionIndex = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearanceDisplay::duplicate(const CAppearanceDisplay& ap) {
    clear();
    CBaseNode::duplicate(ap);

    address = ap.address;
    blockNumber = ap.blockNumber;
    transactionIndex = ap.transactionIndex;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearanceDisplay& CAppearanceDisplay::operator=(const CAppearanceDisplay& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAppearanceDisplay::operator==(const CAppearanceDisplay& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAppearanceDisplay& v1, const CAppearanceDisplay& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAppearanceDisplay> CAppearanceDisplayArray;
extern CArchive& operator>>(CArchive& archive, CAppearanceDisplayArray& array);
extern CArchive& operator<<(CArchive& archive, const CAppearanceDisplayArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_APPEARANCEDISPLAY;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
