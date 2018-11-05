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
class COnWithdrawEvent : public CLogEntry {
public:
    biguint_t playerID;
    address_t playerAddress;
    string_q playerName;
    biguint_t ethOut;
    biguint_t timeStamp;

public:
    COnWithdrawEvent(void);
    COnWithdrawEvent(const COnWithdrawEvent& on);
    virtual ~COnWithdrawEvent(void);
    COnWithdrawEvent& operator=(const COnWithdrawEvent& on);

    DECLARE_NODE(COnWithdrawEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COnWithdrawEvent& item) const;
    bool operator!=(const COnWithdrawEvent& item) const { return !operator==(item); }
    friend bool operator<(const COnWithdrawEvent& v1, const COnWithdrawEvent& v2);
    friend ostream& operator<<(ostream& os, const COnWithdrawEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COnWithdrawEvent& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COnWithdrawEvent::COnWithdrawEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COnWithdrawEvent::COnWithdrawEvent(const COnWithdrawEvent& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COnWithdrawEvent::~COnWithdrawEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnWithdrawEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnWithdrawEvent::initialize(void) {
    CLogEntry::initialize();

    playerID = 0;
    playerAddress = "";
    playerName = "";
    ethOut = 0;
    timeStamp = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnWithdrawEvent::duplicate(const COnWithdrawEvent& on) {
    clear();
    CLogEntry::duplicate(on);

    playerID = on.playerID;
    playerAddress = on.playerAddress;
    playerName = on.playerName;
    ethOut = on.ethOut;
    timeStamp = on.timeStamp;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COnWithdrawEvent& COnWithdrawEvent::operator=(const COnWithdrawEvent& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COnWithdrawEvent::operator==(const COnWithdrawEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COnWithdrawEvent& v1, const COnWithdrawEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COnWithdrawEvent> COnWithdrawEventArray;
extern CArchive& operator>>(CArchive& archive, COnWithdrawEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COnWithdrawEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

