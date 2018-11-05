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
class CMintReserve : public CTransaction {
public:
    address_t beneficiary;

public:
    CMintReserve(void);
    CMintReserve(const CMintReserve& mi);
    virtual ~CMintReserve(void);
    CMintReserve& operator=(const CMintReserve& mi);

    DECLARE_NODE(CMintReserve);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMintReserve& item) const;
    bool operator!=(const CMintReserve& item) const { return !operator==(item); }
    friend bool operator<(const CMintReserve& v1, const CMintReserve& v2);
    friend ostream& operator<<(ostream& os, const CMintReserve& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMintReserve& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMintReserve::CMintReserve(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMintReserve::CMintReserve(const CMintReserve& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMintReserve::~CMintReserve(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintReserve::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintReserve::initialize(void) {
    CTransaction::initialize();

    beneficiary = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintReserve::duplicate(const CMintReserve& mi) {
    clear();
    CTransaction::duplicate(mi);

    beneficiary = mi.beneficiary;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMintReserve& CMintReserve::operator=(const CMintReserve& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMintReserve::operator==(const CMintReserve& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMintReserve& v1, const CMintReserve& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMintReserve> CMintReserveArray;
extern CArchive& operator>>(CArchive& archive, CMintReserveArray& array);
extern CArchive& operator<<(CArchive& archive, const CMintReserveArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

