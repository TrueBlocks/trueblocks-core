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
class CTransferX : public CTransaction {
public:
    address_t _to;
    biguint_t _amount;
    string_q _transferMetaData;

public:
    CTransferX(void);
    CTransferX(const CTransferX& tr);
    virtual ~CTransferX(void);
    CTransferX& operator=(const CTransferX& tr);

    DECLARE_NODE(CTransferX);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransferX& item) const;
    bool operator!=(const CTransferX& item) const { return !operator==(item); }
    friend bool operator<(const CTransferX& v1, const CTransferX& v2);
    friend ostream& operator<<(ostream& os, const CTransferX& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransferX& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransferX::CTransferX(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransferX::CTransferX(const CTransferX& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransferX::~CTransferX(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferX::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferX::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _amount = 0;
    _transferMetaData = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferX::duplicate(const CTransferX& tr) {
    clear();
    CTransaction::duplicate(tr);

    _to = tr._to;
    _amount = tr._amount;
    _transferMetaData = tr._transferMetaData;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransferX& CTransferX::operator=(const CTransferX& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransferX::operator==(const CTransferX& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransferX& v1, const CTransferX& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransferX> CTransferXArray;
extern CArchive& operator>>(CArchive& archive, CTransferXArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferXArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

