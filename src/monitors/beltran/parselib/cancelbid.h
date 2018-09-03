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
class CCancelBid : public CTransaction {
public:
    address_t bidder;
    string_q seal;

public:
    CCancelBid(void);
    CCancelBid(const CCancelBid& ca);
    virtual ~CCancelBid(void);
    CCancelBid& operator=(const CCancelBid& ca);

    DECLARE_NODE(CCancelBid);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCancelBid& item) const;
    bool operator!=(const CCancelBid& item) const { return !operator==(item); }
    friend bool operator<(const CCancelBid& v1, const CCancelBid& v2);
    friend ostream& operator<<(ostream& os, const CCancelBid& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCancelBid& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCancelBid::CCancelBid(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCancelBid::CCancelBid(const CCancelBid& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCancelBid::~CCancelBid(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelBid::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelBid::initialize(void) {
    CTransaction::initialize();

    bidder = "";
    seal = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelBid::duplicate(const CCancelBid& ca) {
    clear();
    CTransaction::duplicate(ca);

    bidder = ca.bidder;
    seal = ca.seal;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCancelBid& CCancelBid::operator=(const CCancelBid& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCancelBid::operator==(const CCancelBid& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCancelBid& v1, const CCancelBid& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCancelBid> CCancelBidArray;
extern CArchive& operator>>(CArchive& archive, CCancelBidArray& array);
extern CArchive& operator<<(CArchive& archive, const CCancelBidArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

