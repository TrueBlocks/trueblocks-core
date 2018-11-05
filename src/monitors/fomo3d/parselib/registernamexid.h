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
class CRegisterNameXID : public CTransaction {
public:
    string_q _nameString;
    biguint_t _affCode;
    bool _all;

public:
    CRegisterNameXID(void);
    CRegisterNameXID(const CRegisterNameXID& re);
    virtual ~CRegisterNameXID(void);
    CRegisterNameXID& operator=(const CRegisterNameXID& re);

    DECLARE_NODE(CRegisterNameXID);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRegisterNameXID& item) const;
    bool operator!=(const CRegisterNameXID& item) const { return !operator==(item); }
    friend bool operator<(const CRegisterNameXID& v1, const CRegisterNameXID& v2);
    friend ostream& operator<<(ostream& os, const CRegisterNameXID& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRegisterNameXID& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRegisterNameXID::CRegisterNameXID(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRegisterNameXID::CRegisterNameXID(const CRegisterNameXID& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRegisterNameXID::~CRegisterNameXID(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterNameXID::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterNameXID::initialize(void) {
    CTransaction::initialize();

    _nameString = "";
    _affCode = 0;
    _all = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterNameXID::duplicate(const CRegisterNameXID& re) {
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
inline CRegisterNameXID& CRegisterNameXID::operator=(const CRegisterNameXID& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRegisterNameXID::operator==(const CRegisterNameXID& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRegisterNameXID& v1, const CRegisterNameXID& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRegisterNameXID> CRegisterNameXIDArray;
extern CArchive& operator>>(CArchive& archive, CRegisterNameXIDArray& array);
extern CArchive& operator<<(CArchive& archive, const CRegisterNameXIDArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

