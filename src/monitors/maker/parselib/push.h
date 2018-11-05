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
class CPush : public CTransaction {
public:
    address_t dst;
    biguint_t wad;

public:
    CPush(void);
    CPush(const CPush& pu);
    virtual ~CPush(void);
    CPush& operator=(const CPush& pu);

    DECLARE_NODE(CPush);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPush& item) const;
    bool operator!=(const CPush& item) const { return !operator==(item); }
    friend bool operator<(const CPush& v1, const CPush& v2);
    friend ostream& operator<<(ostream& os, const CPush& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPush& pu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPush::CPush(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPush::CPush(const CPush& pu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPush::~CPush(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPush::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPush::initialize(void) {
    CTransaction::initialize();

    dst = "";
    wad = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPush::duplicate(const CPush& pu) {
    clear();
    CTransaction::duplicate(pu);

    dst = pu.dst;
    wad = pu.wad;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPush& CPush::operator=(const CPush& pu) {
    duplicate(pu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPush::operator==(const CPush& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPush& v1, const CPush& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPush> CPushArray;
extern CArchive& operator>>(CArchive& archive, CPushArray& array);
extern CArchive& operator<<(CArchive& archive, const CPushArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

