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
class CUnhalt : public CTransaction {
public:
    CUnhalt(void);
    CUnhalt(const CUnhalt& un);
    virtual ~CUnhalt(void);
    CUnhalt& operator=(const CUnhalt& un);

    DECLARE_NODE(CUnhalt);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnhalt& item) const;
    bool operator!=(const CUnhalt& item) const { return !operator==(item); }
    friend bool operator<(const CUnhalt& v1, const CUnhalt& v2);
    friend ostream& operator<<(ostream& os, const CUnhalt& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnhalt& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnhalt::CUnhalt(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnhalt::CUnhalt(const CUnhalt& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnhalt::~CUnhalt(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnhalt::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnhalt::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnhalt::duplicate(const CUnhalt& un) {
    clear();
    CTransaction::duplicate(un);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnhalt& CUnhalt::operator=(const CUnhalt& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnhalt::operator==(const CUnhalt& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnhalt& v1, const CUnhalt& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnhalt> CUnhaltArray;
extern CArchive& operator>>(CArchive& archive, CUnhaltArray& array);
extern CArchive& operator<<(CArchive& archive, const CUnhaltArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

