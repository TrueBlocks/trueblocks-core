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
class CVotedEvent : public CLogEntry {
public:
    biguint_t proposalID;
    bool position;
    address_t voter;

public:
    CVotedEvent(void);
    CVotedEvent(const CVotedEvent& vo);
    virtual ~CVotedEvent(void);
    CVotedEvent& operator=(const CVotedEvent& vo);

    DECLARE_NODE(CVotedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CVotedEvent& item) const;
    bool operator!=(const CVotedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CVotedEvent& v1, const CVotedEvent& v2);
    friend ostream& operator<<(ostream& os, const CVotedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CVotedEvent& vo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CVotedEvent::CVotedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CVotedEvent::CVotedEvent(const CVotedEvent& vo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(vo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CVotedEvent::~CVotedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVotedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVotedEvent::initialize(void) {
    CLogEntry::initialize();

    proposalID = 0;
    position = 0;
    voter = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVotedEvent::duplicate(const CVotedEvent& vo) {
    clear();
    CLogEntry::duplicate(vo);

    proposalID = vo.proposalID;
    position = vo.position;
    voter = vo.voter;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CVotedEvent& CVotedEvent::operator=(const CVotedEvent& vo) {
    duplicate(vo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CVotedEvent::operator==(const CVotedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CVotedEvent& v1, const CVotedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CVotedEvent> CVotedEventArray;
extern CArchive& operator>>(CArchive& archive, CVotedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CVotedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

