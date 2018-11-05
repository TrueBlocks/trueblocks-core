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
class CAffirmStatusAsAccreditedInvestor : public CTransaction {
public:
    address_t _account;

public:
    CAffirmStatusAsAccreditedInvestor(void);
    CAffirmStatusAsAccreditedInvestor(const CAffirmStatusAsAccreditedInvestor& af);
    virtual ~CAffirmStatusAsAccreditedInvestor(void);
    CAffirmStatusAsAccreditedInvestor& operator=(const CAffirmStatusAsAccreditedInvestor& af);

    DECLARE_NODE(CAffirmStatusAsAccreditedInvestor);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAffirmStatusAsAccreditedInvestor& item) const;
    bool operator!=(const CAffirmStatusAsAccreditedInvestor& item) const { return !operator==(item); }
    friend bool operator<(const CAffirmStatusAsAccreditedInvestor& v1, const CAffirmStatusAsAccreditedInvestor& v2);
    friend ostream& operator<<(ostream& os, const CAffirmStatusAsAccreditedInvestor& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAffirmStatusAsAccreditedInvestor& af);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAffirmStatusAsAccreditedInvestor::CAffirmStatusAsAccreditedInvestor(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAffirmStatusAsAccreditedInvestor::CAffirmStatusAsAccreditedInvestor(const CAffirmStatusAsAccreditedInvestor& af) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(af);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAffirmStatusAsAccreditedInvestor::~CAffirmStatusAsAccreditedInvestor(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAffirmStatusAsAccreditedInvestor::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAffirmStatusAsAccreditedInvestor::initialize(void) {
    CTransaction::initialize();

    _account = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAffirmStatusAsAccreditedInvestor::duplicate(const CAffirmStatusAsAccreditedInvestor& af) {
    clear();
    CTransaction::duplicate(af);

    _account = af._account;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAffirmStatusAsAccreditedInvestor& CAffirmStatusAsAccreditedInvestor::operator=(const CAffirmStatusAsAccreditedInvestor& af) {
    duplicate(af);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAffirmStatusAsAccreditedInvestor::operator==(const CAffirmStatusAsAccreditedInvestor& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAffirmStatusAsAccreditedInvestor& v1, const CAffirmStatusAsAccreditedInvestor& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAffirmStatusAsAccreditedInvestor> CAffirmStatusAsAccreditedInvestorArray;
extern CArchive& operator>>(CArchive& archive, CAffirmStatusAsAccreditedInvestorArray& array);
extern CArchive& operator<<(CArchive& archive, const CAffirmStatusAsAccreditedInvestorArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

