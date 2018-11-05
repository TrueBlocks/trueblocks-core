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
class CProposeOwnership : public CTransaction {
public:
    address_t _newOwnerCandidate;

public:
    CProposeOwnership(void);
    CProposeOwnership(const CProposeOwnership& pr);
    virtual ~CProposeOwnership(void);
    CProposeOwnership& operator=(const CProposeOwnership& pr);

    DECLARE_NODE(CProposeOwnership);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CProposeOwnership& item) const;
    bool operator!=(const CProposeOwnership& item) const { return !operator==(item); }
    friend bool operator<(const CProposeOwnership& v1, const CProposeOwnership& v2);
    friend ostream& operator<<(ostream& os, const CProposeOwnership& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CProposeOwnership& pr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CProposeOwnership::CProposeOwnership(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CProposeOwnership::CProposeOwnership(const CProposeOwnership& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CProposeOwnership::~CProposeOwnership(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProposeOwnership::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProposeOwnership::initialize(void) {
    CTransaction::initialize();

    _newOwnerCandidate = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProposeOwnership::duplicate(const CProposeOwnership& pr) {
    clear();
    CTransaction::duplicate(pr);

    _newOwnerCandidate = pr._newOwnerCandidate;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CProposeOwnership& CProposeOwnership::operator=(const CProposeOwnership& pr) {
    duplicate(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CProposeOwnership::operator==(const CProposeOwnership& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CProposeOwnership& v1, const CProposeOwnership& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CProposeOwnership> CProposeOwnershipArray;
extern CArchive& operator>>(CArchive& archive, CProposeOwnershipArray& array);
extern CArchive& operator<<(CArchive& archive, const CProposeOwnershipArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

