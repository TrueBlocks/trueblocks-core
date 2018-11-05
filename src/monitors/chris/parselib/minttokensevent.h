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
class CMintTokensEvent : public CLogEntry {
public:
    biguint_t _value;
    biguint_t _balance;

public:
    CMintTokensEvent(void);
    CMintTokensEvent(const CMintTokensEvent& mi);
    virtual ~CMintTokensEvent(void);
    CMintTokensEvent& operator=(const CMintTokensEvent& mi);

    DECLARE_NODE(CMintTokensEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMintTokensEvent& item) const;
    bool operator!=(const CMintTokensEvent& item) const { return !operator==(item); }
    friend bool operator<(const CMintTokensEvent& v1, const CMintTokensEvent& v2);
    friend ostream& operator<<(ostream& os, const CMintTokensEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMintTokensEvent& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMintTokensEvent::CMintTokensEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMintTokensEvent::CMintTokensEvent(const CMintTokensEvent& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMintTokensEvent::~CMintTokensEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTokensEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTokensEvent::initialize(void) {
    CLogEntry::initialize();

    _value = 0;
    _balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTokensEvent::duplicate(const CMintTokensEvent& mi) {
    clear();
    CLogEntry::duplicate(mi);

    _value = mi._value;
    _balance = mi._balance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMintTokensEvent& CMintTokensEvent::operator=(const CMintTokensEvent& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMintTokensEvent::operator==(const CMintTokensEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMintTokensEvent& v1, const CMintTokensEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMintTokensEvent> CMintTokensEventArray;
extern CArchive& operator>>(CArchive& archive, CMintTokensEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CMintTokensEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

