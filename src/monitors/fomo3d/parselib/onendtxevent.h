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
class COnEndTxEvent : public CLogEntry {
public:
    biguint_t compressedData;
    biguint_t compressedIDs;
    string_q playerName;
    address_t playerAddress;
    biguint_t ethIn;
    biguint_t keysBought;
    address_t winnerAddr;
    string_q winnerName;
    biguint_t amountWon;
    biguint_t newPot;
    biguint_t P3DAmount;
    biguint_t genAmount;
    biguint_t potAmount;
    biguint_t airDropPot;

public:
    COnEndTxEvent(void);
    COnEndTxEvent(const COnEndTxEvent& on);
    virtual ~COnEndTxEvent(void);
    COnEndTxEvent& operator=(const COnEndTxEvent& on);

    DECLARE_NODE(COnEndTxEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COnEndTxEvent& item) const;
    bool operator!=(const COnEndTxEvent& item) const { return !operator==(item); }
    friend bool operator<(const COnEndTxEvent& v1, const COnEndTxEvent& v2);
    friend ostream& operator<<(ostream& os, const COnEndTxEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COnEndTxEvent& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COnEndTxEvent::COnEndTxEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COnEndTxEvent::COnEndTxEvent(const COnEndTxEvent& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COnEndTxEvent::~COnEndTxEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnEndTxEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnEndTxEvent::initialize(void) {
    CLogEntry::initialize();

    compressedData = 0;
    compressedIDs = 0;
    playerName = "";
    playerAddress = "";
    ethIn = 0;
    keysBought = 0;
    winnerAddr = "";
    winnerName = "";
    amountWon = 0;
    newPot = 0;
    P3DAmount = 0;
    genAmount = 0;
    potAmount = 0;
    airDropPot = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnEndTxEvent::duplicate(const COnEndTxEvent& on) {
    clear();
    CLogEntry::duplicate(on);

    compressedData = on.compressedData;
    compressedIDs = on.compressedIDs;
    playerName = on.playerName;
    playerAddress = on.playerAddress;
    ethIn = on.ethIn;
    keysBought = on.keysBought;
    winnerAddr = on.winnerAddr;
    winnerName = on.winnerName;
    amountWon = on.amountWon;
    newPot = on.newPot;
    P3DAmount = on.P3DAmount;
    genAmount = on.genAmount;
    potAmount = on.potAmount;
    airDropPot = on.airDropPot;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COnEndTxEvent& COnEndTxEvent::operator=(const COnEndTxEvent& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COnEndTxEvent::operator==(const COnEndTxEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COnEndTxEvent& v1, const COnEndTxEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COnEndTxEvent> COnEndTxEventArray;
extern CArchive& operator>>(CArchive& archive, COnEndTxEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COnEndTxEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

