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
class CUnblockMe : public CTransaction {
public:
    CUnblockMe(void);
    CUnblockMe(const CUnblockMe& un);
    virtual ~CUnblockMe(void);
    CUnblockMe& operator=(const CUnblockMe& un);

    DECLARE_NODE(CUnblockMe);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnblockMe& item) const;
    bool operator!=(const CUnblockMe& item) const { return !operator==(item); }
    friend bool operator<(const CUnblockMe& v1, const CUnblockMe& v2);
    friend ostream& operator<<(ostream& os, const CUnblockMe& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnblockMe& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnblockMe::CUnblockMe(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnblockMe::CUnblockMe(const CUnblockMe& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnblockMe::~CUnblockMe(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnblockMe::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnblockMe::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnblockMe::duplicate(const CUnblockMe& un) {
    clear();
    CTransaction::duplicate(un);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnblockMe& CUnblockMe::operator=(const CUnblockMe& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnblockMe::operator==(const CUnblockMe& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnblockMe& v1, const CUnblockMe& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnblockMe> CUnblockMeArray;
extern CArchive& operator>>(CArchive& archive, CUnblockMeArray& array);
extern CArchive& operator<<(CArchive& archive, const CUnblockMeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

