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
class CVerifyStatusEvent : public CLogEntry {
public:
    address_t _verifier;
    address_t _account;
    biguint_t _verifyDate;
    string_q _verifyMetaData;

public:
    CVerifyStatusEvent(void);
    CVerifyStatusEvent(const CVerifyStatusEvent& ve);
    virtual ~CVerifyStatusEvent(void);
    CVerifyStatusEvent& operator=(const CVerifyStatusEvent& ve);

    DECLARE_NODE(CVerifyStatusEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CVerifyStatusEvent& item) const;
    bool operator!=(const CVerifyStatusEvent& item) const { return !operator==(item); }
    friend bool operator<(const CVerifyStatusEvent& v1, const CVerifyStatusEvent& v2);
    friend ostream& operator<<(ostream& os, const CVerifyStatusEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CVerifyStatusEvent& ve);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CVerifyStatusEvent::CVerifyStatusEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CVerifyStatusEvent::CVerifyStatusEvent(const CVerifyStatusEvent& ve) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ve);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CVerifyStatusEvent::~CVerifyStatusEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVerifyStatusEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVerifyStatusEvent::initialize(void) {
    CLogEntry::initialize();

    _verifier = "";
    _account = "";
    _verifyDate = 0;
    _verifyMetaData = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CVerifyStatusEvent::duplicate(const CVerifyStatusEvent& ve) {
    clear();
    CLogEntry::duplicate(ve);

    _verifier = ve._verifier;
    _account = ve._account;
    _verifyDate = ve._verifyDate;
    _verifyMetaData = ve._verifyMetaData;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CVerifyStatusEvent& CVerifyStatusEvent::operator=(const CVerifyStatusEvent& ve) {
    duplicate(ve);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CVerifyStatusEvent::operator==(const CVerifyStatusEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CVerifyStatusEvent& v1, const CVerifyStatusEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CVerifyStatusEvent> CVerifyStatusEventArray;
extern CArchive& operator>>(CArchive& archive, CVerifyStatusEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CVerifyStatusEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

