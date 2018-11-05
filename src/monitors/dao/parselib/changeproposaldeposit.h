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
class CChangeProposalDeposit : public CTransaction {
public:
    biguint_t _proposalDeposit;

public:
    CChangeProposalDeposit(void);
    CChangeProposalDeposit(const CChangeProposalDeposit& ch);
    virtual ~CChangeProposalDeposit(void);
    CChangeProposalDeposit& operator=(const CChangeProposalDeposit& ch);

    DECLARE_NODE(CChangeProposalDeposit);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeProposalDeposit& item) const;
    bool operator!=(const CChangeProposalDeposit& item) const { return !operator==(item); }
    friend bool operator<(const CChangeProposalDeposit& v1, const CChangeProposalDeposit& v2);
    friend ostream& operator<<(ostream& os, const CChangeProposalDeposit& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeProposalDeposit& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeProposalDeposit::CChangeProposalDeposit(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeProposalDeposit::CChangeProposalDeposit(const CChangeProposalDeposit& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeProposalDeposit::~CChangeProposalDeposit(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeProposalDeposit::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeProposalDeposit::initialize(void) {
    CTransaction::initialize();

    _proposalDeposit = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeProposalDeposit::duplicate(const CChangeProposalDeposit& ch) {
    clear();
    CTransaction::duplicate(ch);

    _proposalDeposit = ch._proposalDeposit;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeProposalDeposit& CChangeProposalDeposit::operator=(const CChangeProposalDeposit& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeProposalDeposit::operator==(const CChangeProposalDeposit& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeProposalDeposit& v1, const CChangeProposalDeposit& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeProposalDeposit> CChangeProposalDepositArray;
extern CArchive& operator>>(CArchive& archive, CChangeProposalDepositArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeProposalDepositArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

