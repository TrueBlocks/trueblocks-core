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
class CToggleTrustedVerifierEvent : public CLogEntry {
public:
    address_t _verifier;
    bool _isTrustedVerifier;

public:
    CToggleTrustedVerifierEvent(void);
    CToggleTrustedVerifierEvent(const CToggleTrustedVerifierEvent& to);
    virtual ~CToggleTrustedVerifierEvent(void);
    CToggleTrustedVerifierEvent& operator=(const CToggleTrustedVerifierEvent& to);

    DECLARE_NODE(CToggleTrustedVerifierEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CToggleTrustedVerifierEvent& item) const;
    bool operator!=(const CToggleTrustedVerifierEvent& item) const { return !operator==(item); }
    friend bool operator<(const CToggleTrustedVerifierEvent& v1, const CToggleTrustedVerifierEvent& v2);
    friend ostream& operator<<(ostream& os, const CToggleTrustedVerifierEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CToggleTrustedVerifierEvent& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CToggleTrustedVerifierEvent::CToggleTrustedVerifierEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CToggleTrustedVerifierEvent::CToggleTrustedVerifierEvent(const CToggleTrustedVerifierEvent& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CToggleTrustedVerifierEvent::~CToggleTrustedVerifierEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CToggleTrustedVerifierEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CToggleTrustedVerifierEvent::initialize(void) {
    CLogEntry::initialize();

    _verifier = "";
    _isTrustedVerifier = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CToggleTrustedVerifierEvent::duplicate(const CToggleTrustedVerifierEvent& to) {
    clear();
    CLogEntry::duplicate(to);

    _verifier = to._verifier;
    _isTrustedVerifier = to._isTrustedVerifier;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CToggleTrustedVerifierEvent& CToggleTrustedVerifierEvent::operator=(const CToggleTrustedVerifierEvent& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CToggleTrustedVerifierEvent::operator==(const CToggleTrustedVerifierEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CToggleTrustedVerifierEvent& v1, const CToggleTrustedVerifierEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CToggleTrustedVerifierEvent> CToggleTrustedVerifierEventArray;
extern CArchive& operator>>(CArchive& archive, CToggleTrustedVerifierEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CToggleTrustedVerifierEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

