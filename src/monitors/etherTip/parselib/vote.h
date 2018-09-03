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
class CVote : public CTransaction {
public:
    biguint_t proposalNumber;
    bool supportsProposal;
    string_q justificationText;

public:
    CVote(void);
    CVote(const CVote& vo);
    virtual ~CVote(void);
    CVote& operator=(const CVote& vo);

    DECLARE_NODE(CVote);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CVote& item) const;
    bool operator!=(const CVote& item) const { return !operator==(item); }
    friend bool operator<(const CVote& v1, const CVote& v2);
    friend ostream& operator<<(ostream& os, const CVote& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CVote& vo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CVote::CVote(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CVote::CVote(const CVote& vo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(vo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CVote::~CVote(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVote::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVote::initialize(void) {
    CTransaction::initialize();

    proposalNumber = 0;
    supportsProposal = 0;
    justificationText = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVote::duplicate(const CVote& vo) {
    clear();
    CTransaction::duplicate(vo);

    proposalNumber = vo.proposalNumber;
    supportsProposal = vo.supportsProposal;
    justificationText = vo.justificationText;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CVote& CVote::operator=(const CVote& vo) {
    duplicate(vo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CVote::operator==(const CVote& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CVote& v1, const CVote& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CVote> CVoteArray;
extern CArchive& operator>>(CArchive& archive, CVoteArray& array);
extern CArchive& operator<<(CArchive& archive, const CVoteArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

