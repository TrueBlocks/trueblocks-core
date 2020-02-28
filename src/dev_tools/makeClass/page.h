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
    CSubpageArray subpages;
    bool polling;
    string_q menuType;
    string_q files;
    bool no_error;
    bool no_data;
    bool obj_table;
    bool dat_table;
    bool no_dash;
    bool has_text;
    string_q color;

  public:
    CPage(void);
    CPage(const CPage& pa);
    virtual ~CPage(void);
    CPage& operator=(const CPage& pa);

    DECLARE_NODE(CPage);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPage& item) const;
    bool operator!=(const CPage& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CPage& v1, const CPage& v2);
    friend ostream& operator<<(ostream& os, const CPage& item);

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
    subpages.clear();
    polling = false;
    menuType = "";
    files = "";
    no_error = false;
    no_data = false;
    obj_table = false;
    dat_table = false;
    no_dash = false;
    has_text = false;
    color = "";

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
    subpages = pa.subpages;
    polling = pa.polling;
    menuType = pa.menuType;
    files = pa.files;
    no_error = pa.no_error;
    no_data = pa.no_data;
    obj_table = pa.obj_table;
    dat_table = pa.dat_table;
    no_dash = pa.no_dash;
    has_text = pa.has_text;
    color = pa.color;

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
inline bool CPage::operator==(const CPage& item) const {
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
