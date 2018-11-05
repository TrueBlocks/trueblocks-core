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
class CPause : public CTransaction {
public:
    CPause(void);
    CPause(const CPause& pa);
    virtual ~CPause(void);
    CPause& operator=(const CPause& pa);

    DECLARE_NODE(CPause);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPause& item) const;
    bool operator!=(const CPause& item) const { return !operator==(item); }
    friend bool operator<(const CPause& v1, const CPause& v2);
    friend ostream& operator<<(ostream& os, const CPause& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPause& pa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPause::CPause(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPause::CPause(const CPause& pa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPause::~CPause(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPause::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPause::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPause::duplicate(const CPause& pa) {
    clear();
    CTransaction::duplicate(pa);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPause& CPause::operator=(const CPause& pa) {
    duplicate(pa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPause::operator==(const CPause& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPause& v1, const CPause& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPause> CPauseArray;
extern CArchive& operator>>(CArchive& archive, CPauseArray& array);
extern CArchive& operator<<(CArchive& archive, const CPauseArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

