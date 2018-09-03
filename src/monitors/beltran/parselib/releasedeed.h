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
class CReleaseDeed : public CTransaction {
public:
    string_q _hash;

public:
    CReleaseDeed(void);
    CReleaseDeed(const CReleaseDeed& re);
    virtual ~CReleaseDeed(void);
    CReleaseDeed& operator=(const CReleaseDeed& re);

    DECLARE_NODE(CReleaseDeed);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReleaseDeed& item) const;
    bool operator!=(const CReleaseDeed& item) const { return !operator==(item); }
    friend bool operator<(const CReleaseDeed& v1, const CReleaseDeed& v2);
    friend ostream& operator<<(ostream& os, const CReleaseDeed& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReleaseDeed& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReleaseDeed::CReleaseDeed(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReleaseDeed::CReleaseDeed(const CReleaseDeed& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReleaseDeed::~CReleaseDeed(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReleaseDeed::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReleaseDeed::initialize(void) {
    CTransaction::initialize();

    _hash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReleaseDeed::duplicate(const CReleaseDeed& re) {
    clear();
    CTransaction::duplicate(re);

    _hash = re._hash;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CReleaseDeed& CReleaseDeed::operator=(const CReleaseDeed& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReleaseDeed::operator==(const CReleaseDeed& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReleaseDeed& v1, const CReleaseDeed& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReleaseDeed> CReleaseDeedArray;
extern CArchive& operator>>(CArchive& archive, CReleaseDeedArray& array);
extern CArchive& operator<<(CArchive& archive, const CReleaseDeedArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

