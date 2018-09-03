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
class CTransferRegistrars : public CTransaction {
public:
    string_q _hash;

public:
    CTransferRegistrars(void);
    CTransferRegistrars(const CTransferRegistrars& tr);
    virtual ~CTransferRegistrars(void);
    CTransferRegistrars& operator=(const CTransferRegistrars& tr);

    DECLARE_NODE(CTransferRegistrars);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransferRegistrars& item) const;
    bool operator!=(const CTransferRegistrars& item) const { return !operator==(item); }
    friend bool operator<(const CTransferRegistrars& v1, const CTransferRegistrars& v2);
    friend ostream& operator<<(ostream& os, const CTransferRegistrars& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransferRegistrars& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransferRegistrars::CTransferRegistrars(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransferRegistrars::CTransferRegistrars(const CTransferRegistrars& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransferRegistrars::~CTransferRegistrars(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferRegistrars::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferRegistrars::initialize(void) {
    CTransaction::initialize();

    _hash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferRegistrars::duplicate(const CTransferRegistrars& tr) {
    clear();
    CTransaction::duplicate(tr);

    _hash = tr._hash;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransferRegistrars& CTransferRegistrars::operator=(const CTransferRegistrars& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransferRegistrars::operator==(const CTransferRegistrars& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransferRegistrars& v1, const CTransferRegistrars& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransferRegistrars> CTransferRegistrarsArray;
extern CArchive& operator>>(CArchive& archive, CTransferRegistrarsArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferRegistrarsArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

