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
class CMint9f8f : public CTransaction {
public:
    biguint_t wad;

public:
    CMint9f8f(void);
    CMint9f8f(const CMint9f8f& mi);
    virtual ~CMint9f8f(void);
    CMint9f8f& operator=(const CMint9f8f& mi);

    DECLARE_NODE(CMint9f8f);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMint9f8f& item) const;
    bool operator!=(const CMint9f8f& item) const { return !operator==(item); }
    friend bool operator<(const CMint9f8f& v1, const CMint9f8f& v2);
    friend ostream& operator<<(ostream& os, const CMint9f8f& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMint9f8f& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMint9f8f::CMint9f8f(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMint9f8f::CMint9f8f(const CMint9f8f& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMint9f8f::~CMint9f8f(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMint9f8f::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMint9f8f::initialize(void) {
    CTransaction::initialize();

    wad = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMint9f8f::duplicate(const CMint9f8f& mi) {
    clear();
    CTransaction::duplicate(mi);

    wad = mi.wad;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMint9f8f& CMint9f8f::operator=(const CMint9f8f& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMint9f8f::operator==(const CMint9f8f& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMint9f8f& v1, const CMint9f8f& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMint9f8f> CMint9f8fArray;
extern CArchive& operator>>(CArchive& archive, CMint9f8fArray& array);
extern CArchive& operator<<(CArchive& archive, const CMint9f8fArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

