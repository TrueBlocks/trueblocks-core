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
class CStartTokenSwap : public CTransaction {
public:
    CStartTokenSwap(void);
    CStartTokenSwap(const CStartTokenSwap& st);
    virtual ~CStartTokenSwap(void);
    CStartTokenSwap& operator=(const CStartTokenSwap& st);

    DECLARE_NODE(CStartTokenSwap);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStartTokenSwap& item) const;
    bool operator!=(const CStartTokenSwap& item) const { return !operator==(item); }
    friend bool operator<(const CStartTokenSwap& v1, const CStartTokenSwap& v2);
    friend ostream& operator<<(ostream& os, const CStartTokenSwap& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStartTokenSwap& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStartTokenSwap::CStartTokenSwap(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStartTokenSwap::CStartTokenSwap(const CStartTokenSwap& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStartTokenSwap::~CStartTokenSwap(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartTokenSwap::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartTokenSwap::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartTokenSwap::duplicate(const CStartTokenSwap& st) {
    clear();
    CTransaction::duplicate(st);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStartTokenSwap& CStartTokenSwap::operator=(const CStartTokenSwap& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStartTokenSwap::operator==(const CStartTokenSwap& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStartTokenSwap& v1, const CStartTokenSwap& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStartTokenSwap> CStartTokenSwapArray;
extern CArchive& operator>>(CArchive& archive, CStartTokenSwapArray& array);
extern CArchive& operator<<(CArchive& archive, const CStartTokenSwapArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

