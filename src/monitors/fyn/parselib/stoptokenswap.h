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
class CStopTokenSwap : public CTransaction {
public:
    CStopTokenSwap(void);
    CStopTokenSwap(const CStopTokenSwap& st);
    virtual ~CStopTokenSwap(void);
    CStopTokenSwap& operator=(const CStopTokenSwap& st);

    DECLARE_NODE(CStopTokenSwap);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStopTokenSwap& item) const;
    bool operator!=(const CStopTokenSwap& item) const { return !operator==(item); }
    friend bool operator<(const CStopTokenSwap& v1, const CStopTokenSwap& v2);
    friend ostream& operator<<(ostream& os, const CStopTokenSwap& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStopTokenSwap& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStopTokenSwap::CStopTokenSwap(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStopTokenSwap::CStopTokenSwap(const CStopTokenSwap& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStopTokenSwap::~CStopTokenSwap(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStopTokenSwap::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStopTokenSwap::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStopTokenSwap::duplicate(const CStopTokenSwap& st) {
    clear();
    CTransaction::duplicate(st);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStopTokenSwap& CStopTokenSwap::operator=(const CStopTokenSwap& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStopTokenSwap::operator==(const CStopTokenSwap& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStopTokenSwap& v1, const CStopTokenSwap& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStopTokenSwap> CStopTokenSwapArray;
extern CArchive& operator>>(CArchive& archive, CStopTokenSwapArray& array);
extern CArchive& operator<<(CArchive& archive, const CStopTokenSwapArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

