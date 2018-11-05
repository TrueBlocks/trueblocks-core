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
class CChangeFounder : public CTransaction {
public:
    address_t newFounder;

public:
    CChangeFounder(void);
    CChangeFounder(const CChangeFounder& ch);
    virtual ~CChangeFounder(void);
    CChangeFounder& operator=(const CChangeFounder& ch);

    DECLARE_NODE(CChangeFounder);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeFounder& item) const;
    bool operator!=(const CChangeFounder& item) const { return !operator==(item); }
    friend bool operator<(const CChangeFounder& v1, const CChangeFounder& v2);
    friend ostream& operator<<(ostream& os, const CChangeFounder& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeFounder& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeFounder::CChangeFounder(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeFounder::CChangeFounder(const CChangeFounder& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeFounder::~CChangeFounder(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFounder::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFounder::initialize(void) {
    CTransaction::initialize();

    newFounder = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFounder::duplicate(const CChangeFounder& ch) {
    clear();
    CTransaction::duplicate(ch);

    newFounder = ch.newFounder;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeFounder& CChangeFounder::operator=(const CChangeFounder& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeFounder::operator==(const CChangeFounder& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeFounder& v1, const CChangeFounder& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeFounder> CChangeFounderArray;
extern CArchive& operator>>(CArchive& archive, CChangeFounderArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeFounderArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

