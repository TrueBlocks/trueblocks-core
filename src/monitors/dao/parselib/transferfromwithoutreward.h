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
class CTransferFromWithoutReward : public CTransaction {
public:
    address_t _from;
    address_t _to;
    biguint_t _value;

public:
    CTransferFromWithoutReward(void);
    CTransferFromWithoutReward(const CTransferFromWithoutReward& tr);
    virtual ~CTransferFromWithoutReward(void);
    CTransferFromWithoutReward& operator=(const CTransferFromWithoutReward& tr);

    DECLARE_NODE(CTransferFromWithoutReward);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransferFromWithoutReward& item) const;
    bool operator!=(const CTransferFromWithoutReward& item) const { return !operator==(item); }
    friend bool operator<(const CTransferFromWithoutReward& v1, const CTransferFromWithoutReward& v2);
    friend ostream& operator<<(ostream& os, const CTransferFromWithoutReward& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransferFromWithoutReward& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransferFromWithoutReward::CTransferFromWithoutReward(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransferFromWithoutReward::CTransferFromWithoutReward(const CTransferFromWithoutReward& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransferFromWithoutReward::~CTransferFromWithoutReward(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferFromWithoutReward::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferFromWithoutReward::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferFromWithoutReward::duplicate(const CTransferFromWithoutReward& tr) {
    clear();
    CTransaction::duplicate(tr);

    _from = tr._from;
    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransferFromWithoutReward& CTransferFromWithoutReward::operator=(const CTransferFromWithoutReward& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransferFromWithoutReward::operator==(const CTransferFromWithoutReward& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransferFromWithoutReward& v1, const CTransferFromWithoutReward& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransferFromWithoutReward> CTransferFromWithoutRewardArray;
extern CArchive& operator>>(CArchive& archive, CTransferFromWithoutRewardArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferFromWithoutRewardArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

