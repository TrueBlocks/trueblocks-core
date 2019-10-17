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
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class COptionDef : public CBaseNode {
public:
    string_q num;
    string_q group;
    string_q api_route;
    string_q tool;
    string_q order;
    string_q command;
    string_q command_short;
    string_q data_type;
    string_q option_kind;
    string_q default_value;
    string_q description_core;
    string_q description_api;
    string_q is_required;
    string_q core_visible;
    string_q docs_visible;

public:
    COptionDef(void);
    COptionDef(const COptionDef& op);
    virtual ~COptionDef(void);
    COptionDef& operator=(const COptionDef& op);

    DECLARE_NODE(COptionDef);

    // EXISTING_CODE
    COptionDef(const string_q& line);
    // EXISTING_CODE
    bool operator==(const COptionDef& item) const;
    bool operator!=(const COptionDef& item) const { return !operator==(item); }
    friend bool operator<(const COptionDef& v1, const COptionDef& v2);
    friend ostream& operator<<(ostream& os, const COptionDef& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COptionDef& op);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COptionDef::COptionDef(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COptionDef::COptionDef(const COptionDef& op) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(op);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COptionDef::~COptionDef(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COptionDef::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COptionDef::initialize(void) {
    CBaseNode::initialize();

    num = "";
    group = "";
    api_route = "";
    tool = "";
    order = "";
    command = "";
    command_short = "";
    data_type = "";
    option_kind = "";
    default_value = "";
    description_core = "";
    description_api = "";
    is_required = "";
    core_visible = "";
    docs_visible = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COptionDef::duplicate(const COptionDef& op) {
    clear();
    CBaseNode::duplicate(op);

    num = op.num;
    group = op.group;
    api_route = op.api_route;
    tool = op.tool;
    order = op.order;
    command = op.command;
    command_short = op.command_short;
    data_type = op.data_type;
    option_kind = op.option_kind;
    default_value = op.default_value;
    description_core = op.description_core;
    description_api = op.description_api;
    is_required = op.is_required;
    core_visible = op.core_visible;
    docs_visible = op.docs_visible;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COptionDef& COptionDef::operator=(const COptionDef& op) {
    duplicate(op);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COptionDef::operator==(const COptionDef& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COptionDef& v1, const COptionDef& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COptionDef> COptionDefArray;
extern CArchive& operator>>(CArchive& archive, COptionDefArray& array);
extern CArchive& operator<<(CArchive& archive, const COptionDefArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_OPTIONDEF;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

