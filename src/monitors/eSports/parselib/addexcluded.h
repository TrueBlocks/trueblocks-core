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
class CAddExcluded : public CTransaction {
public:
    address_t _toExclude;

public:
    CAddExcluded(void);
    CAddExcluded(const CAddExcluded& ad);
    virtual ~CAddExcluded(void);
    CAddExcluded& operator=(const CAddExcluded& ad);

    DECLARE_NODE(CAddExcluded);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAddExcluded& item) const;
    bool operator!=(const CAddExcluded& item) const { return !operator==(item); }
    friend bool operator<(const CAddExcluded& v1, const CAddExcluded& v2);
    friend ostream& operator<<(ostream& os, const CAddExcluded& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAddExcluded& ad);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAddExcluded::CAddExcluded(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAddExcluded::CAddExcluded(const CAddExcluded& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAddExcluded::~CAddExcluded(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddExcluded::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddExcluded::initialize(void) {
    CTransaction::initialize();

    _toExclude = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddExcluded::duplicate(const CAddExcluded& ad) {
    clear();
    CTransaction::duplicate(ad);

    _toExclude = ad._toExclude;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAddExcluded& CAddExcluded::operator=(const CAddExcluded& ad) {
    duplicate(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAddExcluded::operator==(const CAddExcluded& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAddExcluded& v1, const CAddExcluded& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAddExcluded> CAddExcludedArray;
extern CArchive& operator>>(CArchive& archive, CAddExcludedArray& array);
extern CArchive& operator<<(CArchive& archive, const CAddExcludedArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

