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
    COption("by_date", "d", "enum[hour|day|week|month|year]", OPT_FLAG, "summarize by date"),
    COption("by_block", "b", "enum[1|10|100|1000|10000|100000|1000000]", OPT_FLAG, "summarize by block number"),
    COption("discrete", "i", "", OPT_SWITCH, "while accumulating, reset accumulator at each period"),
    COption("thing", "t", "<val>", OPT_HIDDEN | OPT_FLAG, "option two"),
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_DEPRECATED, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_DEPRECATED, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Process various data related to Ethereum's issuance.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    string_q by_date = "";
    string_q by_block = "";
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

        } else if (startsWith(arg, "-d:") || startsWith(arg, "--by_date:")) {
            if (!confirmEnum("by_date", by_date, arg))
                return false;

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--by_block:")) {
            if (!confirmEnum("by_block", by_block, arg))
                return false;

        } else if (arg == "-t" || arg == "--thing") {
            thing = true;

        } else if (arg == "-i" || arg == "--discrete") {
            discrete = true;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmUint("start", startBlock, arg))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmUint("end", endBlock, arg))
                return false;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (startBlock >= endBlock || endBlock > getLatestBlock_client())
        return usage("Invalid --start or --end.");

    if (thing) {
        check_uncles();
        return false;
    }

    if (!by_date.empty() && !by_block.empty())
        return usage("Please choose either --by_date or --by_block, not both.");

    if (!by_date.empty() || !by_block.empty()) {
        string_q by_what = by_date + by_block;
        map<string_q, period_t> perMap;
        perMap["hour"] = BY_HOUR;
        perMap["day"] = BY_DAY;
        perMap["week"] = BY_WEEK;
        perMap["month"] = BY_MONTH;
        perMap["year"] = BY_YEAR;
        perMap["1"] = BY_1;
        perMap["10"] = BY_10;
        perMap["100"] = BY_100;
        perMap["1000"] = BY_1000;
        perMap["10000"] = BY_10000;
        perMap["100000"] = BY_100000;
        perMap["1000000"] = BY_1000000;
        by_period = perMap[by_what];
    }

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
    optionOn(OPT_PREFUND);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");

    startBlock = 0;
    endBlock = getLatestBlock_client();
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
