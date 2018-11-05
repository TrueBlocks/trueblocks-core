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
class CNewContract : public CTransaction {
public:
    address_t _newContract;

public:
    CNewContract(void);
    CNewContract(const CNewContract& ne);
    virtual ~CNewContract(void);
    CNewContract& operator=(const CNewContract& ne);

    DECLARE_NODE(CNewContract);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewContract& item) const;
    bool operator!=(const CNewContract& item) const { return !operator==(item); }
    friend bool operator<(const CNewContract& v1, const CNewContract& v2);
    friend ostream& operator<<(ostream& os, const CNewContract& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewContract& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewContract::CNewContract(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewContract::CNewContract(const CNewContract& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewContract::~CNewContract(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewContract::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewContract::initialize(void) {
    CTransaction::initialize();

    _newContract = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewContract::duplicate(const CNewContract& ne) {
    clear();
    CTransaction::duplicate(ne);

    _newContract = ne._newContract;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewContract& CNewContract::operator=(const CNewContract& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewContract::operator==(const CNewContract& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewContract& v1, const CNewContract& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewContract> CNewContractArray;
extern CArchive& operator>>(CArchive& archive, CNewContractArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewContractArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

