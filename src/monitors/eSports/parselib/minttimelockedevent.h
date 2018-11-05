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
#include "logentry.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMintTimelockedEvent : public CLogEntry {
public:
    address_t beneficiary;
    biguint_t amount;

public:
    CMintTimelockedEvent(void);
    CMintTimelockedEvent(const CMintTimelockedEvent& mi);
    virtual ~CMintTimelockedEvent(void);
    CMintTimelockedEvent& operator=(const CMintTimelockedEvent& mi);

    DECLARE_NODE(CMintTimelockedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMintTimelockedEvent& item) const;
    bool operator!=(const CMintTimelockedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CMintTimelockedEvent& v1, const CMintTimelockedEvent& v2);
    friend ostream& operator<<(ostream& os, const CMintTimelockedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMintTimelockedEvent& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMintTimelockedEvent::CMintTimelockedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMintTimelockedEvent::CMintTimelockedEvent(const CMintTimelockedEvent& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMintTimelockedEvent::~CMintTimelockedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTimelockedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTimelockedEvent::initialize(void) {
    CLogEntry::initialize();

    beneficiary = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTimelockedEvent::duplicate(const CMintTimelockedEvent& mi) {
    clear();
    CLogEntry::duplicate(mi);

    beneficiary = mi.beneficiary;
    amount = mi.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMintTimelockedEvent& CMintTimelockedEvent::operator=(const CMintTimelockedEvent& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMintTimelockedEvent::operator==(const CMintTimelockedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMintTimelockedEvent& v1, const CMintTimelockedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMintTimelockedEvent> CMintTimelockedEventArray;
extern CArchive& operator>>(CArchive& archive, CMintTimelockedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CMintTimelockedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

