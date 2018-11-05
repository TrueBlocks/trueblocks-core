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
class CTokenSwapOverEvent : public CLogEntry {
public:
    CTokenSwapOverEvent(void);
    CTokenSwapOverEvent(const CTokenSwapOverEvent& to);
    virtual ~CTokenSwapOverEvent(void);
    CTokenSwapOverEvent& operator=(const CTokenSwapOverEvent& to);

    DECLARE_NODE(CTokenSwapOverEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTokenSwapOverEvent& item) const;
    bool operator!=(const CTokenSwapOverEvent& item) const { return !operator==(item); }
    friend bool operator<(const CTokenSwapOverEvent& v1, const CTokenSwapOverEvent& v2);
    friend ostream& operator<<(ostream& os, const CTokenSwapOverEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTokenSwapOverEvent& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTokenSwapOverEvent::CTokenSwapOverEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTokenSwapOverEvent::CTokenSwapOverEvent(const CTokenSwapOverEvent& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTokenSwapOverEvent::~CTokenSwapOverEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenSwapOverEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenSwapOverEvent::initialize(void) {
    CLogEntry::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenSwapOverEvent::duplicate(const CTokenSwapOverEvent& to) {
    clear();
    CLogEntry::duplicate(to);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTokenSwapOverEvent& CTokenSwapOverEvent::operator=(const CTokenSwapOverEvent& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTokenSwapOverEvent::operator==(const CTokenSwapOverEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTokenSwapOverEvent& v1, const CTokenSwapOverEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTokenSwapOverEvent> CTokenSwapOverEventArray;
extern CArchive& operator>>(CArchive& archive, CTokenSwapOverEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CTokenSwapOverEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

