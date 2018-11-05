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
class CExecutionFailureEvent : public CLogEntry {
public:
    biguint_t transactionId;

public:
    CExecutionFailureEvent(void);
    CExecutionFailureEvent(const CExecutionFailureEvent& ex);
    virtual ~CExecutionFailureEvent(void);
    CExecutionFailureEvent& operator=(const CExecutionFailureEvent& ex);

    DECLARE_NODE(CExecutionFailureEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CExecutionFailureEvent& item) const;
    bool operator!=(const CExecutionFailureEvent& item) const { return !operator==(item); }
    friend bool operator<(const CExecutionFailureEvent& v1, const CExecutionFailureEvent& v2);
    friend ostream& operator<<(ostream& os, const CExecutionFailureEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CExecutionFailureEvent& ex);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CExecutionFailureEvent::CExecutionFailureEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CExecutionFailureEvent::CExecutionFailureEvent(const CExecutionFailureEvent& ex) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ex);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CExecutionFailureEvent::~CExecutionFailureEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecutionFailureEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecutionFailureEvent::initialize(void) {
    CLogEntry::initialize();

    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecutionFailureEvent::duplicate(const CExecutionFailureEvent& ex) {
    clear();
    CLogEntry::duplicate(ex);

    transactionId = ex.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CExecutionFailureEvent& CExecutionFailureEvent::operator=(const CExecutionFailureEvent& ex) {
    duplicate(ex);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CExecutionFailureEvent::operator==(const CExecutionFailureEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CExecutionFailureEvent& v1, const CExecutionFailureEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CExecutionFailureEvent> CExecutionFailureEventArray;
extern CArchive& operator>>(CArchive& archive, CExecutionFailureEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CExecutionFailureEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

