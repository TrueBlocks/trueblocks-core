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
class CRevokeTokenGrant : public CTransaction {
public:
    address_t _holder;
    biguint_t _grantId;

public:
    CRevokeTokenGrant(void);
    CRevokeTokenGrant(const CRevokeTokenGrant& re);
    virtual ~CRevokeTokenGrant(void);
    CRevokeTokenGrant& operator=(const CRevokeTokenGrant& re);

    DECLARE_NODE(CRevokeTokenGrant);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRevokeTokenGrant& item) const;
    bool operator!=(const CRevokeTokenGrant& item) const { return !operator==(item); }
    friend bool operator<(const CRevokeTokenGrant& v1, const CRevokeTokenGrant& v2);
    friend ostream& operator<<(ostream& os, const CRevokeTokenGrant& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRevokeTokenGrant& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRevokeTokenGrant::CRevokeTokenGrant(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRevokeTokenGrant::CRevokeTokenGrant(const CRevokeTokenGrant& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRevokeTokenGrant::~CRevokeTokenGrant(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokeTokenGrant::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokeTokenGrant::initialize(void) {
    CTransaction::initialize();

    _holder = "";
    _grantId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokeTokenGrant::duplicate(const CRevokeTokenGrant& re) {
    clear();
    CTransaction::duplicate(re);

    _holder = re._holder;
    _grantId = re._grantId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRevokeTokenGrant& CRevokeTokenGrant::operator=(const CRevokeTokenGrant& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRevokeTokenGrant::operator==(const CRevokeTokenGrant& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRevokeTokenGrant& v1, const CRevokeTokenGrant& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRevokeTokenGrant> CRevokeTokenGrantArray;
extern CArchive& operator>>(CArchive& archive, CRevokeTokenGrantArray& array);
extern CArchive& operator<<(CArchive& archive, const CRevokeTokenGrantArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

