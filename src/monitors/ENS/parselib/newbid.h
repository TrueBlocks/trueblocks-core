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
class CNewBid : public CTransaction {
public:
    string_q sealedBid;

public:
    CNewBid(void);
    CNewBid(const CNewBid& ne);
    virtual ~CNewBid(void);
    CNewBid& operator=(const CNewBid& ne);

    DECLARE_NODE(CNewBid);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewBid& item) const;
    bool operator!=(const CNewBid& item) const { return !operator==(item); }
    friend bool operator<(const CNewBid& v1, const CNewBid& v2);
    friend ostream& operator<<(ostream& os, const CNewBid& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewBid& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewBid::CNewBid(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewBid::CNewBid(const CNewBid& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewBid::~CNewBid(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBid::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBid::initialize(void) {
    CTransaction::initialize();

    sealedBid = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBid::duplicate(const CNewBid& ne) {
    clear();
    CTransaction::duplicate(ne);

    sealedBid = ne.sealedBid;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewBid& CNewBid::operator=(const CNewBid& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewBid::operator==(const CNewBid& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewBid& v1, const CNewBid& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewBid> CNewBidArray;
extern CArchive& operator>>(CArchive& archive, CNewBidArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewBidArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

