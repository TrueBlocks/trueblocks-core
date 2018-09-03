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
class CClaimedTokensEvent : public CLogEntry {
public:
    address_t _recipient;
    biguint_t _sent_amount;

public:
    CClaimedTokensEvent(void);
    CClaimedTokensEvent(const CClaimedTokensEvent& cl);
    virtual ~CClaimedTokensEvent(void);
    CClaimedTokensEvent& operator=(const CClaimedTokensEvent& cl);

    DECLARE_NODE(CClaimedTokensEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CClaimedTokensEvent& item) const;
    bool operator!=(const CClaimedTokensEvent& item) const { return !operator==(item); }
    friend bool operator<(const CClaimedTokensEvent& v1, const CClaimedTokensEvent& v2);
    friend ostream& operator<<(ostream& os, const CClaimedTokensEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CClaimedTokensEvent& cl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CClaimedTokensEvent::CClaimedTokensEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CClaimedTokensEvent::CClaimedTokensEvent(const CClaimedTokensEvent& cl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CClaimedTokensEvent::~CClaimedTokensEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimedTokensEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimedTokensEvent::initialize(void) {
    CLogEntry::initialize();

    _recipient = "";
    _sent_amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimedTokensEvent::duplicate(const CClaimedTokensEvent& cl) {
    clear();
    CLogEntry::duplicate(cl);

    _recipient = cl._recipient;
    _sent_amount = cl._sent_amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CClaimedTokensEvent& CClaimedTokensEvent::operator=(const CClaimedTokensEvent& cl) {
    duplicate(cl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CClaimedTokensEvent::operator==(const CClaimedTokensEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CClaimedTokensEvent& v1, const CClaimedTokensEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CClaimedTokensEvent> CClaimedTokensEventArray;
extern CArchive& operator>>(CArchive& archive, CClaimedTokensEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CClaimedTokensEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

