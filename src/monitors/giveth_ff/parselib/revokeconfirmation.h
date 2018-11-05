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
class CRevokeConfirmation : public CTransaction {
public:
    biguint_t transactionId;

public:
    CRevokeConfirmation(void);
    CRevokeConfirmation(const CRevokeConfirmation& re);
    virtual ~CRevokeConfirmation(void);
    CRevokeConfirmation& operator=(const CRevokeConfirmation& re);

    DECLARE_NODE(CRevokeConfirmation);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRevokeConfirmation& item) const;
    bool operator!=(const CRevokeConfirmation& item) const { return !operator==(item); }
    friend bool operator<(const CRevokeConfirmation& v1, const CRevokeConfirmation& v2);
    friend ostream& operator<<(ostream& os, const CRevokeConfirmation& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRevokeConfirmation& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRevokeConfirmation::CRevokeConfirmation(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRevokeConfirmation::CRevokeConfirmation(const CRevokeConfirmation& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRevokeConfirmation::~CRevokeConfirmation(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokeConfirmation::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokeConfirmation::initialize(void) {
    CTransaction::initialize();

    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokeConfirmation::duplicate(const CRevokeConfirmation& re) {
    clear();
    CTransaction::duplicate(re);

    transactionId = re.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRevokeConfirmation& CRevokeConfirmation::operator=(const CRevokeConfirmation& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRevokeConfirmation::operator==(const CRevokeConfirmation& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRevokeConfirmation& v1, const CRevokeConfirmation& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRevokeConfirmation> CRevokeConfirmationArray;
extern CArchive& operator>>(CArchive& archive, CRevokeConfirmationArray& array);
extern CArchive& operator<<(CArchive& archive, const CRevokeConfirmationArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

