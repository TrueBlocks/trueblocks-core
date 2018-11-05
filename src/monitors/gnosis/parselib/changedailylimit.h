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
class CChangeDailyLimit : public CTransaction {
public:
    biguint_t _dailyLimit;

public:
    CChangeDailyLimit(void);
    CChangeDailyLimit(const CChangeDailyLimit& ch);
    virtual ~CChangeDailyLimit(void);
    CChangeDailyLimit& operator=(const CChangeDailyLimit& ch);

    DECLARE_NODE(CChangeDailyLimit);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeDailyLimit& item) const;
    bool operator!=(const CChangeDailyLimit& item) const { return !operator==(item); }
    friend bool operator<(const CChangeDailyLimit& v1, const CChangeDailyLimit& v2);
    friend ostream& operator<<(ostream& os, const CChangeDailyLimit& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeDailyLimit& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeDailyLimit::CChangeDailyLimit(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeDailyLimit::CChangeDailyLimit(const CChangeDailyLimit& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeDailyLimit::~CChangeDailyLimit(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeDailyLimit::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeDailyLimit::initialize(void) {
    CTransaction::initialize();

    _dailyLimit = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeDailyLimit::duplicate(const CChangeDailyLimit& ch) {
    clear();
    CTransaction::duplicate(ch);

    _dailyLimit = ch._dailyLimit;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeDailyLimit& CChangeDailyLimit::operator=(const CChangeDailyLimit& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeDailyLimit::operator==(const CChangeDailyLimit& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeDailyLimit& v1, const CChangeDailyLimit& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeDailyLimit> CChangeDailyLimitArray;
extern CArchive& operator>>(CArchive& archive, CChangeDailyLimitArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeDailyLimitArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

