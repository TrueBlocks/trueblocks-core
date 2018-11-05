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
class CChangeBaseValue : public CTransaction {
public:
    biguint_t valueInWei;

public:
    CChangeBaseValue(void);
    CChangeBaseValue(const CChangeBaseValue& ch);
    virtual ~CChangeBaseValue(void);
    CChangeBaseValue& operator=(const CChangeBaseValue& ch);

    DECLARE_NODE(CChangeBaseValue);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeBaseValue& item) const;
    bool operator!=(const CChangeBaseValue& item) const { return !operator==(item); }
    friend bool operator<(const CChangeBaseValue& v1, const CChangeBaseValue& v2);
    friend ostream& operator<<(ostream& os, const CChangeBaseValue& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeBaseValue& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeBaseValue::CChangeBaseValue(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeBaseValue::CChangeBaseValue(const CChangeBaseValue& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeBaseValue::~CChangeBaseValue(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeBaseValue::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeBaseValue::initialize(void) {
    CTransaction::initialize();

    valueInWei = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeBaseValue::duplicate(const CChangeBaseValue& ch) {
    clear();
    CTransaction::duplicate(ch);

    valueInWei = ch.valueInWei;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeBaseValue& CChangeBaseValue::operator=(const CChangeBaseValue& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeBaseValue::operator==(const CChangeBaseValue& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeBaseValue& v1, const CChangeBaseValue& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeBaseValue> CChangeBaseValueArray;
extern CArchive& operator>>(CArchive& archive, CChangeBaseValueArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeBaseValueArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

