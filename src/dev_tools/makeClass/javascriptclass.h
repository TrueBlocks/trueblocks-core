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
class CJavascriptDef : public CBaseNode {
  public:
    string_q longName;
    string_q properName;
    string_q pageNotes;
    string_q query_url;
    string_q query_opts;
    string_q query_extract;
    string_q subpage;
    string_q state;
    bool polling;
    string_q files;
    string_q menuType;

  public:
    CJavascriptDef(void);
    CJavascriptDef(const CJavascriptDef& ja);
    virtual ~CJavascriptDef(void);
    CJavascriptDef& operator=(const CJavascriptDef& ja);

    DECLARE_NODE(CJavascriptDef);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CJavascriptDef& item) const;
    bool operator!=(const CJavascriptDef& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CJavascriptDef& v1, const CJavascriptDef& v2);
    friend ostream& operator<<(ostream& os, const CJavascriptDef& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CJavascriptDef& ja);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CJavascriptDef::CJavascriptDef(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CJavascriptDef::CJavascriptDef(const CJavascriptDef& ja) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ja);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CJavascriptDef::~CJavascriptDef(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CJavascriptDef::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CJavascriptDef::initialize(void) {
    CBaseNode::initialize();

    longName = "";
    properName = "";
    pageNotes = "";
    query_url = "";
    query_opts = "";
    query_extract = "";
    subpage = "";
    state = "";
    polling = false;
    files = "";
    menuType = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CJavascriptDef::duplicate(const CJavascriptDef& ja) {
    clear();
    CBaseNode::duplicate(ja);

    longName = ja.longName;
    properName = ja.properName;
    pageNotes = ja.pageNotes;
    query_url = ja.query_url;
    query_opts = ja.query_opts;
    query_extract = ja.query_extract;
    subpage = ja.subpage;
    state = ja.state;
    polling = ja.polling;
    files = ja.files;
    menuType = ja.menuType;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CJavascriptDef& CJavascriptDef::operator=(const CJavascriptDef& ja) {
    duplicate(ja);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CJavascriptDef::operator==(const CJavascriptDef& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CJavascriptDef& v1, const CJavascriptDef& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CJavascriptDef> CJavascriptDefArray;
extern CArchive& operator>>(CArchive& archive, CJavascriptDefArray& array);
extern CArchive& operator<<(CArchive& archive, const CJavascriptDefArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_JAVASCRIPTDEF;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
