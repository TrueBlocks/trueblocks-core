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
class CContributeBTCs : public CTransaction {
public:
    CContributeBTCs(void);
    CContributeBTCs(const CContributeBTCs& co);
    virtual ~CContributeBTCs(void);
    CContributeBTCs& operator=(const CContributeBTCs& co);

    DECLARE_NODE(CContributeBTCs);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CContributeBTCs& item) const;
    bool operator!=(const CContributeBTCs& item) const { return !operator==(item); }
    friend bool operator<(const CContributeBTCs& v1, const CContributeBTCs& v2);
    friend ostream& operator<<(ostream& os, const CContributeBTCs& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CContributeBTCs& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CContributeBTCs::CContributeBTCs(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CContributeBTCs::CContributeBTCs(const CContributeBTCs& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CContributeBTCs::~CContributeBTCs(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributeBTCs::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributeBTCs::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributeBTCs::duplicate(const CContributeBTCs& co) {
    clear();
    CTransaction::duplicate(co);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CContributeBTCs& CContributeBTCs::operator=(const CContributeBTCs& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CContributeBTCs::operator==(const CContributeBTCs& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CContributeBTCs& v1, const CContributeBTCs& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CContributeBTCs> CContributeBTCsArray;
extern CArchive& operator>>(CArchive& archive, CContributeBTCsArray& array);
extern CArchive& operator<<(CArchive& archive, const CContributeBTCsArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

