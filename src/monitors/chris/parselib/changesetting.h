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
class CChangeSetting : public CTransaction {
public:
    uint32_t _setting;
    biguint_t _value;

public:
    CChangeSetting(void);
    CChangeSetting(const CChangeSetting& ch);
    virtual ~CChangeSetting(void);
    CChangeSetting& operator=(const CChangeSetting& ch);

    DECLARE_NODE(CChangeSetting);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeSetting& item) const;
    bool operator!=(const CChangeSetting& item) const { return !operator==(item); }
    friend bool operator<(const CChangeSetting& v1, const CChangeSetting& v2);
    friend ostream& operator<<(ostream& os, const CChangeSetting& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeSetting& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeSetting::CChangeSetting(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeSetting::CChangeSetting(const CChangeSetting& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeSetting::~CChangeSetting(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeSetting::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeSetting::initialize(void) {
    CTransaction::initialize();

    _setting = 0;
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeSetting::duplicate(const CChangeSetting& ch) {
    clear();
    CTransaction::duplicate(ch);

    _setting = ch._setting;
    _value = ch._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeSetting& CChangeSetting::operator=(const CChangeSetting& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeSetting::operator==(const CChangeSetting& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeSetting& v1, const CChangeSetting& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeSetting> CChangeSettingArray;
extern CArchive& operator>>(CArchive& archive, CChangeSettingArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeSettingArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

