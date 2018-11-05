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
class CSetSelfClaim : public CTransaction {
public:
    string_q key;
    string_q value;

public:
    CSetSelfClaim(void);
    CSetSelfClaim(const CSetSelfClaim& se);
    virtual ~CSetSelfClaim(void);
    CSetSelfClaim& operator=(const CSetSelfClaim& se);

    DECLARE_NODE(CSetSelfClaim);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetSelfClaim& item) const;
    bool operator!=(const CSetSelfClaim& item) const { return !operator==(item); }
    friend bool operator<(const CSetSelfClaim& v1, const CSetSelfClaim& v2);
    friend ostream& operator<<(ostream& os, const CSetSelfClaim& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetSelfClaim& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetSelfClaim::CSetSelfClaim(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetSelfClaim::CSetSelfClaim(const CSetSelfClaim& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetSelfClaim::~CSetSelfClaim(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetSelfClaim::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetSelfClaim::initialize(void) {
    CTransaction::initialize();

    key = "";
    value = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetSelfClaim::duplicate(const CSetSelfClaim& se) {
    clear();
    CTransaction::duplicate(se);

    key = se.key;
    value = se.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetSelfClaim& CSetSelfClaim::operator=(const CSetSelfClaim& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetSelfClaim::operator==(const CSetSelfClaim& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetSelfClaim& v1, const CSetSelfClaim& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetSelfClaim> CSetSelfClaimArray;
extern CArchive& operator>>(CArchive& archive, CSetSelfClaimArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetSelfClaimArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

