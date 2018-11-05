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
class CWithdrawPaymentEvent : public CLogEntry {
public:
    address_t _account;
    biguint_t _payment;

public:
    CWithdrawPaymentEvent(void);
    CWithdrawPaymentEvent(const CWithdrawPaymentEvent& wi);
    virtual ~CWithdrawPaymentEvent(void);
    CWithdrawPaymentEvent& operator=(const CWithdrawPaymentEvent& wi);

    DECLARE_NODE(CWithdrawPaymentEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawPaymentEvent& item) const;
    bool operator!=(const CWithdrawPaymentEvent& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawPaymentEvent& v1, const CWithdrawPaymentEvent& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawPaymentEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawPaymentEvent& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawPaymentEvent::CWithdrawPaymentEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawPaymentEvent::CWithdrawPaymentEvent(const CWithdrawPaymentEvent& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawPaymentEvent::~CWithdrawPaymentEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawPaymentEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawPaymentEvent::initialize(void) {
    CLogEntry::initialize();

    _account = "";
    _payment = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawPaymentEvent::duplicate(const CWithdrawPaymentEvent& wi) {
    clear();
    CLogEntry::duplicate(wi);

    _account = wi._account;
    _payment = wi._payment;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawPaymentEvent& CWithdrawPaymentEvent::operator=(const CWithdrawPaymentEvent& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawPaymentEvent::operator==(const CWithdrawPaymentEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawPaymentEvent& v1, const CWithdrawPaymentEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawPaymentEvent> CWithdrawPaymentEventArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawPaymentEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawPaymentEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

