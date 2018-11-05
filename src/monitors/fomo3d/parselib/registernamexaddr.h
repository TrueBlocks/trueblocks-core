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
class CRegisterNameXaddr : public CTransaction {
public:
    string_q _nameString;
    address_t _affCode;
    bool _all;

public:
    CRegisterNameXaddr(void);
    CRegisterNameXaddr(const CRegisterNameXaddr& re);
    virtual ~CRegisterNameXaddr(void);
    CRegisterNameXaddr& operator=(const CRegisterNameXaddr& re);

    DECLARE_NODE(CRegisterNameXaddr);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRegisterNameXaddr& item) const;
    bool operator!=(const CRegisterNameXaddr& item) const { return !operator==(item); }
    friend bool operator<(const CRegisterNameXaddr& v1, const CRegisterNameXaddr& v2);
    friend ostream& operator<<(ostream& os, const CRegisterNameXaddr& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRegisterNameXaddr& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRegisterNameXaddr::CRegisterNameXaddr(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRegisterNameXaddr::CRegisterNameXaddr(const CRegisterNameXaddr& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRegisterNameXaddr::~CRegisterNameXaddr(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterNameXaddr::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterNameXaddr::initialize(void) {
    CTransaction::initialize();

    _nameString = "";
    _affCode = "";
    _all = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterNameXaddr::duplicate(const CRegisterNameXaddr& re) {
    clear();
    CTransaction::duplicate(re);

    _nameString = re._nameString;
    _affCode = re._affCode;
    _all = re._all;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRegisterNameXaddr& CRegisterNameXaddr::operator=(const CRegisterNameXaddr& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRegisterNameXaddr::operator==(const CRegisterNameXaddr& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRegisterNameXaddr& v1, const CRegisterNameXaddr& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRegisterNameXaddr> CRegisterNameXaddrArray;
extern CArchive& operator>>(CArchive& archive, CRegisterNameXaddrArray& array);
extern CArchive& operator<<(CArchive& archive, const CRegisterNameXaddrArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

