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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("generate", "g", "", OPT_SWITCH, "generate expercted miner and uncle rewards for each block"),
    COption("audit", "a", "", OPT_SWITCH, "audit miner and uncle rewards for each block"),
    COption("uncles", "u", "", OPT_SWITCH, "generate a list of blocks containing one or more uncles"),
    COption("by_year", "y", "", OPT_SWITCH, "summarize previously generated results by year"),
    COption("by_month", "m", "", OPT_SWITCH, "summarize previously generated results by month"),
    COption("by_week", "w", "", OPT_SWITCH, "summarize previously generated results by week"),
    COption("by_day", "d", "", OPT_SWITCH, "summarize previously generated results by day"),
    COption("by_hour", "o", "", OPT_SWITCH, "summarize previously generated results by hour"),
    COption("discrete", "i", "", OPT_SWITCH, "while accumulating, reset accumulator at each period"),
    //COption("option1", "o", "", OPT_HIDDEN|OPT_SWITCH, "option one"),
    COption("thing", "t", "<val>", OPT_HIDDEN|OPT_FLAG, "option two"),
    COption("", "", "", OPT_DESCRIPTION, "Process various data related to Ethereum's issuance.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    bool by_year = false;
    bool by_month = false;
    bool by_week = false;
    bool by_day = false;
    bool by_hour = false;
    bool thing = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-g" || arg == "--generate") {
            generate = true;

        } else if (arg == "-a" || arg == "--audit") {
            audit = true;

        } else if (arg == "-u" || arg == "--uncles") {
            uncles = true;

        } else if (arg == "-y" || arg == "--by_year") {
            by_year = true;

        } else if (arg == "-m" || arg == "--by_month") {
            by_month = true;

        } else if (arg == "-w" || arg == "--by_week") {
            by_week = true;

        } else if (arg == "-d" || arg == "--by_day") {
            by_day = true;

        } else if (arg == "-o" || arg == "--by_hour") {
            by_hour = true;

        } else if (arg == "-t" || arg == "--thing") {
            thing = true;

        } else if (arg == "-i" || arg == "--discrete") {
            discrete = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (thing) {
        check_uncles();
        return false;
    }

    if (by_year)
        by_period = BY_YEAR;
    else if (by_month)
        by_period = BY_MONTH;
    else if (by_week)
        by_period = BY_WEEK;
    else if (by_day)
        by_period = BY_DAY;
    else if (by_hour)
        by_period = BY_HOUR;
    else
        by_period = BY_NOTHING;

    generate = generate || by_period;

    if (!generate && !audit && !uncles)
        return usage("Please choose one of the available options.");

    if ((generate + audit + uncles) != 1)
        return usage("Please choose only one of the available options.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------
biguint_t operator-(const biguint_t& a, const bigint_t& b) {
    ASSERT(b < a); // Can't make a negative unsigned big int
    bigint_t aa = str_2_BigInt(bnu_2_Str(a));
    return str_2_BigUint(bni_2_Str(aa - b));
}

//--------------------------------------------------------------
biguint_t operator+(const biguint_t& a, const bigint_t& b) {
    bigint_t aa = str_2_BigInt(bnu_2_Str(a));
    return str_2_BigUint(bni_2_Str(aa + b));
}
