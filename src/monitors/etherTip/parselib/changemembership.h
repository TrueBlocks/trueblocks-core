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
class CChangeMembership : public CTransaction {
public:
    address_t targetMember;
    biguint_t voteWeight;
    bool canAddProposals;
    string_q memberName;

public:
    CChangeMembership(void);
    CChangeMembership(const CChangeMembership& ch);
    virtual ~CChangeMembership(void);
    CChangeMembership& operator=(const CChangeMembership& ch);

    DECLARE_NODE(CChangeMembership);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeMembership& item) const;
    bool operator!=(const CChangeMembership& item) const { return !operator==(item); }
    friend bool operator<(const CChangeMembership& v1, const CChangeMembership& v2);
    friend ostream& operator<<(ostream& os, const CChangeMembership& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeMembership& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeMembership::CChangeMembership(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeMembership::CChangeMembership(const CChangeMembership& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeMembership::~CChangeMembership(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeMembership::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeMembership::initialize(void) {
    CTransaction::initialize();

    targetMember = "";
    voteWeight = 0;
    canAddProposals = 0;
    memberName = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeMembership::duplicate(const CChangeMembership& ch) {
    clear();
    CTransaction::duplicate(ch);

    targetMember = ch.targetMember;
    voteWeight = ch.voteWeight;
    canAddProposals = ch.canAddProposals;
    memberName = ch.memberName;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeMembership& CChangeMembership::operator=(const CChangeMembership& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeMembership::operator==(const CChangeMembership& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeMembership& v1, const CChangeMembership& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeMembership> CChangeMembershipArray;
extern CArchive& operator>>(CArchive& archive, CChangeMembershipArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeMembershipArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

