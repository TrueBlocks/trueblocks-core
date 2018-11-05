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
class CChangeVestingWhitelister : public CTransaction {
public:
    address_t _newWhitelister;

public:
    CChangeVestingWhitelister(void);
    CChangeVestingWhitelister(const CChangeVestingWhitelister& ch);
    virtual ~CChangeVestingWhitelister(void);
    CChangeVestingWhitelister& operator=(const CChangeVestingWhitelister& ch);

    DECLARE_NODE(CChangeVestingWhitelister);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeVestingWhitelister& item) const;
    bool operator!=(const CChangeVestingWhitelister& item) const { return !operator==(item); }
    friend bool operator<(const CChangeVestingWhitelister& v1, const CChangeVestingWhitelister& v2);
    friend ostream& operator<<(ostream& os, const CChangeVestingWhitelister& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeVestingWhitelister& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeVestingWhitelister::CChangeVestingWhitelister(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeVestingWhitelister::CChangeVestingWhitelister(const CChangeVestingWhitelister& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeVestingWhitelister::~CChangeVestingWhitelister(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeVestingWhitelister::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeVestingWhitelister::initialize(void) {
    CTransaction::initialize();

    _newWhitelister = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeVestingWhitelister::duplicate(const CChangeVestingWhitelister& ch) {
    clear();
    CTransaction::duplicate(ch);

    _newWhitelister = ch._newWhitelister;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeVestingWhitelister& CChangeVestingWhitelister::operator=(const CChangeVestingWhitelister& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeVestingWhitelister::operator==(const CChangeVestingWhitelister& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeVestingWhitelister& v1, const CChangeVestingWhitelister& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeVestingWhitelister> CChangeVestingWhitelisterArray;
extern CArchive& operator>>(CArchive& archive, CChangeVestingWhitelisterArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeVestingWhitelisterArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

