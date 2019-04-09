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
#include <string>
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~!block", "one or more block numbers (or a 'special' block), or..."),
    COption("~!date",  "one or more dates formatted as YYYY-MM-DD[THH[:MM[:SS]]]"),
    COption("-data",   "display the result as data (tab delimited; useful for scripting)"),
    COption("-list",   "list names and block numbers for special blocks"),
    COption("",        "Finds the nearest block prior to a date, or the nearest date prior to a block.\n"
                       " Alternatively, search for one of special 'named' blocks.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool containsAny(const string_q& haystack, const string_q& needle);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isList = false;
    bool foundOne = false;
    Init();
    blknum_t latestBlock = getLastBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (arg == "UTC") {
            // do nothing

        } else if (arg == "-l" || arg == "--list") {
            isList = true;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;
            colorsOff();

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: '" + orig + "'. Quitting...");
            }

        } else if (containsAny(arg, ":- ") && countOf(arg, '-') > 1) {

            ASSERT(!startsWith(arg, "-"));
            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            time_q date = str_2_Date(arg);
            if (date == earliestDate) {
                return usage("Invalid date: '" + orig + "'. Quitting...");

            } else if (date > Now()) {
                cout << "The date you specified (";
                cout << cTeal << orig << cOff;
                cout << ") is in the future. No such block. Quitting...\n";
                return false;

            } else if (date < time_q(2015, 7, 30, 15, 25, 00)) {
                // first block was at 15:26:00
                cout << "The date you specified (";
                cout << cTeal << orig << cOff;
                cout << ") is before the first block. Quitting...\n";
                return false;
            }

            foundOne = true;
            requests.push_back("date:" + int_2_Str(date_2_Ts(date)));

        } else {

            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            // if we're here, we better have a good block, assume we don't
            CNameValue spec;
            if (findSpecial(spec, arg)) {
                string_q val = spec.second;
                if (spec.first == "latest")
                    val = uint_2_Str(getLastBlock_client());
                requests.push_back("special:" + spec.first + "|" + val);
                foundOne = true;

            } else  {

                string_q ret = blocks.parseBlockList(arg, latestBlock);
                if (endsWith(ret, "\n")) {
                    cerr << "\n  " << ret << "\n";
                    return false;
                } else if (!ret.empty()) {
                    return usage(ret);
                }

                // Now we transfer the list of blocks to the requests array
                string_q blockList = getBlockNumList();  // get the list from blocks
                blocks.Init();  // clear out blocks
                CStringArray blks;
                explode(blks, blockList, '|');
                for (auto blk : blks) {
                    requests.push_back("block:" + blk);
                    foundOne = true;
                }
            }
        }
    }

    if (isList) {
        if (requests.size())
            return usage("The --list option must appear alone on the line. Quitting...");
        cout << listSpecials(false);
        return false;
    }

    if (!foundOne)
        return usage("Please supply either a JSON formatted date or a blockNumber. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    requests.clear();
    asData = false;
    optionOff(OPT_DENOM);
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    // Upgrade the configuration file by opening it, fixing the data, and then re-writing it
    CToml toml(configPath("whenBlock.toml"));

    // versions prior to 0.6.0
    if (toml.isBackLevel()) {
        string_q ss = toml.getConfigStr("specials", "list", "");
        if (!contains(ss, "kitties")) {
            toml.setConfigArray("specials", "list", STR_DEFAULT_WHENBLOCKS);
            toml.writeFile();
            getGlobalConfig("whenBlock");
        }
    }

    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return substitute(str, "block date", "< block | date > [ block... | date... ]");

    } else if (which == "notes") {
        string_q ret = str;
        if (verbose || COptions::isReadme) {
            ret += "Add custom special blocks by editing ~/.quickBlocks/whenBlock.toml.\n";
        }
        ret += listSpecials(true);
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
string_q COptions::listSpecials(bool terse) const {
    if (specials.size() == 0)
        ((COptionsBase *)this)->loadSpecials();  // NOLINT

    ostringstream os;
    if (!asData) {
        if (terse) {
            os << "Use the following names to represent `special` blocks:\n  ";
        } else {
            os << bYellow << "\n  Blocks:" << cOff;
        }
    }

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
        if (asData && !terse) {
            if (!contains(bn, "tbd"))
                os << bn << " ";
        } else {
            if (terse) {
                os << name;
                os << " (" << cTeal << bn << extra << cOff << ")";
                if (!((i+1) % N_PER_LINE)) {
                    if (i < specials.size()-1)
                        os << "\n  ";
                } else if (i < specials.size()-1) {
                    os << ", ";
                }
            } else {
                os << "\n      - " << padRight(name, 15);
                os << cTeal << padLeft(bn, 9) << cOff;
                if (verbose) {
                    CBlock block;
                    getBlock(block, bn);
                    block.timestamp = (block.blockNumber == 0 ? 1438269973 : block.timestamp);
                    if (block.timestamp != 0) {
                        UNHIDE_FIELD(CBlock, "date");
                        os << block.Format(" [{DATE}] ([{TIMESTAMP}])");
                    }
                }
                os << extra;
            }
        }
    }
    if (terse) {
        if (specials.size() % N_PER_LINE)
            os << "\n";
    } else {
        os << "\n";
    }
    return os.str().c_str();
}

//---------------------------------------------------------------------------------------
bool containsAny(const string_q& haystack, const string_q& needle) {
    string need = needle.c_str();
    for (const auto elem : need)
        if (contains(haystack, elem))
            return true;
    return false;
}

