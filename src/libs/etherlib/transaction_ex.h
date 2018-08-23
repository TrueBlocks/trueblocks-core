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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTransaction_Ex : public CTransaction {
public:
    string_q name;
    uint64_t traceId;

public:
    CTransaction_Ex(void);
    CTransaction_Ex(const CTransaction_Ex& tr);
    virtual ~CTransaction_Ex(void);
    CTransaction_Ex& operator=(const CTransaction_Ex& tr);

    DECLARE_NODE(CTransaction_Ex);

    // EXISTING_CODE
    CTransaction_Ex(const CTransaction *p);
    // EXISTING_CODE
    bool operator==(const CTransaction_Ex& item) const;
    bool operator!=(const CTransaction_Ex& item) const { return !operator==(item); }
    friend bool operator<(const CTransaction_Ex& v1, const CTransaction_Ex& v2);
    friend ostream& operator<<(ostream& os, const CTransaction_Ex& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransaction_Ex& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransaction_Ex::CTransaction_Ex(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransaction_Ex::CTransaction_Ex(const CTransaction_Ex& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransaction_Ex::~CTransaction_Ex(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction_Ex::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction_Ex::initialize(void) {
    CTransaction::initialize();

    name = "";
    traceId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction_Ex::duplicate(const CTransaction_Ex& tr) {
    clear();
    CTransaction::duplicate(tr);

    name = tr.name;
    traceId = tr.traceId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransaction_Ex& CTransaction_Ex::operator=(const CTransaction_Ex& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransaction_Ex::operator==(const CTransaction_Ex& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransaction_Ex& v1, const CTransaction_Ex& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransaction_Ex> CTransaction_ExArray;
extern CArchive& operator>>(CArchive& archive, CTransaction_ExArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransaction_ExArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

