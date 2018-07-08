/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
static COption params[] = {
    COption("~!block", "one or more block numbers (or a 'special' block), or..."),
    COption("~!date",  "one or more dates formatted as YYYY-MM-DD[THH[:MM[:SS]]]"),
    COption("-data",   "display the result as data (tab delimited; useful for scripting)"),
    COption("-list",   "list names and block numbers for special blocks"),
    COption("",        "Finds the nearest block prior to a date, or the nearest date prior to a block.\n"
                       " Alternatively, search for one of special 'named' blocks.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

extern SFTime grabDate(const string_q& strIn);
extern bool containsAny(const string_q& haystack, const string_q& needle);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isList = false;
    bool foundOne = false;
    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        string_q orig = arg;

        if (arg == "UTC") {
            // do nothing

        } else if (arg == "-l" || arg == "--list") {
            isList = true;

        } else if (arg == "-d" || arg == "--data") {

            alone = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: '" + orig + "'. Quitting...");
            }

        } else if (containsAny(arg, ":- ") && countOf(arg, '-') > 1) {

            ASSERT(!startsWith(arg, "-"));
            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            SFTime date = grabDate(arg);
            if (date == earliestDate) {
                return usage("Invalid date: '" + orig + "'. Quitting...");

            } else if (date > Now()) {
                cout << "The date you specified (";
                cout << cTeal << orig << cOff;
                cout << ") is in the future. No such block. Quitting...\n";
                return false;

            } else if (date < SFTime(2015, 7, 30, 15, 25, 00)) {
                // first block was at 15:26:00
                cout << "The date you specified (";
                cout << cTeal << orig << cOff;
                cout << ") is before the first block. Quitting...\n";
                return false;
            }

            foundOne = true;
            requests.push_back("date:" + asString(toTimestamp(date)));

        } else {

            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            // if we're here, we better have a good block, assume we don't
            CNameValue spec;
            if (findSpecial(spec, arg)) {
                string_q val = spec.second;
                if (spec.first == "latest")
                    val = asStringU(getLatestBlockFromClient());
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
                string_q blockList = getBlockNumList();
                blocks.Init();
                while (!blockList.empty()) {
                    requests.push_back("block:" + nextTokenClear(blockList, '|'));
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
    paramsPtr  = params;
    nParamsRef = nParams;
    pOptions = this;

    requests.clear();
    alone = false;
    optionOff(OPT_DENOM);
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

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
SFTime grabDate(const string_q& strIn) {

    if (strIn.empty()) {
        return earliestDate;
    }

// #error
    string_q str = strIn;
    replaceAny(str, " -:", ";");
    replace(str, ";UTC", "");
    str = nextTokenClear(str, '.');

    // Expects four number year, two number month and day at a minimum. Fields may be separated by '-' or ';'
    //    YYYYMMDD or YYYY;MM;DD
    replaceAll(str, ";", "");
    if (contains(str, "T")) {
        replace(str, "T", "");
               if (str.length() == 10) { str += "0000";
        } else if (str.length() == 12) { str += "00";
        } else if (str.length() != 14) { cerr << "Bad: " << str << "\n"; return earliestDate;
        }
    } else {
        str += "000000";
    }

#define NP ((uint32_t)-1)
#define toLong32u(a) (uint32_t)toLongU((a))
    uint32_t y, m, d, h, mn, s;
    y = m = d = h = mn = s = NP;
    if (isUnsigned(extract(str,  0, 4))) { y  = toLong32u(extract(str,  0, 4)); }
    if (isUnsigned(extract(str,  4, 2))) { m  = toLong32u(extract(str,  4, 2)); }
    if (isUnsigned(extract(str,  6, 2))) { d  = toLong32u(extract(str,  6, 2)); }
    if (isUnsigned(extract(str,  8, 2))) { h  = toLong32u(extract(str,  8, 2)); }
    if (isUnsigned(extract(str, 10, 2))) { mn = toLong32u(extract(str, 10, 2)); }
    if (isUnsigned(extract(str, 12, 2))) { s  = toLong32u(extract(str, 12, 2)); }

    // If any of them was not an unsigned int, it's a fail
    if (y == NP || m == NP || d == NP || h == NP || mn == NP || s == NP)
        return earliestDate;

    if (m > 12) return earliestDate;
    if (d > 31) return earliestDate;
    if (h > 23) return earliestDate;
    if (mn > 59) return earliestDate;
    if (s > 59) return earliestDate;

    return SFTime(y, m, d, h, mn, s);
}

//--------------------------------------------------------------------------------
string_q COptions::listSpecials(bool terse) const {
    if (specials.size() == 0)
        ((COptionsBase*)this)->loadSpecials();  // NOLINT

    ostringstream os;
    if (!alone) {
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
            bn = asStringU(getLatestBlockFromClient());
            if (isTestMode()) {
                bn = "";
            } else if (COptionsBase::isReadme) {
                bn = "--";
            } else if (i > 0 && toUnsigned(specials[i-1].second) >= getLatestBlockFromClient()) {
                extra = iWhite + " (syncing)" + cOff;
            }
        }

        if (alone && !terse) {
            if (!contains(bn, "tbd"))
                os << bn << " ";
        } else {
            if (terse) {
                os << name;
                os << " (" << cTeal << bn << extra << cOff << ")";
                if (!((i+1)%4)) {
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
        if (specials.size() % 4)
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

