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
class CAllocateFounderTokensEvent : public CLogEntry {
public:
    address_t sender;

public:
    CAllocateFounderTokensEvent(void);
    CAllocateFounderTokensEvent(const CAllocateFounderTokensEvent& al);
    virtual ~CAllocateFounderTokensEvent(void);
    CAllocateFounderTokensEvent& operator=(const CAllocateFounderTokensEvent& al);

    DECLARE_NODE(CAllocateFounderTokensEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAllocateFounderTokensEvent& item) const;
    bool operator!=(const CAllocateFounderTokensEvent& item) const { return !operator==(item); }
    friend bool operator<(const CAllocateFounderTokensEvent& v1, const CAllocateFounderTokensEvent& v2);
    friend ostream& operator<<(ostream& os, const CAllocateFounderTokensEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAllocateFounderTokensEvent& al);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAllocateFounderTokensEvent::CAllocateFounderTokensEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAllocateFounderTokensEvent::CAllocateFounderTokensEvent(const CAllocateFounderTokensEvent& al) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(al);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAllocateFounderTokensEvent::~CAllocateFounderTokensEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateFounderTokensEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateFounderTokensEvent::initialize(void) {
    CLogEntry::initialize();

    sender = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateFounderTokensEvent::duplicate(const CAllocateFounderTokensEvent& al) {
    clear();
    CLogEntry::duplicate(al);

    sender = al.sender;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAllocateFounderTokensEvent& CAllocateFounderTokensEvent::operator=(const CAllocateFounderTokensEvent& al) {
    duplicate(al);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAllocateFounderTokensEvent::operator==(const CAllocateFounderTokensEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAllocateFounderTokensEvent& v1, const CAllocateFounderTokensEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAllocateFounderTokensEvent> CAllocateFounderTokensEventArray;
extern CArchive& operator>>(CArchive& archive, CAllocateFounderTokensEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CAllocateFounderTokensEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

