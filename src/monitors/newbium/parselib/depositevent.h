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
class CDepositEvent : public CLogEntry {
public:
    address_t token;
    address_t user;
    biguint_t amount;
    biguint_t balance;

public:
    CDepositEvent(void);
    CDepositEvent(const CDepositEvent& de);
    virtual ~CDepositEvent(void);
    CDepositEvent& operator=(const CDepositEvent& de);

    DECLARE_NODE(CDepositEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDepositEvent& item) const;
    bool operator!=(const CDepositEvent& item) const { return !operator==(item); }
    friend bool operator<(const CDepositEvent& v1, const CDepositEvent& v2);
    friend ostream& operator<<(ostream& os, const CDepositEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDepositEvent& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDepositEvent::CDepositEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDepositEvent::CDepositEvent(const CDepositEvent& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDepositEvent::~CDepositEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositEvent::initialize(void) {
    CLogEntry::initialize();

    token = "";
    user = "";
    amount = 0;
    balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositEvent::duplicate(const CDepositEvent& de) {
    clear();
    CLogEntry::duplicate(de);

    token = de.token;
    user = de.user;
    amount = de.amount;
    balance = de.balance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDepositEvent& CDepositEvent::operator=(const CDepositEvent& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDepositEvent::operator==(const CDepositEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDepositEvent& v1, const CDepositEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDepositEvent> CDepositEventArray;
extern CArchive& operator>>(CArchive& archive, CDepositEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CDepositEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

