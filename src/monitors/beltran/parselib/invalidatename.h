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
class CInvalidateName : public CTransaction {
public:
    string_q unhashedName;

public:
    CInvalidateName(void);
    CInvalidateName(const CInvalidateName& in);
    virtual ~CInvalidateName(void);
    CInvalidateName& operator=(const CInvalidateName& in);

    DECLARE_NODE(CInvalidateName);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CInvalidateName& item) const;
    bool operator!=(const CInvalidateName& item) const { return !operator==(item); }
    friend bool operator<(const CInvalidateName& v1, const CInvalidateName& v2);
    friend ostream& operator<<(ostream& os, const CInvalidateName& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CInvalidateName& in);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CInvalidateName::CInvalidateName(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CInvalidateName::CInvalidateName(const CInvalidateName& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CInvalidateName::~CInvalidateName(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInvalidateName::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInvalidateName::initialize(void) {
    CTransaction::initialize();

    unhashedName = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInvalidateName::duplicate(const CInvalidateName& in) {
    clear();
    CTransaction::duplicate(in);

    unhashedName = in.unhashedName;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CInvalidateName& CInvalidateName::operator=(const CInvalidateName& in) {
    duplicate(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CInvalidateName::operator==(const CInvalidateName& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CInvalidateName& v1, const CInvalidateName& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CInvalidateName> CInvalidateNameArray;
extern CArchive& operator>>(CArchive& archive, CInvalidateNameArray& array);
extern CArchive& operator<<(CArchive& archive, const CInvalidateNameArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

