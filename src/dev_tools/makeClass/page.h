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
#include "subpage.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPage : public CBaseNode {
  public:
    string_q longName;
    string_q properName;
    string_q twoName;
    string_q sevenName;
    string_q singular;
    bool noPage;
    string_q dest_path;
    string_q recordIcons;
    string_q imports;
    string_q defaultTable;
    string_q defaultSearch;
    string_q defaultSort;
    string_q defaultTags;
    string_q dataUrl;
    string_q dataQuery;
    string_q cmdUrl;
    string_q schema;
    CSubpageArray subpages;

  public:
    CPage(void);
    CPage(const CPage& pa);
    virtual ~CPage(void);
    CPage& operator=(const CPage& pa);

    DECLARE_NODE(CPage);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPage& it) const;
    bool operator!=(const CPage& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPage& v1, const CPage& v2);
    friend ostream& operator<<(ostream& os, const CPage& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPage& pa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPage::CPage(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPage::CPage(const CPage& pa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPage::~CPage(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPage::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPage::initialize(void) {
    CBaseNode::initialize();

    longName = "";
    properName = "";
    twoName = "";
    sevenName = "";
    singular = "";
    noPage = false;
    dest_path = "";
    recordIcons = "";
    imports = "";
    defaultTable = "";
    defaultSearch = "";
    defaultSort = "";
    defaultTags = "";
    dataUrl = "";
    dataQuery = "";
    cmdUrl = "";
    schema = "";
    subpages.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPage::duplicate(const CPage& pa) {
    clear();
    CBaseNode::duplicate(pa);

    longName = pa.longName;
    properName = pa.properName;
    twoName = pa.twoName;
    sevenName = pa.sevenName;
    singular = pa.singular;
    noPage = pa.noPage;
    dest_path = pa.dest_path;
    recordIcons = pa.recordIcons;
    imports = pa.imports;
    defaultTable = pa.defaultTable;
    defaultSearch = pa.defaultSearch;
    defaultSort = pa.defaultSort;
    defaultTags = pa.defaultTags;
    dataUrl = pa.dataUrl;
    dataQuery = pa.dataQuery;
    cmdUrl = pa.cmdUrl;
    schema = pa.schema;
    subpages = pa.subpages;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPage& CPage::operator=(const CPage& pa) {
    duplicate(pa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPage::operator==(const CPage& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPage& v1, const CPage& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPage> CPageArray;
extern CArchive& operator>>(CArchive& archive, CPageArray& array);
extern CArchive& operator<<(CArchive& archive, const CPageArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PAGE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
