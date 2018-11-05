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
class CTogglePause : public CTransaction {
public:
    CTogglePause(void);
    CTogglePause(const CTogglePause& to);
    virtual ~CTogglePause(void);
    CTogglePause& operator=(const CTogglePause& to);

    DECLARE_NODE(CTogglePause);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTogglePause& item) const;
    bool operator!=(const CTogglePause& item) const { return !operator==(item); }
    friend bool operator<(const CTogglePause& v1, const CTogglePause& v2);
    friend ostream& operator<<(ostream& os, const CTogglePause& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTogglePause& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTogglePause::CTogglePause(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTogglePause::CTogglePause(const CTogglePause& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTogglePause::~CTogglePause(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTogglePause::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTogglePause::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTogglePause::duplicate(const CTogglePause& to) {
    clear();
    CTransaction::duplicate(to);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTogglePause& CTogglePause::operator=(const CTogglePause& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTogglePause::operator==(const CTogglePause& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTogglePause& v1, const CTogglePause& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTogglePause> CTogglePauseArray;
extern CArchive& operator>>(CArchive& archive, CTogglePauseArray& array);
extern CArchive& operator<<(CArchive& archive, const CTogglePauseArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

