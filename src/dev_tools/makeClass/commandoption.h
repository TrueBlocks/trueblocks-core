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
class CCommandOption : public CBaseNode {
public:
    string_q num;
    string_q group;
    string_q api_route;
    string_q tool;
    string_q command;
    string_q hotkey;
    string_q def_val;
    string_q is_required;
    string_q core_visible;
    string_q docs_visible;
    string_q generate;
    string_q option_kind;
    string_q data_type;
    string_q description;

public:
    CCommandOption(void);
    CCommandOption(const CCommandOption& op);
    virtual ~CCommandOption(void);
    CCommandOption& operator=(const CCommandOption& op);

    DECLARE_NODE(CCommandOption);

    // EXISTING_CODE
    CCommandOption(const string_q& line);
    // EXISTING_CODE
    bool operator==(const CCommandOption& item) const;
    bool operator!=(const CCommandOption& item) const { return !operator==(item); }
    friend bool operator<(const CCommandOption& v1, const CCommandOption& v2);
    friend ostream& operator<<(ostream& os, const CCommandOption& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCommandOption& op);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCommandOption::CCommandOption(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCommandOption::CCommandOption(const CCommandOption& op) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(op);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCommandOption::~CCommandOption(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCommandOption::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCommandOption::initialize(void) {
    CBaseNode::initialize();

    num = "";
    group = "";
    api_route = "";
    tool = "";
    command = "";
    hotkey = "";
    def_val = "";
    is_required = "";
    core_visible = "";
    docs_visible = "";
    generate = "";
    option_kind = "";
    data_type = "";
    description = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCommandOption::duplicate(const CCommandOption& op) {
    clear();
    CBaseNode::duplicate(op);

    num = op.num;
    group = op.group;
    api_route = op.api_route;
    tool = op.tool;
    command = op.command;
    hotkey = op.hotkey;
    def_val = op.def_val;
    is_required = op.is_required;
    core_visible = op.core_visible;
    docs_visible = op.docs_visible;
    generate = op.generate;
    option_kind = op.option_kind;
    data_type = op.data_type;
    description = op.description;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCommandOption& CCommandOption::operator=(const CCommandOption& op) {
    duplicate(op);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCommandOption::operator==(const CCommandOption& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCommandOption& v1, const CCommandOption& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCommandOption> CCommandOptionArray;
extern CArchive& operator>>(CArchive& archive, CCommandOptionArray& array);
extern CArchive& operator<<(CArchive& archive, const CCommandOptionArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_OPTIONDEF;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

