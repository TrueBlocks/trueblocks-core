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
class CClaimRemovedEvent : public CLogEntry {
public:
    address_t issuer;
    address_t subject;
    string_q key;
    biguint_t removedAt;

public:
    CClaimRemovedEvent(void);
    CClaimRemovedEvent(const CClaimRemovedEvent& cl);
    virtual ~CClaimRemovedEvent(void);
    CClaimRemovedEvent& operator=(const CClaimRemovedEvent& cl);

    DECLARE_NODE(CClaimRemovedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CClaimRemovedEvent& item) const;
    bool operator!=(const CClaimRemovedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CClaimRemovedEvent& v1, const CClaimRemovedEvent& v2);
    friend ostream& operator<<(ostream& os, const CClaimRemovedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CClaimRemovedEvent& cl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CClaimRemovedEvent::CClaimRemovedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CClaimRemovedEvent::CClaimRemovedEvent(const CClaimRemovedEvent& cl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CClaimRemovedEvent::~CClaimRemovedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimRemovedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimRemovedEvent::initialize(void) {
    CLogEntry::initialize();

    issuer = "";
    subject = "";
    key = "";
    removedAt = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimRemovedEvent::duplicate(const CClaimRemovedEvent& cl) {
    clear();
    CLogEntry::duplicate(cl);

    issuer = cl.issuer;
    subject = cl.subject;
    key = cl.key;
    removedAt = cl.removedAt;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CClaimRemovedEvent& CClaimRemovedEvent::operator=(const CClaimRemovedEvent& cl) {
    duplicate(cl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CClaimRemovedEvent::operator==(const CClaimRemovedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CClaimRemovedEvent& v1, const CClaimRemovedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CClaimRemovedEvent> CClaimRemovedEventArray;
extern CArchive& operator>>(CArchive& archive, CClaimRemovedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CClaimRemovedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

