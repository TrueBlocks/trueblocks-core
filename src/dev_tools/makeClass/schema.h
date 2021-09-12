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
class CSchema : public CBaseNode {
  public:
    string_q name;
    string_q selector;
    string_q type;
    string_q unused;
    uint64_t width;
    bool editable;
    uint64_t id;
    uint64_t decimals;
    bool isPill;
    string_q align;
    string_q cn;
    bool download;
    string_q chart;
    bool searchable;
    uint64_t detail;
    bool wide;
    string_q underField;
    string_q onDisplay;
    string_q onAccept;
    string_q onValidate;

  public:
    CSchema(void);
    CSchema(const CSchema& sc);
    virtual ~CSchema(void);
    CSchema& operator=(const CSchema& sc);

    DECLARE_NODE(CSchema);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSchema& it) const;
    bool operator!=(const CSchema& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CSchema& v1, const CSchema& v2);
    friend ostream& operator<<(ostream& os, const CSchema& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSchema& sc);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSchema::CSchema(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSchema::CSchema(const CSchema& sc) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(sc);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSchema::~CSchema(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSchema::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSchema::initialize(void) {
    CBaseNode::initialize();

    name = "";
    selector = "";
    type = "";
    unused = "";
    width = 0;
    editable = false;
    id = 0;
    decimals = 0;
    isPill = false;
    align = "";
    cn = "";
    download = false;
    chart = "";
    searchable = false;
    detail = 0;
    wide = false;
    underField = "";
    onDisplay = "";
    onAccept = "";
    onValidate = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSchema::duplicate(const CSchema& sc) {
    clear();
    CBaseNode::duplicate(sc);

    name = sc.name;
    selector = sc.selector;
    type = sc.type;
    unused = sc.unused;
    width = sc.width;
    editable = sc.editable;
    id = sc.id;
    decimals = sc.decimals;
    isPill = sc.isPill;
    align = sc.align;
    cn = sc.cn;
    download = sc.download;
    chart = sc.chart;
    searchable = sc.searchable;
    detail = sc.detail;
    wide = sc.wide;
    underField = sc.underField;
    onDisplay = sc.onDisplay;
    onAccept = sc.onAccept;
    onValidate = sc.onValidate;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSchema& CSchema::operator=(const CSchema& sc) {
    duplicate(sc);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSchema::operator==(const CSchema& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSchema& v1, const CSchema& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSchema> CSchemaArray;
extern CArchive& operator>>(CArchive& archive, CSchemaArray& array);
extern CArchive& operator<<(CArchive& archive, const CSchemaArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CSchema& sch);
extern CArchive& operator>>(CArchive& archive, CSchema& sch);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_SCHEMA;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
