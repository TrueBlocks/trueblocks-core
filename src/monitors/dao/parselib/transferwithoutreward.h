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
class CTransferWithoutReward : public CTransaction {
public:
    address_t _to;
    biguint_t _value;

public:
    CTransferWithoutReward(void);
    CTransferWithoutReward(const CTransferWithoutReward& tr);
    virtual ~CTransferWithoutReward(void);
    CTransferWithoutReward& operator=(const CTransferWithoutReward& tr);

    DECLARE_NODE(CTransferWithoutReward);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransferWithoutReward& item) const;
    bool operator!=(const CTransferWithoutReward& item) const { return !operator==(item); }
    friend bool operator<(const CTransferWithoutReward& v1, const CTransferWithoutReward& v2);
    friend ostream& operator<<(ostream& os, const CTransferWithoutReward& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransferWithoutReward& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransferWithoutReward::CTransferWithoutReward(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransferWithoutReward::CTransferWithoutReward(const CTransferWithoutReward& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransferWithoutReward::~CTransferWithoutReward(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferWithoutReward::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferWithoutReward::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferWithoutReward::duplicate(const CTransferWithoutReward& tr) {
    clear();
    CTransaction::duplicate(tr);

    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransferWithoutReward& CTransferWithoutReward::operator=(const CTransferWithoutReward& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransferWithoutReward::operator==(const CTransferWithoutReward& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransferWithoutReward& v1, const CTransferWithoutReward& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransferWithoutReward> CTransferWithoutRewardArray;
extern CArchive& operator>>(CArchive& archive, CTransferWithoutRewardArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferWithoutRewardArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

