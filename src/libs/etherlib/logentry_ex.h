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
#include "logentry.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLogEntry_Ex : public CLogEntry {
public:
    string_q name;

public:
    CLogEntry_Ex(void);
    CLogEntry_Ex(const CLogEntry_Ex& lo);
    virtual ~CLogEntry_Ex(void);
    CLogEntry_Ex& operator=(const CLogEntry_Ex& lo);

    DECLARE_NODE(CLogEntry_Ex);

    // EXISTING_CODE
    CLogEntry_Ex(const CLogEntry *p);
    // EXISTING_CODE
    bool operator==(const CLogEntry_Ex& item) const;
    bool operator!=(const CLogEntry_Ex& item) const { return !operator==(item); }
    friend bool operator<(const CLogEntry_Ex& v1, const CLogEntry_Ex& v2);
    friend ostream& operator<<(ostream& os, const CLogEntry_Ex& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLogEntry_Ex& lo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogEntry_Ex::CLogEntry_Ex(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogEntry_Ex::CLogEntry_Ex(const CLogEntry_Ex& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogEntry_Ex::~CLogEntry_Ex(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry_Ex::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry_Ex::initialize(void) {
    CLogEntry::initialize();

    name = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry_Ex::duplicate(const CLogEntry_Ex& lo) {
    clear();
    CLogEntry::duplicate(lo);

    name = lo.name;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CLogEntry_Ex& CLogEntry_Ex::operator=(const CLogEntry_Ex& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLogEntry_Ex::operator==(const CLogEntry_Ex& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLogEntry_Ex& v1, const CLogEntry_Ex& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLogEntry_Ex> CLogEntry_ExArray;
extern CArchive& operator>>(CArchive& archive, CLogEntry_ExArray& array);
extern CArchive& operator<<(CArchive& archive, const CLogEntry_ExArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

