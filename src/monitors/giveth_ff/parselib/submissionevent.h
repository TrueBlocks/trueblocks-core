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
class CSubmissionEvent : public CLogEntry {
public:
    biguint_t transactionId;

public:
    CSubmissionEvent(void);
    CSubmissionEvent(const CSubmissionEvent& su);
    virtual ~CSubmissionEvent(void);
    CSubmissionEvent& operator=(const CSubmissionEvent& su);

    DECLARE_NODE(CSubmissionEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSubmissionEvent& item) const;
    bool operator!=(const CSubmissionEvent& item) const { return !operator==(item); }
    friend bool operator<(const CSubmissionEvent& v1, const CSubmissionEvent& v2);
    friend ostream& operator<<(ostream& os, const CSubmissionEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSubmissionEvent& su);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSubmissionEvent::CSubmissionEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSubmissionEvent::CSubmissionEvent(const CSubmissionEvent& su) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(su);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSubmissionEvent::~CSubmissionEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSubmissionEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSubmissionEvent::initialize(void) {
    CLogEntry::initialize();

    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSubmissionEvent::duplicate(const CSubmissionEvent& su) {
    clear();
    CLogEntry::duplicate(su);

    transactionId = su.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSubmissionEvent& CSubmissionEvent::operator=(const CSubmissionEvent& su) {
    duplicate(su);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSubmissionEvent::operator==(const CSubmissionEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSubmissionEvent& v1, const CSubmissionEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSubmissionEvent> CSubmissionEventArray;
extern CArchive& operator>>(CArchive& archive, CSubmissionEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CSubmissionEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

