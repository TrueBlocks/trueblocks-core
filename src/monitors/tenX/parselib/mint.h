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
class CMint : public CTransaction {
public:
    address_t _to;
    biguint_t _amount;

public:
    CMint(void);
    CMint(const CMint& mi);
    virtual ~CMint(void);
    CMint& operator=(const CMint& mi);

    DECLARE_NODE(CMint);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMint& item) const;
    bool operator!=(const CMint& item) const { return !operator==(item); }
    friend bool operator<(const CMint& v1, const CMint& v2);
    friend ostream& operator<<(ostream& os, const CMint& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMint& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMint::CMint(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMint::CMint(const CMint& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMint::~CMint(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMint::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMint::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMint::duplicate(const CMint& mi) {
    clear();
    CTransaction::duplicate(mi);

    _to = mi._to;
    _amount = mi._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMint& CMint::operator=(const CMint& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMint::operator==(const CMint& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMint& v1, const CMint& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMint> CMintArray;
extern CArchive& operator>>(CArchive& archive, CMintArray& array);
extern CArchive& operator<<(CArchive& archive, const CMintArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

