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
class CSchema : public CBaseNode {
  public:
    string_q name;
    string_q selector;
    string_q type;
    bool hidden;
    uint64_t width;
    string_q function;
    bool editable;
    uint64_t id;
    uint64_t decimals;
    bool pill;
    bool hideZero;
    string_q align;
    string_q cn;
    bool domain;
    bool range;
    string_q onAccept;
    string_q onValidate;

  public:
    CSchema(void);
    CSchema(const CSchema& sc);
    virtual ~CSchema(void);
    CSchema& operator=(const CSchema& sc);

    DECLARE_NODE(CSchema);

    // EXISTING_CODE
    CSchema(string_q& line);
    bool showEmptyField(const string_q& fn) const override;
    // EXISTING_CODE
    bool operator==(const CSchema& item) const;
    bool operator!=(const CSchema& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CSchema& v1, const CSchema& v2);
    friend ostream& operator<<(ostream& os, const CSchema& item);

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
    hidden = false;
    width = 0;
    function = "";
    editable = false;
    id = 0;
    decimals = 0;
    pill = false;
    hideZero = false;
    align = "";
    cn = "";
    domain = false;
    range = false;
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
    hidden = sc.hidden;
    width = sc.width;
    function = sc.function;
    editable = sc.editable;
    id = sc.id;
    decimals = sc.decimals;
    pill = sc.pill;
    hideZero = sc.hideZero;
    align = sc.align;
    cn = sc.cn;
    domain = sc.domain;
    range = sc.range;
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
inline bool CSchema::operator==(const CSchema& item) const {
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
extern const char* STR_DISPLAY_SCHEMA;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
