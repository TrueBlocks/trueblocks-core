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
class CMessageEvent : public CLogEntry {
public:
    string_q fName;
    string_q message;

public:
    CMessageEvent(void);
    CMessageEvent(const CMessageEvent& me);
    virtual ~CMessageEvent(void);
    CMessageEvent& operator=(const CMessageEvent& me);

    DECLARE_NODE(CMessageEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMessageEvent& item) const;
    bool operator!=(const CMessageEvent& item) const { return !operator==(item); }
    friend bool operator<(const CMessageEvent& v1, const CMessageEvent& v2);
    friend ostream& operator<<(ostream& os, const CMessageEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMessageEvent& me);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMessageEvent::CMessageEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMessageEvent::CMessageEvent(const CMessageEvent& me) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(me);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMessageEvent::~CMessageEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMessageEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMessageEvent::initialize(void) {
    CLogEntry::initialize();

    fName = "";
    message = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMessageEvent::duplicate(const CMessageEvent& me) {
    clear();
    CLogEntry::duplicate(me);

    fName = me.fName;
    message = me.message;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMessageEvent& CMessageEvent::operator=(const CMessageEvent& me) {
    duplicate(me);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMessageEvent::operator==(const CMessageEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMessageEvent& v1, const CMessageEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMessageEvent> CMessageEventArray;
extern CArchive& operator>>(CArchive& archive, CMessageEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CMessageEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

