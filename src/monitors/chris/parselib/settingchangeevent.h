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
class CSettingChangeEvent : public CLogEntry {
public:
    address_t _admin;
    uint32_t _setting;
    biguint_t _value;

public:
    CSettingChangeEvent(void);
    CSettingChangeEvent(const CSettingChangeEvent& se);
    virtual ~CSettingChangeEvent(void);
    CSettingChangeEvent& operator=(const CSettingChangeEvent& se);

    DECLARE_NODE(CSettingChangeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSettingChangeEvent& item) const;
    bool operator!=(const CSettingChangeEvent& item) const { return !operator==(item); }
    friend bool operator<(const CSettingChangeEvent& v1, const CSettingChangeEvent& v2);
    friend ostream& operator<<(ostream& os, const CSettingChangeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSettingChangeEvent& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSettingChangeEvent::CSettingChangeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSettingChangeEvent::CSettingChangeEvent(const CSettingChangeEvent& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSettingChangeEvent::~CSettingChangeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSettingChangeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSettingChangeEvent::initialize(void) {
    CLogEntry::initialize();

    _admin = "";
    _setting = 0;
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSettingChangeEvent::duplicate(const CSettingChangeEvent& se) {
    clear();
    CLogEntry::duplicate(se);

    _admin = se._admin;
    _setting = se._setting;
    _value = se._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSettingChangeEvent& CSettingChangeEvent::operator=(const CSettingChangeEvent& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSettingChangeEvent::operator==(const CSettingChangeEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSettingChangeEvent& v1, const CSettingChangeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSettingChangeEvent> CSettingChangeEventArray;
extern CArchive& operator>>(CArchive& archive, CSettingChangeEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CSettingChangeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

