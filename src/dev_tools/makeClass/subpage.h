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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CSubpage : public CBaseNode {
  public:
    string_q page;
    string_q subpage;
    string_q route;
    string_q options;
    string_q component;
    string_q extract;
    string_q icon;

  public:
    CSubpage(void);
    CSubpage(const CSubpage& su);
    virtual ~CSubpage(void);
    CSubpage& operator=(const CSubpage& su);

    DECLARE_NODE(CSubpage);

    // EXISTING_CODE
    bool isSeparator;
    // EXISTING_CODE
    bool operator==(const CSubpage& item) const;
    bool operator!=(const CSubpage& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CSubpage& v1, const CSubpage& v2);
    friend ostream& operator<<(ostream& os, const CSubpage& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSubpage& su);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSubpage::CSubpage(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSubpage::CSubpage(const CSubpage& su) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(su);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSubpage::~CSubpage(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSubpage::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSubpage::initialize(void) {
    CBaseNode::initialize();

    page = "";
    subpage = "";
    route = "";
    options = "";
    component = "";
    extract = "";
    icon = "";

    // EXISTING_CODE
    isSeparator = false;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSubpage::duplicate(const CSubpage& su) {
    clear();
    CBaseNode::duplicate(su);

    page = su.page;
    subpage = su.subpage;
    route = su.route;
    options = su.options;
    component = su.component;
    extract = su.extract;
    icon = su.icon;

    // EXISTING_CODE
    isSeparator = su.isSeparator;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSubpage& CSubpage::operator=(const CSubpage& su) {
    duplicate(su);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSubpage::operator==(const CSubpage& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSubpage& v1, const CSubpage& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSubpage> CSubpageArray;
extern CArchive& operator>>(CArchive& archive, CSubpageArray& array);
extern CArchive& operator<<(CArchive& archive, const CSubpageArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_SUBPAGE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
