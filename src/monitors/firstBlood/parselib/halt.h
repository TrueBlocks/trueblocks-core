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
class CHalt : public CTransaction {
public:
    CHalt(void);
    CHalt(const CHalt& ha);
    virtual ~CHalt(void);
    CHalt& operator=(const CHalt& ha);

    DECLARE_NODE(CHalt);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CHalt& item) const;
    bool operator!=(const CHalt& item) const { return !operator==(item); }
    friend bool operator<(const CHalt& v1, const CHalt& v2);
    friend ostream& operator<<(ostream& os, const CHalt& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CHalt& ha);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CHalt::CHalt(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CHalt::CHalt(const CHalt& ha) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ha);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CHalt::~CHalt(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHalt::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHalt::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHalt::duplicate(const CHalt& ha) {
    clear();
    CTransaction::duplicate(ha);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CHalt& CHalt::operator=(const CHalt& ha) {
    duplicate(ha);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CHalt::operator==(const CHalt& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CHalt& v1, const CHalt& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CHalt> CHaltArray;
extern CArchive& operator>>(CArchive& archive, CHaltArray& array);
extern CArchive& operator<<(CArchive& archive, const CHaltArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

