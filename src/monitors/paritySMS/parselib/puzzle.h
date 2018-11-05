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
class CPuzzle : public CTransaction {
public:
    address_t _who;
    string_q _puzzle;

public:
    CPuzzle(void);
    CPuzzle(const CPuzzle& pu);
    virtual ~CPuzzle(void);
    CPuzzle& operator=(const CPuzzle& pu);

    DECLARE_NODE(CPuzzle);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPuzzle& item) const;
    bool operator!=(const CPuzzle& item) const { return !operator==(item); }
    friend bool operator<(const CPuzzle& v1, const CPuzzle& v2);
    friend ostream& operator<<(ostream& os, const CPuzzle& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPuzzle& pu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPuzzle::CPuzzle(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPuzzle::CPuzzle(const CPuzzle& pu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPuzzle::~CPuzzle(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPuzzle::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPuzzle::initialize(void) {
    CTransaction::initialize();

    _who = "";
    _puzzle = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPuzzle::duplicate(const CPuzzle& pu) {
    clear();
    CTransaction::duplicate(pu);

    _who = pu._who;
    _puzzle = pu._puzzle;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPuzzle& CPuzzle::operator=(const CPuzzle& pu) {
    duplicate(pu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPuzzle::operator==(const CPuzzle& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPuzzle& v1, const CPuzzle& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPuzzle> CPuzzleArray;
extern CArchive& operator>>(CArchive& archive, CPuzzleArray& array);
extern CArchive& operator<<(CArchive& archive, const CPuzzleArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

