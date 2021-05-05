#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "configitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CConfigSection : public CBaseNode {
  public:
    string_q section;
    string_q name;
    CConfigItemArray keys;

  public:
    CConfigSection(void);
    CConfigSection(const CConfigSection& co);
    virtual ~CConfigSection(void);
    CConfigSection& operator=(const CConfigSection& co);

    DECLARE_NODE(CConfigSection);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    explicit CConfigSection(const string_q& s, const string_q& n) : section(s), name(n) {
    }
    // EXISTING_CODE
    bool operator==(const CConfigSection& it) const;
    bool operator!=(const CConfigSection& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CConfigSection& v1, const CConfigSection& v2);
    friend ostream& operator<<(ostream& os, const CConfigSection& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfigSection& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfigSection::CConfigSection(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigSection::CConfigSection(const CConfigSection& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfigSection::~CConfigSection(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigSection::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigSection::initialize(void) {
    CBaseNode::initialize();

    section = "";
    name = "";
    keys.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigSection::duplicate(const CConfigSection& co) {
    clear();
    CBaseNode::duplicate(co);

    section = co.section;
    name = co.name;
    keys = co.keys;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigSection& CConfigSection::operator=(const CConfigSection& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfigSection::operator==(const CConfigSection& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfigSection& v1, const CConfigSection& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfigSection> CConfigSectionArray;
extern CArchive& operator>>(CArchive& archive, CConfigSectionArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfigSectionArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CONFIGSECTION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
