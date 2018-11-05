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
class CRemoveClaim : public CTransaction {
public:
    address_t issuer;
    address_t subject;
    string_q key;

public:
    CRemoveClaim(void);
    CRemoveClaim(const CRemoveClaim& re);
    virtual ~CRemoveClaim(void);
    CRemoveClaim& operator=(const CRemoveClaim& re);

    DECLARE_NODE(CRemoveClaim);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRemoveClaim& item) const;
    bool operator!=(const CRemoveClaim& item) const { return !operator==(item); }
    friend bool operator<(const CRemoveClaim& v1, const CRemoveClaim& v2);
    friend ostream& operator<<(ostream& os, const CRemoveClaim& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRemoveClaim& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRemoveClaim::CRemoveClaim(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRemoveClaim::CRemoveClaim(const CRemoveClaim& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRemoveClaim::~CRemoveClaim(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemoveClaim::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemoveClaim::initialize(void) {
    CTransaction::initialize();

    issuer = "";
    subject = "";
    key = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemoveClaim::duplicate(const CRemoveClaim& re) {
    clear();
    CTransaction::duplicate(re);

    issuer = re.issuer;
    subject = re.subject;
    key = re.key;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRemoveClaim& CRemoveClaim::operator=(const CRemoveClaim& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRemoveClaim::operator==(const CRemoveClaim& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRemoveClaim& v1, const CRemoveClaim& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRemoveClaim> CRemoveClaimArray;
extern CArchive& operator>>(CArchive& archive, CRemoveClaimArray& array);
extern CArchive& operator<<(CArchive& archive, const CRemoveClaimArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

