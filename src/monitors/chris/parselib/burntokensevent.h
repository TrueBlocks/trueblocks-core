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
class CBurnTokensEvent : public CLogEntry {
public:
    biguint_t _value;
    biguint_t _balance;

public:
    CBurnTokensEvent(void);
    CBurnTokensEvent(const CBurnTokensEvent& bu);
    virtual ~CBurnTokensEvent(void);
    CBurnTokensEvent& operator=(const CBurnTokensEvent& bu);

    DECLARE_NODE(CBurnTokensEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBurnTokensEvent& item) const;
    bool operator!=(const CBurnTokensEvent& item) const { return !operator==(item); }
    friend bool operator<(const CBurnTokensEvent& v1, const CBurnTokensEvent& v2);
    friend ostream& operator<<(ostream& os, const CBurnTokensEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBurnTokensEvent& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBurnTokensEvent::CBurnTokensEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBurnTokensEvent::CBurnTokensEvent(const CBurnTokensEvent& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBurnTokensEvent::~CBurnTokensEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnTokensEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnTokensEvent::initialize(void) {
    CLogEntry::initialize();

    _value = 0;
    _balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnTokensEvent::duplicate(const CBurnTokensEvent& bu) {
    clear();
    CLogEntry::duplicate(bu);

    _value = bu._value;
    _balance = bu._balance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBurnTokensEvent& CBurnTokensEvent::operator=(const CBurnTokensEvent& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBurnTokensEvent::operator==(const CBurnTokensEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBurnTokensEvent& v1, const CBurnTokensEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBurnTokensEvent> CBurnTokensEventArray;
extern CArchive& operator>>(CArchive& archive, CBurnTokensEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CBurnTokensEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

