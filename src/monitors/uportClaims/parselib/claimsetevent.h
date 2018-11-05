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
class CClaimSetEvent : public CLogEntry {
public:
    address_t issuer;
    address_t subject;
    string_q key;
    string_q value;
    biguint_t updatedAt;

public:
    CClaimSetEvent(void);
    CClaimSetEvent(const CClaimSetEvent& cl);
    virtual ~CClaimSetEvent(void);
    CClaimSetEvent& operator=(const CClaimSetEvent& cl);

    DECLARE_NODE(CClaimSetEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CClaimSetEvent& item) const;
    bool operator!=(const CClaimSetEvent& item) const { return !operator==(item); }
    friend bool operator<(const CClaimSetEvent& v1, const CClaimSetEvent& v2);
    friend ostream& operator<<(ostream& os, const CClaimSetEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CClaimSetEvent& cl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CClaimSetEvent::CClaimSetEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CClaimSetEvent::CClaimSetEvent(const CClaimSetEvent& cl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CClaimSetEvent::~CClaimSetEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimSetEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimSetEvent::initialize(void) {
    CLogEntry::initialize();

    issuer = "";
    subject = "";
    key = "";
    value = "";
    updatedAt = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimSetEvent::duplicate(const CClaimSetEvent& cl) {
    clear();
    CLogEntry::duplicate(cl);

    issuer = cl.issuer;
    subject = cl.subject;
    key = cl.key;
    value = cl.value;
    updatedAt = cl.updatedAt;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CClaimSetEvent& CClaimSetEvent::operator=(const CClaimSetEvent& cl) {
    duplicate(cl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CClaimSetEvent::operator==(const CClaimSetEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CClaimSetEvent& v1, const CClaimSetEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CClaimSetEvent> CClaimSetEventArray;
extern CArchive& operator>>(CArchive& archive, CClaimSetEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CClaimSetEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

