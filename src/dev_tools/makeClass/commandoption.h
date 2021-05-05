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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
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
    string_q api_group;
    string_q api_route;
    string_q tool;
    string_q command;
    string_q hotkey;
    string_q def_val;
    bool is_required;
    bool is_customizable;
    bool is_visible;
    bool is_visible_docs;
    string_q generate;
    string_q option_type;
    string_q data_type;
    string_q real_type;
    string_q description;

  public:
    CCommandOption(void);
    CCommandOption(const CCommandOption& co);
    virtual ~CCommandOption(void);
    CCommandOption& operator=(const CCommandOption& co);

    DECLARE_NODE(CCommandOption);

    // EXISTING_CODE
    bool isEnumList;
    bool isStringList;
    bool isAddressList;
    bool isList;
    bool isEnum;
    bool isBool;
    bool isBlockNum;
    bool isUint32;
    bool isUint64;
    bool isDouble;
    bool isNote;
    bool isError;
    explicit CCommandOption(const string_q& line);
    void verifyOptions(CStringArray& warnings);
    void verifyHotkey(CStringArray& warnings);
    string_q debugCode(void) const;
    string_q swagger_descr;
    // EXISTING_CODE
    bool operator==(const CCommandOption& it) const;
    bool operator!=(const CCommandOption& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CCommandOption& v1, const CCommandOption& v2);
    friend ostream& operator<<(ostream& os, const CCommandOption& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCommandOption& co);
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
inline CCommandOption::CCommandOption(const CCommandOption& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
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
    api_group = "";
    api_route = "";
    tool = "";
    command = "";
    hotkey = "";
    def_val = "";
    is_required = false;
    is_customizable = false;
    is_visible = false;
    is_visible_docs = false;
    generate = "";
    option_type = "";
    data_type = "";
    real_type = "";
    description = "";

    // EXISTING_CODE
    isEnumList = false;
    isStringList = false;
    isAddressList = false;
    isList = false;
    isEnum = false;
    isBool = false;
    isBlockNum = false;
    isUint32 = false;
    isUint64 = false;
    isDouble = false;
    isNote = false;
    isError = false;
    swagger_descr = "";
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCommandOption::duplicate(const CCommandOption& co) {
    clear();
    CBaseNode::duplicate(co);

    num = co.num;
    group = co.group;
    api_group = co.api_group;
    api_route = co.api_route;
    tool = co.tool;
    command = co.command;
    hotkey = co.hotkey;
    def_val = co.def_val;
    is_required = co.is_required;
    is_customizable = co.is_customizable;
    is_visible = co.is_visible;
    is_visible_docs = co.is_visible_docs;
    generate = co.generate;
    option_type = co.option_type;
    data_type = co.data_type;
    real_type = co.real_type;
    description = co.description;

    // EXISTING_CODE
    isList = co.isList;
    isEnumList = co.isEnumList;
    isStringList = co.isStringList;
    isAddressList = co.isAddressList;
    isEnum = co.isEnum;
    isBool = co.isBool;
    isBlockNum = co.isBlockNum;
    isUint32 = co.isUint32;
    isUint64 = co.isUint64;
    isDouble = co.isDouble;
    isNote = co.isNote;
    isError = co.isError;
    swagger_descr = co.swagger_descr;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCommandOption& CCommandOption::operator=(const CCommandOption& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCommandOption::operator==(const CCommandOption& it) const {
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
extern const char* STR_DISPLAY_COMMANDOPTION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
