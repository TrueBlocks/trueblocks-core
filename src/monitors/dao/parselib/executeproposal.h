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
class CExecuteProposal : public CTransaction {
public:
    biguint_t _proposalID;
    string_q _transactionData;

public:
    CExecuteProposal(void);
    CExecuteProposal(const CExecuteProposal& ex);
    virtual ~CExecuteProposal(void);
    CExecuteProposal& operator=(const CExecuteProposal& ex);

    DECLARE_NODE(CExecuteProposal);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CExecuteProposal& item) const;
    bool operator!=(const CExecuteProposal& item) const { return !operator==(item); }
    friend bool operator<(const CExecuteProposal& v1, const CExecuteProposal& v2);
    friend ostream& operator<<(ostream& os, const CExecuteProposal& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CExecuteProposal& ex);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CExecuteProposal::CExecuteProposal(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CExecuteProposal::CExecuteProposal(const CExecuteProposal& ex) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ex);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CExecuteProposal::~CExecuteProposal(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecuteProposal::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecuteProposal::initialize(void) {
    CTransaction::initialize();

    _proposalID = 0;
    _transactionData = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecuteProposal::duplicate(const CExecuteProposal& ex) {
    clear();
    CTransaction::duplicate(ex);

    _proposalID = ex._proposalID;
    _transactionData = ex._transactionData;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CExecuteProposal& CExecuteProposal::operator=(const CExecuteProposal& ex) {
    duplicate(ex);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CExecuteProposal::operator==(const CExecuteProposal& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CExecuteProposal& v1, const CExecuteProposal& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CExecuteProposal> CExecuteProposalArray;
extern CArchive& operator>>(CArchive& archive, CExecuteProposalArray& array);
extern CArchive& operator<<(CArchive& archive, const CExecuteProposalArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

