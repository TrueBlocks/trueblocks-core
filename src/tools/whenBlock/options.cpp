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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

bool parseRequestDates(COptionsBase* opt, CNameValueArray& blocks, const string_q& arg);
//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("block_list", "", "list<string>", OPT_POSITIONAL, "one or more dates, block numbers, hashes, or special named blocks (see notes)"),  // NOLINT
    COption("list", "l", "", OPT_SWITCH, "export a list of the 'special' blocks"),
    COption("", "", "", OPT_DESCRIPTION, "Finds the nearest block prior to a date, or the nearest date prior to a block.\n    Alternatively, search for one of 'special' blocks."),  // NOLINT
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY_WHEN;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray block_list;
    // END_CODE_LOCAL_INIT

    string_q format = getGlobalConfig("whenBlock")->getConfigStr("display", "format", STR_DISPLAY_WHEN);
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseStringList2(this, block_list, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    blknum_t latest = getLastBlock_client();
    for (auto item : block_list) {
        if (isDate(item)) {
            if (!parseRequestDates(this, requests, item))
                return false;

        } else if (!parseBlockList2(this, blocks, item, latest)) {
            return false;

        } else {
            CNameValue spec;
            if (findSpecial(spec, item)) {
                if (spec.first == "latest")
                    spec.second = uint_2_Str(latest);
                requests.push_back(CNameValue("block", spec.second + "|" + spec.first));

            } else {
                blocks.Init();  // clear out blocks
                if (!parseBlockList2(this, blocks, item, latest))
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

    // Data verifictions
    if (requests.size() == 0)
        return usage("Please supply either a JSON formatted date or a blockNumber.");

    // Display formatting
    configureDisplay("whenBlock", "CBlock", STR_DISPLAY_WHEN);
    if (exportFmt == API1 || exportFmt == JSON1)
        manageFields("CBlock:" + string_q(STR_DISPLAY_WHEN));

    // Collect together results for later display
    applyFilter();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_DENOM);

    // BEG_CODE_INIT
    list = false;
    // END_CODE_INIT

    items.clear();
    requests.clear();
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));

    // Upgrade the configuration file by opening it, fixing the data, and then re-writing it (i.e. versions prior to
    // 0.6.0)
    CToml toml(configPath("whenBlock.toml"));
    if (toml.isBackLevel()) {
        string_q ss = toml.getConfigStr("specials", "list", "");
        if (!contains(ss, "kitties")) {
            toml.setConfigArray("specials", "list", STR_DEFAULT_WHENBLOCKS);
            toml.writeFile();
            getGlobalConfig("whenBlock");
        }
    }
    Init();

    // Differnt default for this software, but only change it if user hasn't already therefor not in Init
    if (!isApiMode())
        exportFmt = TXT1;

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.");
    notes.push_back("Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].");
    notes.push_back("You may customize the list of named blocks by editing ~/.quickBlocks/whenBlock.toml.");
    notes.push_back("The following `named` blocks are provided are currently configured:");
    // clang-format on
    // END_CODE_NOTES
    notes.push_back("  " + listSpecials(NONE1));

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
void COptions::applyFilter() {
    for (auto request : requests) {
        CBlock block;
        if (request.first == "block") {
            string_q bn = nextTokenClear(request.second, '|');
            if (request.second == "latest") {
                if (isTestMode()) {
                    continue;
                }
                queryBlock(block, "latest", false);
            } else {
                queryBlock(block, bn, false);
            }
            // TODO(tjayrush): this should be in the library so every request for zero block gets a valid blockNumber
            if (block.blockNumber == 0)
                block.timestamp = blockZeroTs;
            block.name = request.second;
            items[block.blockNumber] = block;

        } else if (request.first == "date") {
            if (lookupDate(this, block, (timestamp_t)str_2_Uint(request.second)))
                items[block.blockNumber] = block;
            else
                LOG_WARN("Could not find a block at date " + request.second);
        }
    }
}

//--------------------------------------------------------------------------------
bool showSpecials(CNameValue& pair, void* data) {
    reinterpret_cast<CNameValueArray*>(data)->push_back(CNameValue("block", pair.second + "|" + pair.first));
    return true;
}

//--------------------------------------------------------------------------------
string_q COptions::listSpecials(format_t fmt) const {
    if (specials.size() == 0)
        ((COptionsBase*)this)->loadSpecials();  // NOLINT

    ostringstream os;
    string_q extra;
    for (size_t i = 0; i < specials.size(); i++) {
        string_q name = specials[i].first;
        string_q bn = specials[i].second;
        if (name == "latest") {
            bn = uint_2_Str(getLastBlock_client());
            if (isTestMode()) {
                bn = "";
            } else if (COptionsBase::isReadme) {
                bn = "--";
            } else if (i > 0 && str_2_Uint(specials[i - 1].second) >= getLastBlock_client()) {
                extra = " (syncing)";
            }
        }

#define N_PER_LINE 4
        os << name;
        os << " (`" << bn << extra << "`)";
        if (!((i + 1) % N_PER_LINE)) {
            if (i < specials.size() - 1)
                os << "\n  ";
        } else if (i < specials.size() - 1) {
            os << ", ";
        }
    }
    return os.str().c_str();
}

//-----------------------------------------------------------------------
const char* STR_DISPLAY_WHEN =
    "[{BLOCKNUMBER}]\t"
    "[{TIMESTAMP}]\t"
    "[{DATE}]"
    "[\t{NAME}]";

//-----------------------------------------------------------------------
bool parseRequestDates(COptionsBase* opt, CNameValueArray& requests, const string_q& arg) {
    time_q date = str_2_Date(arg);
    if (date == earliestDate) {
        return opt->usage("Invalid date: '" + arg + "'.");

    } else if (date > Now()) {
        ostringstream os;
        os << "The date you specified (" << arg << ") "
           << "is in the future. No such block.";
        return opt->usage(os.str());

    } else if (date < time_q(2015, 7, 30, 15, 25, 00)) {
        ostringstream os;
        os << "The date you specified (" << arg << ") "
           << "is before the first block.";
        return opt->usage(os.str());
    }
    requests.push_back(CNameValue("date", int_2_Str(date_2_Ts(date))));
    return true;
}
