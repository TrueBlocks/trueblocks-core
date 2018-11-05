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
class CBid : public CTransaction {
public:
    address_t receiver;

public:
    CBid(void);
    CBid(const CBid& bi);
    virtual ~CBid(void);
    CBid& operator=(const CBid& bi);

    DECLARE_NODE(CBid);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBid& item) const;
    bool operator!=(const CBid& item) const { return !operator==(item); }
    friend bool operator<(const CBid& v1, const CBid& v2);
    friend ostream& operator<<(ostream& os, const CBid& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBid& bi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBid::CBid(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBid::CBid(const CBid& bi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBid::~CBid(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBid::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBid::initialize(void) {
    CTransaction::initialize();

    receiver = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBid::duplicate(const CBid& bi) {
    clear();
    CTransaction::duplicate(bi);

    receiver = bi.receiver;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBid& CBid::operator=(const CBid& bi) {
    duplicate(bi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBid::operator==(const CBid& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBid& v1, const CBid& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBid> CBidArray;
extern CArchive& operator>>(CArchive& archive, CBidArray& array);
extern CArchive& operator<<(CArchive& archive, const CBidArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

