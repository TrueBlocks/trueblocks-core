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
class CMigrateBalance : public CTransaction {
public:
    address_t _holder;

public:
    CMigrateBalance(void);
    CMigrateBalance(const CMigrateBalance& mi);
    virtual ~CMigrateBalance(void);
    CMigrateBalance& operator=(const CMigrateBalance& mi);

    DECLARE_NODE(CMigrateBalance);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMigrateBalance& item) const;
    bool operator!=(const CMigrateBalance& item) const { return !operator==(item); }
    friend bool operator<(const CMigrateBalance& v1, const CMigrateBalance& v2);
    friend ostream& operator<<(ostream& os, const CMigrateBalance& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMigrateBalance& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMigrateBalance::CMigrateBalance(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMigrateBalance::CMigrateBalance(const CMigrateBalance& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMigrateBalance::~CMigrateBalance(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigrateBalance::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigrateBalance::initialize(void) {
    CTransaction::initialize();

    _holder = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigrateBalance::duplicate(const CMigrateBalance& mi) {
    clear();
    CTransaction::duplicate(mi);

    _holder = mi._holder;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMigrateBalance& CMigrateBalance::operator=(const CMigrateBalance& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMigrateBalance::operator==(const CMigrateBalance& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMigrateBalance& v1, const CMigrateBalance& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMigrateBalance> CMigrateBalanceArray;
extern CArchive& operator>>(CArchive& archive, CMigrateBalanceArray& array);
extern CArchive& operator<<(CArchive& archive, const CMigrateBalanceArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

