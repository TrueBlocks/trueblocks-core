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
class CAllocateBountyAndEcosystemTokensEvent : public CLogEntry {
public:
    address_t sender;

public:
    CAllocateBountyAndEcosystemTokensEvent(void);
    CAllocateBountyAndEcosystemTokensEvent(const CAllocateBountyAndEcosystemTokensEvent& al);
    virtual ~CAllocateBountyAndEcosystemTokensEvent(void);
    CAllocateBountyAndEcosystemTokensEvent& operator=(const CAllocateBountyAndEcosystemTokensEvent& al);

    DECLARE_NODE(CAllocateBountyAndEcosystemTokensEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAllocateBountyAndEcosystemTokensEvent& item) const;
    bool operator!=(const CAllocateBountyAndEcosystemTokensEvent& item) const { return !operator==(item); }
    friend bool operator<(const CAllocateBountyAndEcosystemTokensEvent& v1, const CAllocateBountyAndEcosystemTokensEvent& v2);
    friend ostream& operator<<(ostream& os, const CAllocateBountyAndEcosystemTokensEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAllocateBountyAndEcosystemTokensEvent& al);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAllocateBountyAndEcosystemTokensEvent::CAllocateBountyAndEcosystemTokensEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAllocateBountyAndEcosystemTokensEvent::CAllocateBountyAndEcosystemTokensEvent(const CAllocateBountyAndEcosystemTokensEvent& al) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(al);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAllocateBountyAndEcosystemTokensEvent::~CAllocateBountyAndEcosystemTokensEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateBountyAndEcosystemTokensEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateBountyAndEcosystemTokensEvent::initialize(void) {
    CLogEntry::initialize();

    sender = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateBountyAndEcosystemTokensEvent::duplicate(const CAllocateBountyAndEcosystemTokensEvent& al) {
    clear();
    CLogEntry::duplicate(al);

    sender = al.sender;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAllocateBountyAndEcosystemTokensEvent& CAllocateBountyAndEcosystemTokensEvent::operator=(const CAllocateBountyAndEcosystemTokensEvent& al) {
    duplicate(al);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAllocateBountyAndEcosystemTokensEvent::operator==(const CAllocateBountyAndEcosystemTokensEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAllocateBountyAndEcosystemTokensEvent& v1, const CAllocateBountyAndEcosystemTokensEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAllocateBountyAndEcosystemTokensEvent> CAllocateBountyAndEcosystemTokensEventArray;
extern CArchive& operator>>(CArchive& archive, CAllocateBountyAndEcosystemTokensEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CAllocateBountyAndEcosystemTokensEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

