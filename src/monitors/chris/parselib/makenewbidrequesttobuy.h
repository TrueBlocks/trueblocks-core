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
#include <vector>
#include <map>
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMakeNewBidRequestToBUY : public CTransaction {
public:
    biguint_t _tokenAmount;
    biguint_t _dollarPrice;

public:
    CMakeNewBidRequestToBUY(void);
    CMakeNewBidRequestToBUY(const CMakeNewBidRequestToBUY& ma);
    virtual ~CMakeNewBidRequestToBUY(void);
    CMakeNewBidRequestToBUY& operator=(const CMakeNewBidRequestToBUY& ma);

    DECLARE_NODE(CMakeNewBidRequestToBUY);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMakeNewBidRequestToBUY& item) const;
    bool operator!=(const CMakeNewBidRequestToBUY& item) const { return !operator==(item); }
    friend bool operator<(const CMakeNewBidRequestToBUY& v1, const CMakeNewBidRequestToBUY& v2);
    friend ostream& operator<<(ostream& os, const CMakeNewBidRequestToBUY& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMakeNewBidRequestToBUY& ma);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMakeNewBidRequestToBUY::CMakeNewBidRequestToBUY(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMakeNewBidRequestToBUY::CMakeNewBidRequestToBUY(const CMakeNewBidRequestToBUY& ma) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ma);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMakeNewBidRequestToBUY::~CMakeNewBidRequestToBUY(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMakeNewBidRequestToBUY::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMakeNewBidRequestToBUY::initialize(void) {
    CTransaction::initialize();

    _tokenAmount = 0;
    _dollarPrice = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMakeNewBidRequestToBUY::duplicate(const CMakeNewBidRequestToBUY& ma) {
    clear();
    CTransaction::duplicate(ma);

    _tokenAmount = ma._tokenAmount;
    _dollarPrice = ma._dollarPrice;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMakeNewBidRequestToBUY& CMakeNewBidRequestToBUY::operator=(const CMakeNewBidRequestToBUY& ma) {
    duplicate(ma);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMakeNewBidRequestToBUY::operator==(const CMakeNewBidRequestToBUY& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMakeNewBidRequestToBUY& v1, const CMakeNewBidRequestToBUY& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMakeNewBidRequestToBUY> CMakeNewBidRequestToBUYArray;
extern CArchive& operator>>(CArchive& archive, CMakeNewBidRequestToBUYArray& array);
extern CArchive& operator<<(CArchive& archive, const CMakeNewBidRequestToBUYArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

