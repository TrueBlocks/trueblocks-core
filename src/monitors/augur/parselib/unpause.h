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
class CUnpause : public CTransaction {
public:
    CUnpause(void);
    CUnpause(const CUnpause& un);
    virtual ~CUnpause(void);
    CUnpause& operator=(const CUnpause& un);

    DECLARE_NODE(CUnpause);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnpause& item) const;
    bool operator!=(const CUnpause& item) const { return !operator==(item); }
    friend bool operator<(const CUnpause& v1, const CUnpause& v2);
    friend ostream& operator<<(ostream& os, const CUnpause& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnpause& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnpause::CUnpause(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnpause::CUnpause(const CUnpause& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnpause::~CUnpause(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnpause::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnpause::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnpause::duplicate(const CUnpause& un) {
    clear();
    CTransaction::duplicate(un);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnpause& CUnpause::operator=(const CUnpause& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnpause::operator==(const CUnpause& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnpause& v1, const CUnpause& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnpause> CUnpauseArray;
extern CArchive& operator>>(CArchive& archive, CUnpauseArray& array);
extern CArchive& operator<<(CArchive& archive, const CUnpauseArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

