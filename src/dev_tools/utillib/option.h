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
#include "sfarchive.h"

namespace qblocks {

// EXISTING_CODE
// Bit flags to enable / disable various options
#define OPT_DESCRIPTION (0)
#define OPT_HELP (1 << 1)
#define OPT_VERBOSE (1 << 2)
#define OPT_FMT (1 << 3)
#define OPT_UNUSED (1 << 4)
#define OPT_RAW (1 << 11)
#define OPT_PREFUND (1 << 12)
#define OPT_OUTPUT (1 << 13)
#define OPT_CHECKLATEST (1 << 21)
#define OPT_DEFAULT (OPT_HELP | OPT_VERBOSE | OPT_FMT | OPT_OUTPUT | OPT_CHECKLATEST)

#define OPT_REQUIRED (1 << 14)
#define OPT_POSITIONAL (1 << 15)
#define OPT_FLAG (1 << 16)
#define OPT_SWITCH OPT_FLAG
#define OPT_TOGGLE OPT_SWITCH
#define OPT_HIDDEN (1 << 17)
#define OPT_DEPRECATED (OPT_HIDDEN | (1 << 18))
#define NOOPT ((uint32_t)-1)
// EXISTING_CODE

class COption {
  public:
    string_q hotKey;
    string_q longName;
    string_q description;
    string_q permitted;
    string_q option_type;
    bool is_visible;
    bool is_positional;
    bool is_required;
    bool is_deprecated;

  public:
    COption(void);
    COption(const COption& op);
    virtual ~COption(void);
    COption& operator=(const COption& op);

    COption(const string_q& ln, const string_q& sn, const string_q& type, size_t opts, const string_q& d);
    bool operator==(const COption& it) const;
    bool operator!=(const COption& it) const {
        return !operator==(it);
    }
    friend bool operator<(const COption& v1, const COption& v2);
    friend ostream& operator<<(ostream& os, const COption& it);
};

//--------------------------------------------------------------------------
inline COption::COption(void) {
    hotKey = "";
    longName = "";
    description = "";
    permitted = "";
    option_type = "";
    is_visible = false;
    is_positional = false;
    is_required = false;
    is_deprecated = false;
}

//--------------------------------------------------------------------------
inline COption::COption(const COption& op) {
    hotKey = op.hotKey;
    longName = op.longName;
    description = op.description;
    permitted = op.permitted;
    option_type = op.option_type;
    is_visible = op.is_visible;
    is_positional = op.is_positional;
    is_required = op.is_required;
    is_deprecated = op.is_deprecated;
}

//--------------------------------------------------------------------------
inline COption::~COption(void) {
}

inline COption& COption::operator=(const COption& op) {
    hotKey = op.hotKey;
    longName = op.longName;
    description = op.description;
    permitted = op.permitted;
    option_type = op.option_type;
    is_visible = op.is_visible;
    is_positional = op.is_positional;
    is_required = op.is_required;
    is_deprecated = op.is_deprecated;
    return *this;
}

//-------------------------------------------------------------------------
inline bool COption::operator==(const COption& it) const {
    return false;
}

}  // namespace qblocks
