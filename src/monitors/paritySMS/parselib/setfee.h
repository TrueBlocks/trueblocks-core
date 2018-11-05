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
class CSetFee : public CTransaction {
public:
    biguint_t _new;

public:
    CSetFee(void);
    CSetFee(const CSetFee& se);
    virtual ~CSetFee(void);
    CSetFee& operator=(const CSetFee& se);

    DECLARE_NODE(CSetFee);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetFee& item) const;
    bool operator!=(const CSetFee& item) const { return !operator==(item); }
    friend bool operator<(const CSetFee& v1, const CSetFee& v2);
    friend ostream& operator<<(ostream& os, const CSetFee& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetFee& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetFee::CSetFee(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetFee::CSetFee(const CSetFee& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetFee::~CSetFee(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetFee::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetFee::initialize(void) {
    CTransaction::initialize();

    _new = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetFee::duplicate(const CSetFee& se) {
    clear();
    CTransaction::duplicate(se);

    _new = se._new;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetFee& CSetFee::operator=(const CSetFee& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetFee::operator==(const CSetFee& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetFee& v1, const CSetFee& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetFee> CSetFeeArray;
extern CArchive& operator>>(CArchive& archive, CSetFeeArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetFeeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

