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
class CTokenMintEvent : public CLogEntry {
public:
    address_t newTokenHolder;
    biguint_t amountOfTokens;

public:
    CTokenMintEvent(void);
    CTokenMintEvent(const CTokenMintEvent& to);
    virtual ~CTokenMintEvent(void);
    CTokenMintEvent& operator=(const CTokenMintEvent& to);

    DECLARE_NODE(CTokenMintEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTokenMintEvent& item) const;
    bool operator!=(const CTokenMintEvent& item) const { return !operator==(item); }
    friend bool operator<(const CTokenMintEvent& v1, const CTokenMintEvent& v2);
    friend ostream& operator<<(ostream& os, const CTokenMintEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTokenMintEvent& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTokenMintEvent::CTokenMintEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTokenMintEvent::CTokenMintEvent(const CTokenMintEvent& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTokenMintEvent::~CTokenMintEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenMintEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenMintEvent::initialize(void) {
    CLogEntry::initialize();

    newTokenHolder = "";
    amountOfTokens = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenMintEvent::duplicate(const CTokenMintEvent& to) {
    clear();
    CLogEntry::duplicate(to);

    newTokenHolder = to.newTokenHolder;
    amountOfTokens = to.amountOfTokens;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTokenMintEvent& CTokenMintEvent::operator=(const CTokenMintEvent& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTokenMintEvent::operator==(const CTokenMintEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTokenMintEvent& v1, const CTokenMintEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTokenMintEvent> CTokenMintEventArray;
extern CArchive& operator>>(CArchive& archive, CTokenMintEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CTokenMintEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

