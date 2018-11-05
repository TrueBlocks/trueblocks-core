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
class CFinishMigration : public CTransaction {
public:
    CFinishMigration(void);
    CFinishMigration(const CFinishMigration& fi);
    virtual ~CFinishMigration(void);
    CFinishMigration& operator=(const CFinishMigration& fi);

    DECLARE_NODE(CFinishMigration);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFinishMigration& item) const;
    bool operator!=(const CFinishMigration& item) const { return !operator==(item); }
    friend bool operator<(const CFinishMigration& v1, const CFinishMigration& v2);
    friend ostream& operator<<(ostream& os, const CFinishMigration& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFinishMigration& fi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFinishMigration::CFinishMigration(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFinishMigration::CFinishMigration(const CFinishMigration& fi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFinishMigration::~CFinishMigration(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinishMigration::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinishMigration::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinishMigration::duplicate(const CFinishMigration& fi) {
    clear();
    CTransaction::duplicate(fi);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFinishMigration& CFinishMigration::operator=(const CFinishMigration& fi) {
    duplicate(fi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFinishMigration::operator==(const CFinishMigration& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFinishMigration& v1, const CFinishMigration& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFinishMigration> CFinishMigrationArray;
extern CArchive& operator>>(CArchive& archive, CFinishMigrationArray& array);
extern CArchive& operator<<(CArchive& archive, const CFinishMigrationArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

