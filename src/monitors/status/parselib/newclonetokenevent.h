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
class CNewCloneTokenEvent : public CLogEntry {
public:
    address_t _cloneToken;
    biguint_t _snapshotBlock;

public:
    CNewCloneTokenEvent(void);
    CNewCloneTokenEvent(const CNewCloneTokenEvent& ne);
    virtual ~CNewCloneTokenEvent(void);
    CNewCloneTokenEvent& operator=(const CNewCloneTokenEvent& ne);

    DECLARE_NODE(CNewCloneTokenEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewCloneTokenEvent& item) const;
    bool operator!=(const CNewCloneTokenEvent& item) const { return !operator==(item); }
    friend bool operator<(const CNewCloneTokenEvent& v1, const CNewCloneTokenEvent& v2);
    friend ostream& operator<<(ostream& os, const CNewCloneTokenEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewCloneTokenEvent& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewCloneTokenEvent::CNewCloneTokenEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewCloneTokenEvent::CNewCloneTokenEvent(const CNewCloneTokenEvent& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewCloneTokenEvent::~CNewCloneTokenEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewCloneTokenEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewCloneTokenEvent::initialize(void) {
    CLogEntry::initialize();

    _cloneToken = "";
    _snapshotBlock = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewCloneTokenEvent::duplicate(const CNewCloneTokenEvent& ne) {
    clear();
    CLogEntry::duplicate(ne);

    _cloneToken = ne._cloneToken;
    _snapshotBlock = ne._snapshotBlock;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewCloneTokenEvent& CNewCloneTokenEvent::operator=(const CNewCloneTokenEvent& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewCloneTokenEvent::operator==(const CNewCloneTokenEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewCloneTokenEvent& v1, const CNewCloneTokenEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewCloneTokenEvent> CNewCloneTokenEventArray;
extern CArchive& operator>>(CArchive& archive, CNewCloneTokenEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewCloneTokenEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

