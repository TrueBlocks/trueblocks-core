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
class CWithdrawEvent : public CLogEntry {
public:
    address_t token;
    address_t user;
    biguint_t amount;
    biguint_t balance;

public:
    CWithdrawEvent(void);
    CWithdrawEvent(const CWithdrawEvent& wi);
    virtual ~CWithdrawEvent(void);
    CWithdrawEvent& operator=(const CWithdrawEvent& wi);

    DECLARE_NODE(CWithdrawEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawEvent& item) const;
    bool operator!=(const CWithdrawEvent& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawEvent& v1, const CWithdrawEvent& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawEvent& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawEvent::CWithdrawEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawEvent::CWithdrawEvent(const CWithdrawEvent& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawEvent::~CWithdrawEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawEvent::initialize(void) {
    CLogEntry::initialize();

    token = "";
    user = "";
    amount = 0;
    balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawEvent::duplicate(const CWithdrawEvent& wi) {
    clear();
    CLogEntry::duplicate(wi);

    token = wi.token;
    user = wi.user;
    amount = wi.amount;
    balance = wi.balance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawEvent& CWithdrawEvent::operator=(const CWithdrawEvent& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawEvent::operator==(const CWithdrawEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawEvent& v1, const CWithdrawEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawEvent> CWithdrawEventArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

