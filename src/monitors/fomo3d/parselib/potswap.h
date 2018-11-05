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
class CPotSwap : public CTransaction {
public:
    CPotSwap(void);
    CPotSwap(const CPotSwap& po);
    virtual ~CPotSwap(void);
    CPotSwap& operator=(const CPotSwap& po);

    DECLARE_NODE(CPotSwap);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPotSwap& item) const;
    bool operator!=(const CPotSwap& item) const { return !operator==(item); }
    friend bool operator<(const CPotSwap& v1, const CPotSwap& v2);
    friend ostream& operator<<(ostream& os, const CPotSwap& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPotSwap& po);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPotSwap::CPotSwap(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPotSwap::CPotSwap(const CPotSwap& po) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(po);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPotSwap::~CPotSwap(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPotSwap::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPotSwap::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPotSwap::duplicate(const CPotSwap& po) {
    clear();
    CTransaction::duplicate(po);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPotSwap& CPotSwap::operator=(const CPotSwap& po) {
    duplicate(po);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPotSwap::operator==(const CPotSwap& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPotSwap& v1, const CPotSwap& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPotSwap> CPotSwapArray;
extern CArchive& operator>>(CArchive& archive, CPotSwapArray& array);
extern CArchive& operator<<(CArchive& archive, const CPotSwapArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

