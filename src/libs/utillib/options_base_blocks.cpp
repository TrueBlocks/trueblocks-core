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
#include "basetypes.h"
#include "options_base.h"
#include "conversions.h"
#include "logging.h"

namespace qblocks {

//--------------------------------------------------------------------------------
blknum_t COptionsBlockList::parseBlockOption(string_q& msg, blknum_t lastBlock, direction_t offset) const {
    blknum_t ret = NOPOS;

    string_q arg = msg;
    msg = "";

    // if it's a number, return it
    if (isNumeral(arg) || isHexStr(arg)) {
        ret = str_2_Uint(arg);

    } else {
        // if it's not a number, it better be a special value, and we better be able to find it
        CNameValue spec;
        if (COptionsBase::findSpecial(spec, arg)) {
            if (spec.first == "latest")
                spec.second = uint_2_Str(lastBlock);
            ret = str_2_Uint(spec.second);

        } else {
            msg =
                "The given value '" + arg + "' is not a numeral or a special named block." + (isApiMode() ? "" : "\n");
            return NOPOS;
        }
    }

    if (ret > lastBlock) {
        string_q lateStr = (isTestMode() ? "--" : uint_2_Str(lastBlock));
        msg = "Block " + arg + " is later than the last valid block " + lateStr + "." + (isApiMode() ? "" : "\n");
        return NOPOS;
    }

    if (offset == PREV && ret > 0)
        ret--;
    else if (offset == NEXT && ret < lastBlock)
        ret++;
    return ret;
}

typedef enum { NONE = 0, HOURLY, DAILY, WEEKLY, MONTHLY, QUARTERLY, ANNUALLY } skip_t;

//--------------------------------------------------------------------------------
static string_q skip_markers = "hourly|daily|weekly|monthly|quarterly|annually";
//--------------------------------------------------------------------------------
string_q COptionsBlockList::parseBlockList(const string_q& argIn, blknum_t lastBlock) {
    string_q arg = argIn;

    direction_t offset = (contains(arg, ".next") ? NEXT : contains(arg, ".prev") ? PREV : NODIR);
    arg = substitute(substitute(arg, ".next", ""), ".prev", "");

    skip_t skip_type = NONE;

    // scrape off the skip marker if any
    if (contains(arg, ":")) {
        string_q skip_marker = arg;
        arg = nextTokenClear(skip_marker, ':');
        if (isNumeral(skip_marker)) {
            skip = max(blknum_t(1), str_2_Uint(skip_marker));
        } else {
            if (!contains(skip_markers, skip_marker))
                return "Input argument appears to be invalid. No such skip marker: " + argIn;
            if (contains(skip_marker, "annually")) {
                skip_type = ANNUALLY;
            } else if (contains(skip_marker, "quarterly")) {
                skip_type = QUARTERLY;
            } else if (contains(skip_marker, "monthly")) {
                skip_type = MONTHLY;
            } else if (contains(skip_marker, "weekly")) {
                skip_type = WEEKLY;
            } else if (contains(skip_marker, "daily")) {
                skip_type = DAILY;
            } else if (contains(skip_marker, "hourly")) {
                skip_type = HOURLY;
            } else {
                ASSERT(0);  // should never happen
                skip = 1;
            }
            LOG_INFO("skip_type: ", skip_type);
        }
    }

    if (contains(arg, "-") || contains(arg, "+")) {
        // If we already have a range, bail
        if (start != stop)
            return "Specify only a single block range at a time.";

        if (startsWith(arg, "latest"))
            return "Cannot start range with 'latest'";

        if (contains(arg, "+")) {
            string_q n = nextTokenClear(arg, '+');
            blknum_t s1 = parseBlockOption(n, lastBlock, NODIR);
            if (!n.empty())
                return n;
            n = arg;
            blknum_t s2 = parseBlockOption(n, lastBlock, NODIR);
            if (!n.empty())
                return n;
            s2 = s1 + s2;
            arg = uint_2_Str(s1) + "-" + uint_2_Str(s2);
        }

        string_q stp = arg;
        string_q strt = nextTokenClear(stp, '-');
        string_q msg = strt;
        start = parseBlockOption(msg, lastBlock, offset);
        if (!msg.empty())
            return msg;
        msg = stp;
        stop = parseBlockOption(msg, lastBlock, offset);
        if (!msg.empty())
            return msg;

        if (stop <= start)
            return "'stop' must be strictly larger than 'start'";

    } else {
        if (isHash(arg)) {
            hashList.push_back(arg);

        } else {
            string_q msg = arg;
            blknum_t num = parseBlockOption(msg, lastBlock, offset);
            if (!msg.empty())
                return msg;
            numList.push_back(num);
            // CTimeArray times;
            // extern bool expandHourly(CTimeArray& ta, const time_q& start, size_t n, bool fallback = true);
            // extern bool expandDaily(CTimeArray& ta, const time_q& start, size_t n, bool fallback = true);
            // extern bool expandWeekly(CTimeArray& ta, const time_q& start, size_t n, bool fallback = true);
            // extern bool expandMonthly(CTimeArray& ta, const time_q& start, size_t n, bool fallback = true);
            // extern bool expandQuarterly(CTimeArray& ta, const time_q& start, size_t n, bool fallback = true);
            // extern bool expandAnnually(CTimeArray& ta, const time_q& start, size_t n, bool fallback = true);
        }
    }

    latest = lastBlock;
    return "";
}

//--------------------------------------------------------------------------------
void COptionsBlockList::Init(void) {
    numList.clear();
    hashList.clear();
    start = stop = 0;
    skip = 1;
    hashFind = NULL;
}

//--------------------------------------------------------------------------------
COptionsBlockList::COptionsBlockList(void) {
    Init();
}

//--------------------------------------------------------------------------------
bool COptionsBlockList::forEveryBlockNumber(UINT64VISITFUNC func, void* data) const {
    if (!func)
        return false;

    for (uint64_t i = start; i < stop; i = i + skip) {
        if (!(*func)(i, data))
            return false;
    }
    for (size_t i = 0; i < numList.size(); i++) {
        uint64_t n = numList[i];
        if (!(*func)(n, data))
            return false;
    }
    if (hashFind) {
        for (size_t i = 0; i < hashList.size(); i++) {
            uint64_t n = (*hashFind)(hashList[i], data);
            if (!(*func)(n, data))
                return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
bool COptionsBlockList::isInRange(blknum_t bn) const {
    if (start <= bn && bn < stop)
        return true;
    for (size_t i = 0; i < numList.size(); i++)
        if (bn == numList[i])
            return true;
    return false;
}

}  // namespace qblocks
