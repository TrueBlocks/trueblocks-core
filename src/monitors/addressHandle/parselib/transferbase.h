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
class CTransferBase : public CTransaction {
public:
    string_q _base;
    address_t _newAddress;

public:
    CTransferBase(void);
    CTransferBase(const CTransferBase& tr);
    virtual ~CTransferBase(void);
    CTransferBase& operator=(const CTransferBase& tr);

    DECLARE_NODE(CTransferBase);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransferBase& item) const;
    bool operator!=(const CTransferBase& item) const { return !operator==(item); }
    friend bool operator<(const CTransferBase& v1, const CTransferBase& v2);
    friend ostream& operator<<(ostream& os, const CTransferBase& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransferBase& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransferBase::CTransferBase(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransferBase::CTransferBase(const CTransferBase& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransferBase::~CTransferBase(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferBase::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferBase::initialize(void) {
    CTransaction::initialize();

    _base = "";
    _newAddress = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferBase::duplicate(const CTransferBase& tr) {
    clear();
    CTransaction::duplicate(tr);

    _base = tr._base;
    _newAddress = tr._newAddress;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransferBase& CTransferBase::operator=(const CTransferBase& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransferBase::operator==(const CTransferBase& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransferBase& v1, const CTransferBase& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransferBase> CTransferBaseArray;
extern CArchive& operator>>(CArchive& archive, CTransferBaseArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferBaseArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

