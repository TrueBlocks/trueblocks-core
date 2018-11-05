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
class CFund : public CTransaction {
public:
    CFund(void);
    CFund(const CFund& fu);
    virtual ~CFund(void);
    CFund& operator=(const CFund& fu);

    DECLARE_NODE(CFund);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFund& item) const;
    bool operator!=(const CFund& item) const { return !operator==(item); }
    friend bool operator<(const CFund& v1, const CFund& v2);
    friend ostream& operator<<(ostream& os, const CFund& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFund& fu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFund::CFund(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFund::CFund(const CFund& fu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFund::~CFund(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFund::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFund::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFund::duplicate(const CFund& fu) {
    clear();
    CTransaction::duplicate(fu);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFund& CFund::operator=(const CFund& fu) {
    duplicate(fu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFund::operator==(const CFund& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFund& v1, const CFund& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFund> CFundArray;
extern CArchive& operator>>(CArchive& archive, CFundArray& array);
extern CArchive& operator<<(CArchive& archive, const CFundArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

