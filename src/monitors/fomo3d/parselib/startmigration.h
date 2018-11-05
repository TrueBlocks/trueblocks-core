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
class CStartMigration : public CTransaction {
public:
    address_t _newCorpBank;

public:
    CStartMigration(void);
    CStartMigration(const CStartMigration& st);
    virtual ~CStartMigration(void);
    CStartMigration& operator=(const CStartMigration& st);

    DECLARE_NODE(CStartMigration);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStartMigration& item) const;
    bool operator!=(const CStartMigration& item) const { return !operator==(item); }
    friend bool operator<(const CStartMigration& v1, const CStartMigration& v2);
    friend ostream& operator<<(ostream& os, const CStartMigration& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStartMigration& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStartMigration::CStartMigration(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStartMigration::CStartMigration(const CStartMigration& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStartMigration::~CStartMigration(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartMigration::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartMigration::initialize(void) {
    CTransaction::initialize();

    _newCorpBank = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartMigration::duplicate(const CStartMigration& st) {
    clear();
    CTransaction::duplicate(st);

    _newCorpBank = st._newCorpBank;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStartMigration& CStartMigration::operator=(const CStartMigration& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStartMigration::operator==(const CStartMigration& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStartMigration& v1, const CStartMigration& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStartMigration> CStartMigrationArray;
extern CArchive& operator>>(CArchive& archive, CStartMigrationArray& array);
extern CArchive& operator<<(CArchive& archive, const CStartMigrationArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

