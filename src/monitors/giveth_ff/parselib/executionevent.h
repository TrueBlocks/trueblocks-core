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
class CExecutionEvent : public CLogEntry {
public:
    biguint_t transactionId;

public:
    CExecutionEvent(void);
    CExecutionEvent(const CExecutionEvent& ex);
    virtual ~CExecutionEvent(void);
    CExecutionEvent& operator=(const CExecutionEvent& ex);

    DECLARE_NODE(CExecutionEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CExecutionEvent& item) const;
    bool operator!=(const CExecutionEvent& item) const { return !operator==(item); }
    friend bool operator<(const CExecutionEvent& v1, const CExecutionEvent& v2);
    friend ostream& operator<<(ostream& os, const CExecutionEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CExecutionEvent& ex);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CExecutionEvent::CExecutionEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CExecutionEvent::CExecutionEvent(const CExecutionEvent& ex) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ex);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CExecutionEvent::~CExecutionEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecutionEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecutionEvent::initialize(void) {
    CLogEntry::initialize();

    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecutionEvent::duplicate(const CExecutionEvent& ex) {
    clear();
    CLogEntry::duplicate(ex);

    transactionId = ex.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CExecutionEvent& CExecutionEvent::operator=(const CExecutionEvent& ex) {
    duplicate(ex);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CExecutionEvent::operator==(const CExecutionEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CExecutionEvent& v1, const CExecutionEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CExecutionEvent> CExecutionEventArray;
extern CArchive& operator>>(CArchive& archive, CExecutionEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CExecutionEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

