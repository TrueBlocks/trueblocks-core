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
class CUnfreezeEvent : public CLogEntry {
public:
    address_t from;
    biguint_t value;

public:
    CUnfreezeEvent(void);
    CUnfreezeEvent(const CUnfreezeEvent& un);
    virtual ~CUnfreezeEvent(void);
    CUnfreezeEvent& operator=(const CUnfreezeEvent& un);

    DECLARE_NODE(CUnfreezeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnfreezeEvent& item) const;
    bool operator!=(const CUnfreezeEvent& item) const { return !operator==(item); }
    friend bool operator<(const CUnfreezeEvent& v1, const CUnfreezeEvent& v2);
    friend ostream& operator<<(ostream& os, const CUnfreezeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnfreezeEvent& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnfreezeEvent::CUnfreezeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnfreezeEvent::CUnfreezeEvent(const CUnfreezeEvent& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnfreezeEvent::~CUnfreezeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnfreezeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnfreezeEvent::initialize(void) {
    CLogEntry::initialize();

    from = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnfreezeEvent::duplicate(const CUnfreezeEvent& un) {
    clear();
    CLogEntry::duplicate(un);

    from = un.from;
    value = un.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnfreezeEvent& CUnfreezeEvent::operator=(const CUnfreezeEvent& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnfreezeEvent::operator==(const CUnfreezeEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnfreezeEvent& v1, const CUnfreezeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnfreezeEvent> CUnfreezeEventArray;
extern CArchive& operator>>(CArchive& archive, CUnfreezeEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CUnfreezeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

