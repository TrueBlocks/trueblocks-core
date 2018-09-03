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
class CNewProposalInWei : public CTransaction {
public:
    address_t beneficiary;
    biguint_t weiAmount;
    string_q JobDescription;
    string_q transactionBytecode;

public:
    CNewProposalInWei(void);
    CNewProposalInWei(const CNewProposalInWei& ne);
    virtual ~CNewProposalInWei(void);
    CNewProposalInWei& operator=(const CNewProposalInWei& ne);

    DECLARE_NODE(CNewProposalInWei);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewProposalInWei& item) const;
    bool operator!=(const CNewProposalInWei& item) const { return !operator==(item); }
    friend bool operator<(const CNewProposalInWei& v1, const CNewProposalInWei& v2);
    friend ostream& operator<<(ostream& os, const CNewProposalInWei& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewProposalInWei& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewProposalInWei::CNewProposalInWei(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewProposalInWei::CNewProposalInWei(const CNewProposalInWei& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewProposalInWei::~CNewProposalInWei(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposalInWei::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposalInWei::initialize(void) {
    CTransaction::initialize();

    beneficiary = "";
    weiAmount = 0;
    JobDescription = "";
    transactionBytecode = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewProposalInWei::duplicate(const CNewProposalInWei& ne) {
    clear();
    CTransaction::duplicate(ne);

    beneficiary = ne.beneficiary;
    weiAmount = ne.weiAmount;
    JobDescription = ne.JobDescription;
    transactionBytecode = ne.transactionBytecode;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewProposalInWei& CNewProposalInWei::operator=(const CNewProposalInWei& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewProposalInWei::operator==(const CNewProposalInWei& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewProposalInWei& v1, const CNewProposalInWei& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewProposalInWei> CNewProposalInWeiArray;
extern CArchive& operator>>(CArchive& archive, CNewProposalInWeiArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewProposalInWeiArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

