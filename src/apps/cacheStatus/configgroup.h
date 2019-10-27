#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "etherlib.h"
#include "configitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CConfigGroup : public CBaseNode {
public:
    string_q section;
    string_q name;
    CConfigItemArray keys;

public:
    CConfigGroup(void);
    CConfigGroup(const CConfigGroup& co);
    virtual ~CConfigGroup(void);
    CConfigGroup& operator=(const CConfigGroup& co);

    DECLARE_NODE(CConfigGroup);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    explicit CConfigGroup(const string_q& s, const string_q& n) : section(s), name(n) { }
    // EXISTING_CODE
    bool operator==(const CConfigGroup& item) const;
    bool operator!=(const CConfigGroup& item) const { return !operator==(item); }
    friend bool operator<(const CConfigGroup& v1, const CConfigGroup& v2);
    friend ostream& operator<<(ostream& os, const CConfigGroup& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfigGroup& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfigGroup::CConfigGroup(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigGroup::CConfigGroup(const CConfigGroup& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfigGroup::~CConfigGroup(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigGroup::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigGroup::initialize(void) {
    CBaseNode::initialize();

    section = "";
    name = "";
    keys.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigGroup::duplicate(const CConfigGroup& co) {
    clear();
    CBaseNode::duplicate(co);

    section = co.section;
    name = co.name;
    keys = co.keys;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigGroup& CConfigGroup::operator=(const CConfigGroup& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfigGroup::operator==(const CConfigGroup& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfigGroup& v1, const CConfigGroup& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfigGroup> CConfigGroupArray;
extern CArchive& operator>>(CArchive& archive, CConfigGroupArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfigGroupArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CConfigGroup& con);
extern CArchive& operator>>(CArchive& archive, CConfigGroup& con);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CONFIGGROUP;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

