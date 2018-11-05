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
class COnNewNameEvent : public CLogEntry {
public:
    biguint_t playerID;
    address_t playerAddress;
    string_q playerName;
    bool isNewPlayer;
    biguint_t affiliateID;
    address_t affiliateAddress;
    string_q affiliateName;
    biguint_t amountPaid;
    biguint_t timeStamp;

public:
    COnNewNameEvent(void);
    COnNewNameEvent(const COnNewNameEvent& on);
    virtual ~COnNewNameEvent(void);
    COnNewNameEvent& operator=(const COnNewNameEvent& on);

    DECLARE_NODE(COnNewNameEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COnNewNameEvent& item) const;
    bool operator!=(const COnNewNameEvent& item) const { return !operator==(item); }
    friend bool operator<(const COnNewNameEvent& v1, const COnNewNameEvent& v2);
    friend ostream& operator<<(ostream& os, const COnNewNameEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COnNewNameEvent& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COnNewNameEvent::COnNewNameEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COnNewNameEvent::COnNewNameEvent(const COnNewNameEvent& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COnNewNameEvent::~COnNewNameEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnNewNameEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnNewNameEvent::initialize(void) {
    CLogEntry::initialize();

    playerID = 0;
    playerAddress = "";
    playerName = "";
    isNewPlayer = 0;
    affiliateID = 0;
    affiliateAddress = "";
    affiliateName = "";
    amountPaid = 0;
    timeStamp = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnNewNameEvent::duplicate(const COnNewNameEvent& on) {
    clear();
    CLogEntry::duplicate(on);

    playerID = on.playerID;
    playerAddress = on.playerAddress;
    playerName = on.playerName;
    isNewPlayer = on.isNewPlayer;
    affiliateID = on.affiliateID;
    affiliateAddress = on.affiliateAddress;
    affiliateName = on.affiliateName;
    amountPaid = on.amountPaid;
    timeStamp = on.timeStamp;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COnNewNameEvent& COnNewNameEvent::operator=(const COnNewNameEvent& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COnNewNameEvent::operator==(const COnNewNameEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COnNewNameEvent& v1, const COnNewNameEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COnNewNameEvent> COnNewNameEventArray;
extern CArchive& operator>>(CArchive& archive, COnNewNameEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COnNewNameEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

