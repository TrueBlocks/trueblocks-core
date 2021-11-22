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
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

extern bool parseRequestDates(COptionsBase* opt, CNameValueArray& blocks, const string_q& arg);
extern bool parseRequestTs(COptionsBase* opt, CNameValueArray& blocks, timestamp_t ts);
//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("blocks", "", "list<string>", OPT_REQUIRED | OPT_POSITIONAL, "one or more dates, block numbers, hashes, or special named blocks (see notes)"),  // NOLINT
    COption("list", "l", "", OPT_SWITCH, "export a list of the 'special' blocks"),
    COption("timestamps", "t", "", OPT_SWITCH, "ignore other options and generate timestamps only"),
    COption("check", "c", "", OPT_HIDDEN | OPT_SWITCH, "available only with --timestamps, checks the validity of the timestamp data"),  // NOLINT
    COption("fix", "f", "", OPT_HIDDEN | OPT_SWITCH, "available only with --timestamps, fixes incorrect timestamps if any"),  // NOLINT
    COption("count", "u", "", OPT_HIDDEN | OPT_SWITCH, "available only with --timestamps, returns the number of timestamps in the cache"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Find block(s) based on date, blockNum, timestamp, or 'special'."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY_WHEN;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& commandIn) {
    string_q command = commandIn;
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray blocks;
    // END_CODE_LOCAL_INIT

    getBlock_light(latest, "latest");
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-t" || arg == "--timestamps") {
            timestamps = true;

        } else if (arg == "-c" || arg == "--check") {
            check = true;

        } else if (arg == "-f" || arg == "--fix") {
            fix = true;

        } else if (arg == "-u" || arg == "--count") {
            count = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseStringList2(this, blocks, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (Mocked("when"))
        return false;

    for (auto item : blocks) {
        if (isDate(item)) {
            if (!parseRequestDates(this, requests, item))
                return false;

        } else if (isTimestamp(item)) {
            if (!parseRequestTs(this, requests, str_2_Ts(item)))
                return false;

        } else if (!parseBlockList2(this, CBlockOptions::blocks, item, latest.blockNumber)) {
            return false;

        } else {
            CNameValue spec;
            if (findSpecial(spec, item)) {
                if (spec.first == "latest")
                    spec.second = uint_2_Str(latest.blockNumber);
                requests.push_back(CNameValue("block", spec.second + "|" + spec.first));

            } else {
                CBlockOptions::blocks.Init();  // clear out blocks
                if (!parseBlockList2(this, CBlockOptions::blocks, item, latest.blockNumber))
                    return false;
                string_q blockList = getBlockNumList();  // get the list from blocks
                CStringArray blks;
                explode(blks, blockList, '|');
                for (auto blk : blks)
                    requests.push_back(CNameValue("block", blk));
            }
        }
    }

    if (list)
        forEverySpecialBlock(showSpecials, &requests);

    string_q format = getGlobalConfig("whenBlock")->getConfigStr("display", "format", STR_DISPLAY_WHEN);
    if (count)
        format = "[{nTimestamps}]";
    configureDisplay("whenBlock", "CBlock", format);
    manageFields("CBlock:" + string_q(format));
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, NOOPT, OPT_DENOM | OPT_CHECKLATEST);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    list = false;
    timestamps = false;
    check = false;
    fix = false;
    count = false;
    // END_CODE_INIT

    stop = 0;
    isText = false;
    cnt = 0;
    requests.clear();
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.");
    notes.push_back("Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    usageErrs[ERR_OPENINGTIMESTAMPS] = "Could not open timestamp file.";
    usageErrs[ERR_INVALIDDATE1] = "Please supply either a JSON formatted date or a blockNumber.";
    usageErrs[ERR_INVALIDDATE2] = "Invalid date '[{ARG}]'.";
    usageErrs[ERR_INVALIDDATE3] = "The date you specified ([{ARG}]) is before the first block.";
    usageErrs[ERR_ONLYTS] = "The --check, --fix, and --count options are only available with the --timestamps option.";
    // END_ERROR_STRINGS

    // Differnt default for this software, but only change it if user hasn't already therefor not in Init
    if (!isApiMode())
        expContext().exportFmt = TXT1;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool showSpecials(CNameValue& pair, void* data) {
    CNameValueArray* array = reinterpret_cast<CNameValueArray*>(data);
    if (pair.first == "latest")
        pair.second = uint_2_Str(getBlockProgress(BP_CLIENT).client);
    CNameValue nv("block", pair.second + "|" + pair.first);
    array->push_back(nv);
    return true;
}

//-----------------------------------------------------------------------
bool parseRequestTs(COptionsBase* opt, CNameValueArray& requests, timestamp_t ts) {
    requests.push_back(CNameValue("date", int_2_Str(ts)));
    return true;
}

//-----------------------------------------------------------------------
bool parseRequestDates(COptionsBase* opt, CNameValueArray& requests, const string_q& arg) {
    time_q date = str_2_Date(arg);
    if (date == earliestDate) {
        return opt->usage(substitute(opt->usageErrs[ERR_INVALIDDATE2], "[{ARG}]", arg));

    } else if (date < time_q(2015, 7, 30, 15, 25, 00)) {
        return opt->usage(substitute(opt->usageErrs[ERR_INVALIDDATE3], "[{ARG}]", arg));
    }

    requests.push_back(CNameValue("date", int_2_Str(date_2_Ts(date))));
    return true;
}

//-----------------------------------------------------------------------
const char* STR_DISPLAY_WHEN =
    "[{BLOCKNUMBER}]\t"
    "[{TIMESTAMP}]\t"
    "[{DATE}]"
    "[\t{NAME}]";
