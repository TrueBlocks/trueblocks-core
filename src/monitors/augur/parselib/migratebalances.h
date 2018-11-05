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
class CMigrateBalances : public CTransaction {
public:
    CAddressArray _holders;

public:
    CMigrateBalances(void);
    CMigrateBalances(const CMigrateBalances& mi);
    virtual ~CMigrateBalances(void);
    CMigrateBalances& operator=(const CMigrateBalances& mi);

    DECLARE_NODE(CMigrateBalances);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMigrateBalances& item) const;
    bool operator!=(const CMigrateBalances& item) const { return !operator==(item); }
    friend bool operator<(const CMigrateBalances& v1, const CMigrateBalances& v2);
    friend ostream& operator<<(ostream& os, const CMigrateBalances& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMigrateBalances& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMigrateBalances::CMigrateBalances(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMigrateBalances::CMigrateBalances(const CMigrateBalances& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMigrateBalances::~CMigrateBalances(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigrateBalances::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigrateBalances::initialize(void) {
    CTransaction::initialize();

    _holders.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigrateBalances::duplicate(const CMigrateBalances& mi) {
    clear();
    CTransaction::duplicate(mi);

    _holders = mi._holders;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMigrateBalances& CMigrateBalances::operator=(const CMigrateBalances& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMigrateBalances::operator==(const CMigrateBalances& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMigrateBalances& v1, const CMigrateBalances& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMigrateBalances> CMigrateBalancesArray;
extern CArchive& operator>>(CArchive& archive, CMigrateBalancesArray& array);
extern CArchive& operator<<(CArchive& archive, const CMigrateBalancesArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

