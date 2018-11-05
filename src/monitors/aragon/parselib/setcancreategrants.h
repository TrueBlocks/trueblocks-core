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
class CSetCanCreateGrants : public CTransaction {
public:
    address_t _addr;
    bool _allowed;

public:
    CSetCanCreateGrants(void);
    CSetCanCreateGrants(const CSetCanCreateGrants& se);
    virtual ~CSetCanCreateGrants(void);
    CSetCanCreateGrants& operator=(const CSetCanCreateGrants& se);

    DECLARE_NODE(CSetCanCreateGrants);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetCanCreateGrants& item) const;
    bool operator!=(const CSetCanCreateGrants& item) const { return !operator==(item); }
    friend bool operator<(const CSetCanCreateGrants& v1, const CSetCanCreateGrants& v2);
    friend ostream& operator<<(ostream& os, const CSetCanCreateGrants& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetCanCreateGrants& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetCanCreateGrants::CSetCanCreateGrants(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetCanCreateGrants::CSetCanCreateGrants(const CSetCanCreateGrants& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetCanCreateGrants::~CSetCanCreateGrants(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCanCreateGrants::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCanCreateGrants::initialize(void) {
    CTransaction::initialize();

    _addr = "";
    _allowed = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCanCreateGrants::duplicate(const CSetCanCreateGrants& se) {
    clear();
    CTransaction::duplicate(se);

    _addr = se._addr;
    _allowed = se._allowed;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetCanCreateGrants& CSetCanCreateGrants::operator=(const CSetCanCreateGrants& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetCanCreateGrants::operator==(const CSetCanCreateGrants& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetCanCreateGrants& v1, const CSetCanCreateGrants& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetCanCreateGrants> CSetCanCreateGrantsArray;
extern CArchive& operator>>(CArchive& archive, CSetCanCreateGrantsArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetCanCreateGrantsArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

