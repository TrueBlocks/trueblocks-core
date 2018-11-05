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
class CAllowedRecipientChangedEvent : public CLogEntry {
public:
    address_t _recipient;
    bool _allowed;

public:
    CAllowedRecipientChangedEvent(void);
    CAllowedRecipientChangedEvent(const CAllowedRecipientChangedEvent& al);
    virtual ~CAllowedRecipientChangedEvent(void);
    CAllowedRecipientChangedEvent& operator=(const CAllowedRecipientChangedEvent& al);

    DECLARE_NODE(CAllowedRecipientChangedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAllowedRecipientChangedEvent& item) const;
    bool operator!=(const CAllowedRecipientChangedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CAllowedRecipientChangedEvent& v1, const CAllowedRecipientChangedEvent& v2);
    friend ostream& operator<<(ostream& os, const CAllowedRecipientChangedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAllowedRecipientChangedEvent& al);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAllowedRecipientChangedEvent::CAllowedRecipientChangedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAllowedRecipientChangedEvent::CAllowedRecipientChangedEvent(const CAllowedRecipientChangedEvent& al) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(al);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAllowedRecipientChangedEvent::~CAllowedRecipientChangedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllowedRecipientChangedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllowedRecipientChangedEvent::initialize(void) {
    CLogEntry::initialize();

    _recipient = "";
    _allowed = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllowedRecipientChangedEvent::duplicate(const CAllowedRecipientChangedEvent& al) {
    clear();
    CLogEntry::duplicate(al);

    _recipient = al._recipient;
    _allowed = al._allowed;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAllowedRecipientChangedEvent& CAllowedRecipientChangedEvent::operator=(const CAllowedRecipientChangedEvent& al) {
    duplicate(al);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAllowedRecipientChangedEvent::operator==(const CAllowedRecipientChangedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAllowedRecipientChangedEvent& v1, const CAllowedRecipientChangedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAllowedRecipientChangedEvent> CAllowedRecipientChangedEventArray;
extern CArchive& operator>>(CArchive& archive, CAllowedRecipientChangedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CAllowedRecipientChangedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

