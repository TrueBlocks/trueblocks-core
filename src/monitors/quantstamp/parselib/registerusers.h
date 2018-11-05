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
class CRegisterUsers : public CTransaction {
public:
    CAddressArray contributors;

public:
    CRegisterUsers(void);
    CRegisterUsers(const CRegisterUsers& re);
    virtual ~CRegisterUsers(void);
    CRegisterUsers& operator=(const CRegisterUsers& re);

    DECLARE_NODE(CRegisterUsers);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRegisterUsers& item) const;
    bool operator!=(const CRegisterUsers& item) const { return !operator==(item); }
    friend bool operator<(const CRegisterUsers& v1, const CRegisterUsers& v2);
    friend ostream& operator<<(ostream& os, const CRegisterUsers& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRegisterUsers& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRegisterUsers::CRegisterUsers(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRegisterUsers::CRegisterUsers(const CRegisterUsers& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRegisterUsers::~CRegisterUsers(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterUsers::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterUsers::initialize(void) {
    CTransaction::initialize();

    contributors.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterUsers::duplicate(const CRegisterUsers& re) {
    clear();
    CTransaction::duplicate(re);

    contributors = re.contributors;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRegisterUsers& CRegisterUsers::operator=(const CRegisterUsers& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRegisterUsers::operator==(const CRegisterUsers& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRegisterUsers& v1, const CRegisterUsers& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRegisterUsers> CRegisterUsersArray;
extern CArchive& operator>>(CArchive& archive, CRegisterUsersArray& array);
extern CArchive& operator<<(CArchive& archive, const CRegisterUsersArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

