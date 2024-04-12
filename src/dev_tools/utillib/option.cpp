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
#include "option.h"

namespace qblocks {

COption::COption(const string_q& ln, const string_q& sn, const string_q& t, size_t opts, const string_q& d) {
    description = substitute(d, "&#44;", ",");

    is_positional = (opts & OPT_POSITIONAL);
    is_visible = !(opts & OPT_HIDDEN);
    is_required = (opts & OPT_REQUIRED);
    is_deprecated = (opts == OPT_DEPRECATED);

    option_type = t;
    permitted = t;
    permitted = substitute(permitted, "<uint32>", "<num>");
    permitted = substitute(permitted, "<uint64>", "<num>");
    permitted = substitute(permitted, "<blknum>", "<num>");
    permitted = substitute(permitted, "<string>", "<str>");
    permitted = substitute(permitted, "list<topic>", "<hash>");
    permitted = substitute(permitted, "list<addr>", "<addr>");
    if (contains(option_type, "enum")) {
        description +=
            ", one [X] of " + substitute(substitute(substitute(option_type, "list<", ""), ">", ""), "enum", "");
        replace(description, " [X]", (contains(option_type, "list") ? " or more" : ""));
        permitted = "<val>";
    }
    if (permitted == "<str>" || permitted == "<hash>" || permitted == "<addr>")
        permitted = "string";

    hotKey = (sn.empty() ? "" : "-" + sn);
    if (ln.empty())
        return;

    if (!(opts & OPT_HELP || opts & OPT_VERBOSE)) {
        longName = "--" + ln + (permitted.empty() ? "" : " " + permitted);
        if (is_positional)
            longName = ln;
    } else {
        longName = "--" + ln;
    }
}

// EXISTING_CODE
}  // namespace qblocks
