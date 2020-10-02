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
class CDisplayApp : public CBaseNode {
  public:
    address_t address;
    blknum_t blockNumber;
    blknum_t transactionIndex;

  public:
    CDisplayApp(void);
    CDisplayApp(const CDisplayApp& di);
    virtual ~CDisplayApp(void);
    CDisplayApp& operator=(const CDisplayApp& di);

    DECLARE_NODE(CDisplayApp);

    // EXISTING_CODE
    CDisplayApp(address_t a, blknum_t b, blknum_t t) : address(a), blockNumber(b), transactionIndex(t) {
    }
    // EXISTING_CODE
    bool operator==(const CDisplayApp& app) const;
    bool operator!=(const CDisplayApp& app) const {
        return !operator==(app);
    }
    friend bool operator<(const CDisplayApp& v1, const CDisplayApp& v2);
    friend ostream& operator<<(ostream& os, const CDisplayApp& app);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDisplayApp& di);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDisplayApp::CDisplayApp(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDisplayApp::CDisplayApp(const CDisplayApp& di) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(di);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDisplayApp::~CDisplayApp(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisplayApp::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisplayApp::initialize(void) {
    CBaseNode::initialize();

    address = "";
    blockNumber = 0;
    transactionIndex = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisplayApp::duplicate(const CDisplayApp& di) {
    clear();
    CBaseNode::duplicate(di);

    address = di.address;
    blockNumber = di.blockNumber;
    transactionIndex = di.transactionIndex;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDisplayApp& CDisplayApp::operator=(const CDisplayApp& di) {
    duplicate(di);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDisplayApp::operator==(const CDisplayApp& app) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDisplayApp& v1, const CDisplayApp& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDisplayApp> CDisplayAppArray;
extern CArchive& operator>>(CArchive& archive, CDisplayAppArray& array);
extern CArchive& operator<<(CArchive& archive, const CDisplayAppArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_DISPLAYAPP;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
