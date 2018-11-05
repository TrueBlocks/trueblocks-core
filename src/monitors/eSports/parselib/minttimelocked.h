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
class CMintTimelocked : public CTransaction {
public:
    address_t _to;
    biguint_t _amount;
    uint32_t _releaseTime;

public:
    CMintTimelocked(void);
    CMintTimelocked(const CMintTimelocked& mi);
    virtual ~CMintTimelocked(void);
    CMintTimelocked& operator=(const CMintTimelocked& mi);

    DECLARE_NODE(CMintTimelocked);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMintTimelocked& item) const;
    bool operator!=(const CMintTimelocked& item) const { return !operator==(item); }
    friend bool operator<(const CMintTimelocked& v1, const CMintTimelocked& v2);
    friend ostream& operator<<(ostream& os, const CMintTimelocked& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMintTimelocked& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMintTimelocked::CMintTimelocked(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMintTimelocked::CMintTimelocked(const CMintTimelocked& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMintTimelocked::~CMintTimelocked(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTimelocked::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTimelocked::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _amount = 0;
    _releaseTime = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTimelocked::duplicate(const CMintTimelocked& mi) {
    clear();
    CTransaction::duplicate(mi);

    _to = mi._to;
    _amount = mi._amount;
    _releaseTime = mi._releaseTime;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMintTimelocked& CMintTimelocked::operator=(const CMintTimelocked& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMintTimelocked::operator==(const CMintTimelocked& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMintTimelocked& v1, const CMintTimelocked& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMintTimelocked> CMintTimelockedArray;
extern CArchive& operator>>(CArchive& archive, CMintTimelockedArray& array);
extern CArchive& operator<<(CArchive& archive, const CMintTimelockedArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

