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
class CChangeVotingRules : public CTransaction {
public:
    biguint_t minimumQuorumForProposals;
    biguint_t minutesForDebate;
    bigint_t marginOfVotesForMajority;

public:
    CChangeVotingRules(void);
    CChangeVotingRules(const CChangeVotingRules& ch);
    virtual ~CChangeVotingRules(void);
    CChangeVotingRules& operator=(const CChangeVotingRules& ch);

    DECLARE_NODE(CChangeVotingRules);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeVotingRules& item) const;
    bool operator!=(const CChangeVotingRules& item) const { return !operator==(item); }
    friend bool operator<(const CChangeVotingRules& v1, const CChangeVotingRules& v2);
    friend ostream& operator<<(ostream& os, const CChangeVotingRules& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeVotingRules& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeVotingRules::CChangeVotingRules(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeVotingRules::CChangeVotingRules(const CChangeVotingRules& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeVotingRules::~CChangeVotingRules(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeVotingRules::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeVotingRules::initialize(void) {
    CTransaction::initialize();

    minimumQuorumForProposals = 0;
    minutesForDebate = 0;
    marginOfVotesForMajority = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeVotingRules::duplicate(const CChangeVotingRules& ch) {
    clear();
    CTransaction::duplicate(ch);

    minimumQuorumForProposals = ch.minimumQuorumForProposals;
    minutesForDebate = ch.minutesForDebate;
    marginOfVotesForMajority = ch.marginOfVotesForMajority;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeVotingRules& CChangeVotingRules::operator=(const CChangeVotingRules& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeVotingRules::operator==(const CChangeVotingRules& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeVotingRules& v1, const CChangeVotingRules& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeVotingRules> CChangeVotingRulesArray;
extern CArchive& operator>>(CArchive& archive, CChangeVotingRulesArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeVotingRulesArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

