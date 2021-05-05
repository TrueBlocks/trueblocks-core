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
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CConfigItem : public CBaseNode {
  public:
    string_q name;
    string_q value;
    string_q type;
    string_q tip;
    bool required;
    bool read_only;
    CAccountNameArray named;

  public:
    CConfigItem(void);
    CConfigItem(const CConfigItem& co);
    virtual ~CConfigItem(void);
    CConfigItem& operator=(const CConfigItem& co);

    DECLARE_NODE(CConfigItem);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    explicit CConfigItem(const string_q& n, const string_q& v, const string_q& t, const string_q& p, bool r, bool o)
        : name(n), value(v), type(t), tip(p), required(r), read_only(o) {
    }
    // EXISTING_CODE
    bool operator==(const CConfigItem& it) const;
    bool operator!=(const CConfigItem& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CConfigItem& v1, const CConfigItem& v2);
    friend ostream& operator<<(ostream& os, const CConfigItem& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfigItem& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfigItem::CConfigItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigItem::CConfigItem(const CConfigItem& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfigItem::~CConfigItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigItem::initialize(void) {
    CBaseNode::initialize();

    name = "";
    value = "";
    type = "";
    tip = "";
    required = false;
    read_only = false;
    named.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigItem::duplicate(const CConfigItem& co) {
    clear();
    CBaseNode::duplicate(co);

    name = co.name;
    value = co.value;
    type = co.type;
    tip = co.tip;
    required = co.required;
    read_only = co.read_only;
    named = co.named;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigItem& CConfigItem::operator=(const CConfigItem& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfigItem::operator==(const CConfigItem& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfigItem& v1, const CConfigItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfigItem> CConfigItemArray;
extern CArchive& operator>>(CArchive& archive, CConfigItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfigItemArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CONFIGITEM;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
