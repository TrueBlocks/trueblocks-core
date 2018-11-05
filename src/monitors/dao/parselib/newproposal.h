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
class CNewProposal : public CTransaction {
public:
    address_t _recipient;
    biguint_t _amount;
    string_q _description;
    string_q _transactionData;
    biguint_t _debatingPeriod;
    bool _newCurator;

public:
    CNewProposal(void);
    CNewProposal(const CNewProposal& ne);
    virtual ~CNewProposal(void);
    CNewProposal& operator=(const CNewProposal& ne);

    DECLARE_NODE(CNewProposal);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewProposal& item) const;
    bool operator!=(const CNewProposal& item) const { return !operator==(item); }
    friend bool operator<(const CNewProposal& v1, const CNewProposal& v2);
    friend ostream& operator<<(ostream& os, const CNewProposal& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewProposal& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewProposal::CNewProposal(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewProposal::CNewProposal(const CNewProposal& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewProposal::~CNewProposal(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposal::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposal::initialize(void) {
    CTransaction::initialize();

    _recipient = "";
    _amount = 0;
    _description = "";
    _transactionData = "";
    _debatingPeriod = 0;
    _newCurator = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposal::duplicate(const CNewProposal& ne) {
    clear();
    CTransaction::duplicate(ne);

    _recipient = ne._recipient;
    _amount = ne._amount;
    _description = ne._description;
    _transactionData = ne._transactionData;
    _debatingPeriod = ne._debatingPeriod;
    _newCurator = ne._newCurator;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewProposal& CNewProposal::operator=(const CNewProposal& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewProposal::operator==(const CNewProposal& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewProposal& v1, const CNewProposal& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewProposal> CNewProposalArray;
extern CArchive& operator>>(CArchive& archive, CNewProposalArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewProposalArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

