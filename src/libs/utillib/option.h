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
#include "basetypes.h"
#include "basenode.h"
#include "sfarchive.h"

namespace qblocks {

// EXISTING_CODE
// Bit flags to enable / disable various options
#define OPT_DESCRIPTION (0)
#define OPT_HELP (1 << 1)
#define OPT_VERBOSE (1 << 2)
#define OPT_FMT (1 << 3)
#define OPT_DOLLARS (1 << 4)
#define OPT_WEI (1 << 5)
#define OPT_ETHER (1 << 6)
#define OPT_PARITY (1 << 7)
#define OPT_RAW (1 << 11)
#define OPT_PREFUND (1 << 12)
#define OPT_OUTPUT (1 << 13)
#define OPT_CRUD (1 << 14)
#define OPT_MOCKDATA (1 << 21)
#define OPT_DENOM (OPT_DOLLARS | OPT_WEI | OPT_ETHER)
#define OPT_DEFAULT (OPT_HELP | OPT_VERBOSE | OPT_FMT | OPT_DENOM | OPT_PARITY | OPT_MOCKDATA | OPT_OUTPUT)

#define OPT_REQUIRED (1 << 14)
#define OPT_POSITIONAL (1 << 15)
#define OPT_FLAG (1 << 16)
#define OPT_SWITCH OPT_FLAG
#define OPT_TOGGLE OPT_SWITCH
#define OPT_HIDDEN (1 << 17)
#define OPT_DEPRECATED (OPT_HIDDEN | (1 << 18))
#define NOOPT ((uint32_t)-1)
// EXISTING_CODE

//--------------------------------------------------------------------------
class COption : public CBaseNode {
  public:
    string_q hotKey;
    string_q longName;
    string_q description;
    string_q permitted;
    string_q type;
    bool is_hidden;
    bool is_positional;
    bool is_optional;
    bool is_deprecated;
    bool is_readme;

  public:
    COption(void);
    COption(const COption& op);
    virtual ~COption(void);
    COption& operator=(const COption& op);

    DECLARE_NODE(COption);

    // EXISTING_CODE
    COption(const string_q& ln, const string_q& sn, const string_q& type, size_t opts, const string_q& d);
    bool isPublic(void) const;
    string_q readmeDash(const string_q& str, bool isReadme) const;
    string_q getHotKey(bool isReadme) const;
    string_q getLongKey(bool isReadme) const;
    string_q getDescription(bool isReadme) const;
    string_q oneDescription(bool isReadme, size_t* widths) const;
    // EXISTING_CODE
    bool operator==(const COption& it) const;
    bool operator!=(const COption& it) const {
        return !operator==(it);
    }
    friend bool operator<(const COption& v1, const COption& v2);
    friend ostream& operator<<(ostream& os, const COption& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COption& op);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COption::COption(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COption::COption(const COption& op) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(op);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COption::~COption(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COption::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COption::initialize(void) {
    CBaseNode::initialize();

    hotKey = "";
    longName = "";
    description = "";
    permitted = "";
    type = "";
    is_hidden = false;
    is_positional = false;
    is_optional = false;
    is_deprecated = false;
    is_readme = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COption::duplicate(const COption& op) {
    clear();
    CBaseNode::duplicate(op);

    hotKey = op.hotKey;
    longName = op.longName;
    description = op.description;
    permitted = op.permitted;
    type = op.type;
    is_hidden = op.is_hidden;
    is_positional = op.is_positional;
    is_optional = op.is_optional;
    is_deprecated = op.is_deprecated;
    is_readme = op.is_readme;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COption& COption::operator=(const COption& op) {
    duplicate(op);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COption::operator==(const COption& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COption& v1, const COption& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COption> COptionArray;
extern CArchive& operator>>(CArchive& archive, COptionArray& array);
extern CArchive& operator<<(CArchive& archive, const COptionArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const COption& opt);
extern CArchive& operator>>(CArchive& archive, COption& opt);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_OPTION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
