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
class CPull : public CTransaction {
public:
    address_t src;
    biguint_t wad;

public:
    CPull(void);
    CPull(const CPull& pu);
    virtual ~CPull(void);
    CPull& operator=(const CPull& pu);

    DECLARE_NODE(CPull);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPull& item) const;
    bool operator!=(const CPull& item) const { return !operator==(item); }
    friend bool operator<(const CPull& v1, const CPull& v2);
    friend ostream& operator<<(ostream& os, const CPull& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPull& pu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPull::CPull(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPull::CPull(const CPull& pu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPull::~CPull(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPull::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPull::initialize(void) {
    CTransaction::initialize();

    src = "";
    wad = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPull::duplicate(const CPull& pu) {
    clear();
    CTransaction::duplicate(pu);

    src = pu.src;
    wad = pu.wad;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPull& CPull::operator=(const CPull& pu) {
    duplicate(pu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPull::operator==(const CPull& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPull& v1, const CPull& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPull> CPullArray;
extern CArchive& operator>>(CArchive& archive, CPullArray& array);
extern CArchive& operator<<(CArchive& archive, const CPullArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

