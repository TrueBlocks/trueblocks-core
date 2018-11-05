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
class COnPotSwapDepositEvent : public CLogEntry {
public:
    biguint_t roundID;
    biguint_t amountAddedToPot;

public:
    COnPotSwapDepositEvent(void);
    COnPotSwapDepositEvent(const COnPotSwapDepositEvent& on);
    virtual ~COnPotSwapDepositEvent(void);
    COnPotSwapDepositEvent& operator=(const COnPotSwapDepositEvent& on);

    DECLARE_NODE(COnPotSwapDepositEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COnPotSwapDepositEvent& item) const;
    bool operator!=(const COnPotSwapDepositEvent& item) const { return !operator==(item); }
    friend bool operator<(const COnPotSwapDepositEvent& v1, const COnPotSwapDepositEvent& v2);
    friend ostream& operator<<(ostream& os, const COnPotSwapDepositEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COnPotSwapDepositEvent& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COnPotSwapDepositEvent::COnPotSwapDepositEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COnPotSwapDepositEvent::COnPotSwapDepositEvent(const COnPotSwapDepositEvent& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COnPotSwapDepositEvent::~COnPotSwapDepositEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnPotSwapDepositEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnPotSwapDepositEvent::initialize(void) {
    CLogEntry::initialize();

    roundID = 0;
    amountAddedToPot = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnPotSwapDepositEvent::duplicate(const COnPotSwapDepositEvent& on) {
    clear();
    CLogEntry::duplicate(on);

    roundID = on.roundID;
    amountAddedToPot = on.amountAddedToPot;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COnPotSwapDepositEvent& COnPotSwapDepositEvent::operator=(const COnPotSwapDepositEvent& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COnPotSwapDepositEvent::operator==(const COnPotSwapDepositEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COnPotSwapDepositEvent& v1, const COnPotSwapDepositEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COnPotSwapDepositEvent> COnPotSwapDepositEventArray;
extern CArchive& operator>>(CArchive& archive, COnPotSwapDepositEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COnPotSwapDepositEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

