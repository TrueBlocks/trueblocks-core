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
class CSetName : public CTransaction {
public:
    string_q name_;

public:
    CSetName(void);
    CSetName(const CSetName& se);
    virtual ~CSetName(void);
    CSetName& operator=(const CSetName& se);

    DECLARE_NODE(CSetName);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetName& item) const;
    bool operator!=(const CSetName& item) const { return !operator==(item); }
    friend bool operator<(const CSetName& v1, const CSetName& v2);
    friend ostream& operator<<(ostream& os, const CSetName& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetName& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetName::CSetName(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetName::CSetName(const CSetName& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetName::~CSetName(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetName::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetName::initialize(void) {
    CTransaction::initialize();

    name_ = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetName::duplicate(const CSetName& se) {
    clear();
    CTransaction::duplicate(se);

    name_ = se.name_;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetName& CSetName::operator=(const CSetName& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetName::operator==(const CSetName& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetName& v1, const CSetName& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetName> CSetNameArray;
extern CArchive& operator>>(CArchive& archive, CSetNameArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetNameArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

