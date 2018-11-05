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
class CDisableTokenSwapLock : public CTransaction {
public:
    CDisableTokenSwapLock(void);
    CDisableTokenSwapLock(const CDisableTokenSwapLock& di);
    virtual ~CDisableTokenSwapLock(void);
    CDisableTokenSwapLock& operator=(const CDisableTokenSwapLock& di);

    DECLARE_NODE(CDisableTokenSwapLock);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDisableTokenSwapLock& item) const;
    bool operator!=(const CDisableTokenSwapLock& item) const { return !operator==(item); }
    friend bool operator<(const CDisableTokenSwapLock& v1, const CDisableTokenSwapLock& v2);
    friend ostream& operator<<(ostream& os, const CDisableTokenSwapLock& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDisableTokenSwapLock& di);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDisableTokenSwapLock::CDisableTokenSwapLock(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDisableTokenSwapLock::CDisableTokenSwapLock(const CDisableTokenSwapLock& di) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(di);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDisableTokenSwapLock::~CDisableTokenSwapLock(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTokenSwapLock::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTokenSwapLock::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTokenSwapLock::duplicate(const CDisableTokenSwapLock& di) {
    clear();
    CTransaction::duplicate(di);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDisableTokenSwapLock& CDisableTokenSwapLock::operator=(const CDisableTokenSwapLock& di) {
    duplicate(di);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDisableTokenSwapLock::operator==(const CDisableTokenSwapLock& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDisableTokenSwapLock& v1, const CDisableTokenSwapLock& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDisableTokenSwapLock> CDisableTokenSwapLockArray;
extern CArchive& operator>>(CArchive& archive, CDisableTokenSwapLockArray& array);
extern CArchive& operator<<(CArchive& archive, const CDisableTokenSwapLockArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

