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
class CRevoke : public CTransaction {
public:
    address_t _who;

public:
    CRevoke(void);
    CRevoke(const CRevoke& re);
    virtual ~CRevoke(void);
    CRevoke& operator=(const CRevoke& re);

    DECLARE_NODE(CRevoke);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRevoke& item) const;
    bool operator!=(const CRevoke& item) const { return !operator==(item); }
    friend bool operator<(const CRevoke& v1, const CRevoke& v2);
    friend ostream& operator<<(ostream& os, const CRevoke& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRevoke& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRevoke::CRevoke(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRevoke::CRevoke(const CRevoke& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRevoke::~CRevoke(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevoke::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevoke::initialize(void) {
    CTransaction::initialize();

    _who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevoke::duplicate(const CRevoke& re) {
    clear();
    CTransaction::duplicate(re);

    _who = re._who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRevoke& CRevoke::operator=(const CRevoke& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRevoke::operator==(const CRevoke& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRevoke& v1, const CRevoke& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRevoke> CRevokeArray;
extern CArchive& operator>>(CArchive& archive, CRevokeArray& array);
extern CArchive& operator<<(CArchive& archive, const CRevokeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

