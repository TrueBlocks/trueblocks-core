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
class COnApprove : public CTransaction {
public:
    address_t _owner;
    address_t _spender;
    biguint_t _amount;

public:
    COnApprove(void);
    COnApprove(const COnApprove& on);
    virtual ~COnApprove(void);
    COnApprove& operator=(const COnApprove& on);

    DECLARE_NODE(COnApprove);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COnApprove& item) const;
    bool operator!=(const COnApprove& item) const { return !operator==(item); }
    friend bool operator<(const COnApprove& v1, const COnApprove& v2);
    friend ostream& operator<<(ostream& os, const COnApprove& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COnApprove& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COnApprove::COnApprove(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COnApprove::COnApprove(const COnApprove& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COnApprove::~COnApprove(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnApprove::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnApprove::initialize(void) {
    CTransaction::initialize();

    _owner = "";
    _spender = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnApprove::duplicate(const COnApprove& on) {
    clear();
    CTransaction::duplicate(on);

    _owner = on._owner;
    _spender = on._spender;
    _amount = on._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COnApprove& COnApprove::operator=(const COnApprove& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COnApprove::operator==(const COnApprove& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COnApprove& v1, const COnApprove& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COnApprove> COnApproveArray;
extern CArchive& operator>>(CArchive& archive, COnApproveArray& array);
extern CArchive& operator<<(CArchive& archive, const COnApproveArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

