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
class COnTransfer : public CTransaction {
public:
    address_t _from;
    address_t _to;
    biguint_t _amount;

public:
    COnTransfer(void);
    COnTransfer(const COnTransfer& on);
    virtual ~COnTransfer(void);
    COnTransfer& operator=(const COnTransfer& on);

    DECLARE_NODE(COnTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COnTransfer& item) const;
    bool operator!=(const COnTransfer& item) const { return !operator==(item); }
    friend bool operator<(const COnTransfer& v1, const COnTransfer& v2);
    friend ostream& operator<<(ostream& os, const COnTransfer& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COnTransfer& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COnTransfer::COnTransfer(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COnTransfer::COnTransfer(const COnTransfer& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COnTransfer::~COnTransfer(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnTransfer::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnTransfer::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _to = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnTransfer::duplicate(const COnTransfer& on) {
    clear();
    CTransaction::duplicate(on);

    _from = on._from;
    _to = on._to;
    _amount = on._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COnTransfer& COnTransfer::operator=(const COnTransfer& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COnTransfer::operator==(const COnTransfer& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COnTransfer& v1, const COnTransfer& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COnTransfer> COnTransferArray;
extern CArchive& operator>>(CArchive& archive, COnTransferArray& array);
extern CArchive& operator<<(CArchive& archive, const COnTransferArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

