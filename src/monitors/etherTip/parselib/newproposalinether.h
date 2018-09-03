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
class CNewProposalInEther : public CTransaction {
public:
    address_t beneficiary;
    biguint_t etherAmount;
    string_q JobDescription;
    string_q transactionBytecode;

public:
    CNewProposalInEther(void);
    CNewProposalInEther(const CNewProposalInEther& ne);
    virtual ~CNewProposalInEther(void);
    CNewProposalInEther& operator=(const CNewProposalInEther& ne);

    DECLARE_NODE(CNewProposalInEther);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewProposalInEther& item) const;
    bool operator!=(const CNewProposalInEther& item) const { return !operator==(item); }
    friend bool operator<(const CNewProposalInEther& v1, const CNewProposalInEther& v2);
    friend ostream& operator<<(ostream& os, const CNewProposalInEther& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewProposalInEther& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewProposalInEther::CNewProposalInEther(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewProposalInEther::CNewProposalInEther(const CNewProposalInEther& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewProposalInEther::~CNewProposalInEther(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposalInEther::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposalInEther::initialize(void) {
    CTransaction::initialize();

    beneficiary = "";
    etherAmount = 0;
    JobDescription = "";
    transactionBytecode = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposalInEther::duplicate(const CNewProposalInEther& ne) {
    clear();
    CTransaction::duplicate(ne);

    beneficiary = ne.beneficiary;
    etherAmount = ne.etherAmount;
    JobDescription = ne.JobDescription;
    transactionBytecode = ne.transactionBytecode;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewProposalInEther& CNewProposalInEther::operator=(const CNewProposalInEther& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewProposalInEther::operator==(const CNewProposalInEther& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewProposalInEther& v1, const CNewProposalInEther& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewProposalInEther> CNewProposalInEtherArray;
extern CArchive& operator>>(CArchive& archive, CNewProposalInEtherArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewProposalInEtherArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

