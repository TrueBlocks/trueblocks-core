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
class CChangeEvent : public CLogEntry {
public:
    address_t _fromToken;
    address_t _toToken;
    address_t _trader;
    biguint_t _amount;
    biguint_t _return;

public:
    CChangeEvent(void);
    CChangeEvent(const CChangeEvent& ch);
    virtual ~CChangeEvent(void);
    CChangeEvent& operator=(const CChangeEvent& ch);

    DECLARE_NODE(CChangeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeEvent& item) const;
    bool operator!=(const CChangeEvent& item) const { return !operator==(item); }
    friend bool operator<(const CChangeEvent& v1, const CChangeEvent& v2);
    friend ostream& operator<<(ostream& os, const CChangeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeEvent& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeEvent::CChangeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeEvent::CChangeEvent(const CChangeEvent& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeEvent::~CChangeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeEvent::initialize(void) {
    CLogEntry::initialize();

    _fromToken = "";
    _toToken = "";
    _trader = "";
    _amount = 0;
    _return = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeEvent::duplicate(const CChangeEvent& ch) {
    clear();
    CLogEntry::duplicate(ch);

    _fromToken = ch._fromToken;
    _toToken = ch._toToken;
    _trader = ch._trader;
    _amount = ch._amount;
    _return = ch._return;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeEvent& CChangeEvent::operator=(const CChangeEvent& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeEvent::operator==(const CChangeEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeEvent& v1, const CChangeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeEvent> CChangeEventArray;
extern CArchive& operator>>(CArchive& archive, CChangeEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

