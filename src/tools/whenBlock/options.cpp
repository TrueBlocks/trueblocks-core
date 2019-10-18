/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
// BEG_CODE_OPTIONS
    COption("block_list", "", "list<blknum>", OPT_POSITIONAL, "one or more block numbers (or a 'special' block), or"),
    COption("date_list", "", "list<date>", OPT_POSITIONAL, "one or more dates formatted as YYYY-MM-DD[THH[:MM[:SS]]]"),
    COption("list", "l", "", OPT_SWITCH, "export all the named blocks"),
    COption("fmt", "x", "enum[none|json*|txt|csv|api]", OPT_HIDDEN | OPT_FLAG, "export format (one of [none|json*|txt|csv|api])"),
    COption("", "", "", OPT_DESCRIPTION, "Finds the nearest block prior to a date, or the nearest date prior to a block.\n    Alternatively, search for one of special 'named' blocks."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY_WHEN;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

// BEG_CODE_LOCAL_INIT
// END_CODE_LOCAL_INIT

    bool no_header = false;
    string_q format = getGlobalConfig("whenBlock")->getConfigStr("display", "format", STR_DISPLAY_WHEN);
    Init();
    blknum_t latestBlock = getLastBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
// END_CODE_AUTO

        } else if (arg == "UTC") {
            // do nothing

        } else if (arg == "-l" || arg == "--list") {
            forEverySpecialBlock(showSpecials, &requests);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg))
                return usage("Invalid option: '" + orig + "'.");

        } else if (containsAny(arg, ":- ") && countOf(arg, '-') > 1) {

            // a json formatted date
            ASSERT(!startsWith(arg, "-"));
            time_q date = str_2_Date(arg);
            if (date == earliestDate) {
                return usage("Invalid date: '" + orig + "'.");

            } else if (date > Now()) {
                ostringstream os;
                if (isApiMode())
                    colorsOff();
                os << "The date you specified (" << cTeal << orig << cOff << ") " << "is in the future. No such block.";
                if (!isApiMode()) {
                    LOG_WARN(os.str());
                    return false;
                }
                return usage(os.str());

            } else if (date < time_q(2015, 7, 30, 15, 25, 00)) {
                ostringstream os;
                if (isApiMode())
                    colorsOff();
                os << "The date you specified (" << cTeal << orig << cOff << ") " << "is before the first block.";
                if (!isApiMode()) {
                    LOG_WARN(os.str());
                    return false;
                }
                return usage(os.str());

            } else {
                requests.push_back(CNameValue("date", int_2_Str(date_2_Ts(date))));
            }

        } else {

            // if we're here, we better have a good block, assume we don't
            CNameValue spec;
            if (findSpecial(spec, arg)) {
                if (spec.first == "latest")
                    spec.second = uint_2_Str(latestBlock);
                requests.push_back(CNameValue("block", spec.second + "|" + spec.first));

            } else  {

                string_q ret = blocks.parseBlockList(arg, latestBlock);
                if (endsWith(ret, "\n")) {
                    LOG_WARN(substitute(ret,"\n",""));
                    return false;

                } else if (!ret.empty()) {
                    return usage(ret);
                }

                // Now we transfer the list of blocks to the requests array
                string_q blockList = getBlockNumList();  // get the list from blocks
                blocks.Init();  // clear out blocks
                CStringArray blks;
                explode(blks, blockList, '|');
                for (auto blk : blks)
                    requests.push_back(CNameValue("block", blk));
            }
        }
    }

    // Data verifictions
    if (requests.size() == 0)
        return usage("Please supply either a JSON formatted date or a blockNumber.");

    // Display formatting
    switch (exportFmt) {
        case NONE1: format = STR_DISPLAY_WHEN; break;
        case API1:
        case JSON1: format = ""; break;
        case TXT1:
        case CSV1:
            format = getGlobalConfig("whenBlock")->getConfigStr("display", "format", format.empty() ? STR_DISPLAY_WHEN : format);
            break;
    }
    manageFields("CBlock:" + cleanFmt((format.empty() ? STR_DISPLAY_WHEN : format), exportFmt));
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (no_header)
        expContext().fmtMap["header"] = "";

    // collect together results for later display
    applyFilter();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_DENOM);

// BEG_CODE_INIT
// END_CODE_INIT

    items.clear();
    requests.clear();
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    // Upgrade the configuration file by opening it, fixing the data, and then re-writing it (i.e. versions prior to 0.6.0)
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
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "block_list date_list", "< block | date > [ block... | date... ]");

    } else if (which == "notes") {
        string_q ret = str;
        if (verbose || COptions::isReadme) {
            ret += "Add custom special blocks by editing ~/.quickBlocks/whenBlock.toml.\n";
        }
        ret += "Use the following names to represent `special` blocks:\n  ";
        ret += listSpecials(NONE1);
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
void COptions::applyFilter() {
    for (auto request : requests) {
        CBlock block;
        if (request.first == "block") {
            string_q bn = nextTokenClear(request.second,'|');
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
bool showSpecials(CNameValue& pair, void *data) {
    ((CNameValueArray*)data)->push_back(CNameValue("block", pair.second + "|" + pair.first));
    return true;
}

//--------------------------------------------------------------------------------
string_q COptions::listSpecials(format_t fmt) const {
    if (specials.size() == 0)
        ((COptionsBase *)this)->loadSpecials();  // NOLINT

    ostringstream os;
    string_q extra;
    for (size_t i = 0 ; i < specials.size(); i++) {

        string_q name = specials[i].first;
        string_q bn = specials[i].second;
        if (name == "latest") {
            bn = uint_2_Str(getLastBlock_client());
            if (isTestMode()) {
                bn = "";
            } else if (COptionsBase::isReadme) {
                bn = "--";
            } else if (i > 0 && str_2_Uint(specials[i-1].second) >= getLastBlock_client()) {
                extra = iWhite + " (syncing)" + cOff;
            }
        }

#define N_PER_LINE 4
        os << name;
        os << " (" << cTeal << bn << extra << cOff << ")";
        if (!((i+1) % N_PER_LINE)) {
            if (i < specials.size()-1)
                os << "\n  ";
        } else if (i < specials.size()-1) {
            os << ", ";
        }
    }
    if (specials.size() % N_PER_LINE)
        os << "\n";
    return os.str().c_str();
}

//-----------------------------------------------------------------------
const char* STR_DISPLAY_WHEN =
"[{BLOCKNUMBER}]\t"
"[{TIMESTAMP}]\t"
"[{DATE}]"
"[\t{NAME}]";
