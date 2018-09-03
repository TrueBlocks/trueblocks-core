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
class CChangeAdmin : public CTransaction {
public:
    address_t admin_;

public:
    CChangeAdmin(void);
    CChangeAdmin(const CChangeAdmin& ch);
    virtual ~CChangeAdmin(void);
    CChangeAdmin& operator=(const CChangeAdmin& ch);

    DECLARE_NODE(CChangeAdmin);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeAdmin& item) const;
    bool operator!=(const CChangeAdmin& item) const { return !operator==(item); }
    friend bool operator<(const CChangeAdmin& v1, const CChangeAdmin& v2);
    friend ostream& operator<<(ostream& os, const CChangeAdmin& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeAdmin& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeAdmin::CChangeAdmin(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeAdmin::CChangeAdmin(const CChangeAdmin& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeAdmin::~CChangeAdmin(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAdmin::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAdmin::initialize(void) {
    CTransaction::initialize();

    admin_ = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAdmin::duplicate(const CChangeAdmin& ch) {
    clear();
    CTransaction::duplicate(ch);

    admin_ = ch.admin_;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeAdmin& CChangeAdmin::operator=(const CChangeAdmin& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeAdmin::operator==(const CChangeAdmin& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeAdmin& v1, const CChangeAdmin& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeAdmin> CChangeAdminArray;
extern CArchive& operator>>(CArchive& archive, CChangeAdminArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeAdminArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

