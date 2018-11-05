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
class CSetVault : public CTransaction {
public:
    address_t _newVaultAddress;

public:
    CSetVault(void);
    CSetVault(const CSetVault& se);
    virtual ~CSetVault(void);
    CSetVault& operator=(const CSetVault& se);

    DECLARE_NODE(CSetVault);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetVault& item) const;
    bool operator!=(const CSetVault& item) const { return !operator==(item); }
    friend bool operator<(const CSetVault& v1, const CSetVault& v2);
    friend ostream& operator<<(ostream& os, const CSetVault& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetVault& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetVault::CSetVault(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetVault::CSetVault(const CSetVault& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetVault::~CSetVault(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetVault::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetVault::initialize(void) {
    CTransaction::initialize();

    _newVaultAddress = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetVault::duplicate(const CSetVault& se) {
    clear();
    CTransaction::duplicate(se);

    _newVaultAddress = se._newVaultAddress;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetVault& CSetVault::operator=(const CSetVault& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetVault::operator==(const CSetVault& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetVault& v1, const CSetVault& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetVault> CSetVaultArray;
extern CArchive& operator>>(CArchive& archive, CSetVaultArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetVaultArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

