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
class CTradeEvent : public CLogEntry {
public:
    address_t tokenGet;
    biguint_t amountGet;
    address_t tokenGive;
    biguint_t amountGive;
    address_t get;
    address_t give;

public:
    CTradeEvent(void);
    CTradeEvent(const CTradeEvent& tr);
    virtual ~CTradeEvent(void);
    CTradeEvent& operator=(const CTradeEvent& tr);

    DECLARE_NODE(CTradeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTradeEvent& item) const;
    bool operator!=(const CTradeEvent& item) const { return !operator==(item); }
    friend bool operator<(const CTradeEvent& v1, const CTradeEvent& v2);
    friend ostream& operator<<(ostream& os, const CTradeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTradeEvent& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTradeEvent::CTradeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTradeEvent::CTradeEvent(const CTradeEvent& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTradeEvent::~CTradeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTradeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTradeEvent::initialize(void) {
    CLogEntry::initialize();

    tokenGet = "";
    amountGet = 0;
    tokenGive = "";
    amountGive = 0;
    get = "";
    give = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTradeEvent::duplicate(const CTradeEvent& tr) {
    clear();
    CLogEntry::duplicate(tr);

    tokenGet = tr.tokenGet;
    amountGet = tr.amountGet;
    tokenGive = tr.tokenGive;
    amountGive = tr.amountGive;
    get = tr.get;
    give = tr.give;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTradeEvent& CTradeEvent::operator=(const CTradeEvent& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTradeEvent::operator==(const CTradeEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTradeEvent& v1, const CTradeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTradeEvent> CTradeEventArray;
extern CArchive& operator>>(CArchive& archive, CTradeEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CTradeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

