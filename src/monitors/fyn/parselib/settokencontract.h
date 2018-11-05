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
class CSetTokenContract : public CTransaction {
public:
    address_t newTokenContractAddr;

public:
    CSetTokenContract(void);
    CSetTokenContract(const CSetTokenContract& se);
    virtual ~CSetTokenContract(void);
    CSetTokenContract& operator=(const CSetTokenContract& se);

    DECLARE_NODE(CSetTokenContract);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetTokenContract& item) const;
    bool operator!=(const CSetTokenContract& item) const { return !operator==(item); }
    friend bool operator<(const CSetTokenContract& v1, const CSetTokenContract& v2);
    friend ostream& operator<<(ostream& os, const CSetTokenContract& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetTokenContract& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetTokenContract::CSetTokenContract(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetTokenContract::CSetTokenContract(const CSetTokenContract& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetTokenContract::~CSetTokenContract(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetTokenContract::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetTokenContract::initialize(void) {
    CTransaction::initialize();

    newTokenContractAddr = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetTokenContract::duplicate(const CSetTokenContract& se) {
    clear();
    CTransaction::duplicate(se);

    newTokenContractAddr = se.newTokenContractAddr;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetTokenContract& CSetTokenContract::operator=(const CSetTokenContract& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetTokenContract::operator==(const CSetTokenContract& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetTokenContract& v1, const CSetTokenContract& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetTokenContract> CSetTokenContractArray;
extern CArchive& operator>>(CArchive& archive, CSetTokenContractArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetTokenContractArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

