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
class CBuy77a7 : public CTransaction {
public:
    CBuy77a7(void);
    CBuy77a7(const CBuy77a7& bu);
    virtual ~CBuy77a7(void);
    CBuy77a7& operator=(const CBuy77a7& bu);

    DECLARE_NODE(CBuy77a7);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuy77a7& item) const;
    bool operator!=(const CBuy77a7& item) const { return !operator==(item); }
    friend bool operator<(const CBuy77a7& v1, const CBuy77a7& v2);
    friend ostream& operator<<(ostream& os, const CBuy77a7& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuy77a7& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuy77a7::CBuy77a7(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuy77a7::CBuy77a7(const CBuy77a7& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuy77a7::~CBuy77a7(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuy77a7::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuy77a7::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuy77a7::duplicate(const CBuy77a7& bu) {
    clear();
    CTransaction::duplicate(bu);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuy77a7& CBuy77a7::operator=(const CBuy77a7& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuy77a7::operator==(const CBuy77a7& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuy77a7& v1, const CBuy77a7& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuy77a7> CBuy77a7Array;
extern CArchive& operator>>(CArchive& archive, CBuy77a7Array& array);
extern CArchive& operator<<(CArchive& archive, const CBuy77a7Array& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

