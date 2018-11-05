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
class CCancelMigration : public CTransaction {
public:
    CCancelMigration(void);
    CCancelMigration(const CCancelMigration& ca);
    virtual ~CCancelMigration(void);
    CCancelMigration& operator=(const CCancelMigration& ca);

    DECLARE_NODE(CCancelMigration);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCancelMigration& item) const;
    bool operator!=(const CCancelMigration& item) const { return !operator==(item); }
    friend bool operator<(const CCancelMigration& v1, const CCancelMigration& v2);
    friend ostream& operator<<(ostream& os, const CCancelMigration& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCancelMigration& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCancelMigration::CCancelMigration(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCancelMigration::CCancelMigration(const CCancelMigration& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCancelMigration::~CCancelMigration(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelMigration::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelMigration::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelMigration::duplicate(const CCancelMigration& ca) {
    clear();
    CTransaction::duplicate(ca);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCancelMigration& CCancelMigration::operator=(const CCancelMigration& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCancelMigration::operator==(const CCancelMigration& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCancelMigration& v1, const CCancelMigration& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCancelMigration> CCancelMigrationArray;
extern CArchive& operator>>(CArchive& archive, CCancelMigrationArray& array);
extern CArchive& operator<<(CArchive& archive, const CCancelMigrationArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

