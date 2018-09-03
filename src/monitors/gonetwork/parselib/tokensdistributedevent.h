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
class CTokensDistributedEvent : public CLogEntry {
public:
    CTokensDistributedEvent(void);
    CTokensDistributedEvent(const CTokensDistributedEvent& to);
    virtual ~CTokensDistributedEvent(void);
    CTokensDistributedEvent& operator=(const CTokensDistributedEvent& to);

    DECLARE_NODE(CTokensDistributedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTokensDistributedEvent& item) const;
    bool operator!=(const CTokensDistributedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CTokensDistributedEvent& v1, const CTokensDistributedEvent& v2);
    friend ostream& operator<<(ostream& os, const CTokensDistributedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTokensDistributedEvent& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTokensDistributedEvent::CTokensDistributedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTokensDistributedEvent::CTokensDistributedEvent(const CTokensDistributedEvent& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTokensDistributedEvent::~CTokensDistributedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokensDistributedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokensDistributedEvent::initialize(void) {
    CLogEntry::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokensDistributedEvent::duplicate(const CTokensDistributedEvent& to) {
    clear();
    CLogEntry::duplicate(to);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTokensDistributedEvent& CTokensDistributedEvent::operator=(const CTokensDistributedEvent& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTokensDistributedEvent::operator==(const CTokensDistributedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTokensDistributedEvent& v1, const CTokensDistributedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTokensDistributedEvent> CTokensDistributedEventArray;
extern CArchive& operator>>(CArchive& archive, CTokensDistributedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CTokensDistributedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

