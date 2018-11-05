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
class CSeal : public CTransaction {
public:
    CSeal(void);
    CSeal(const CSeal& se);
    virtual ~CSeal(void);
    CSeal& operator=(const CSeal& se);

    DECLARE_NODE(CSeal);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSeal& item) const;
    bool operator!=(const CSeal& item) const { return !operator==(item); }
    friend bool operator<(const CSeal& v1, const CSeal& v2);
    friend ostream& operator<<(ostream& os, const CSeal& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSeal& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSeal::CSeal(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSeal::CSeal(const CSeal& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSeal::~CSeal(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSeal::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSeal::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSeal::duplicate(const CSeal& se) {
    clear();
    CTransaction::duplicate(se);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSeal& CSeal::operator=(const CSeal& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSeal::operator==(const CSeal& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSeal& v1, const CSeal& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSeal> CSealArray;
extern CArchive& operator>>(CArchive& archive, CSealArray& array);
extern CArchive& operator<<(CArchive& archive, const CSealArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

