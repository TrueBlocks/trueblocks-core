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
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~!block", "one or more block numbers (or a 'special' block), or..."),
    CParams("~!date",  "one or more dates formatted as YYYY-MM-DD[THH[:MM[:SS]]]"),
    CParams("-data",   "display the result as data (tab delimited; useful for scripting)"),
    CParams("-list",   "list names and block numbers for special blocks"),
    CParams("",        "Finds the nearest block prior to a date, or the nearest date prior to a block.\n"
                       " Alternatively, search for one of special 'named' blocks.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

extern int sortByBlockNum(const void *v1, const void *v2);
extern SFTime grabDate(const SFString& strIn);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    bool isList = false;
    bool foundOne = false;
    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;

        if (arg == "UTC") {
            // do nothing

        } else if (arg == "-l" || arg == "--list") {
            isList = true;

        } else if (arg == "-d" || arg == "--data") {

            alone = true;

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: '" + orig + "'. Quitting...");
            }

        } else if (arg.ContainsAny(":- ") && countOf('-',arg) > 1) {

            ASSERT(!arg.startsWith("-"));
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

            } else if (date < SFTime(2015,07,30,15,25,00)) {
                // first block was at 15:26:00
                cout << "The date you specified (";
                cout << cTeal << orig << cOff;
                cout << ") is before the first block. Quitting...\n";
                return false;
            }

            foundOne = true;
            requests[requests.getCount()] = "date:" + asString(toTimestamp(date));

        } else {

            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            // if we're here, we better have a good block, assume we don't
            CNameValue spec;
            if (findSpecial(spec, arg)) {
                SFString val = spec.getValue();
                if (spec.getName() == "latest")
                    val = asStringU(getLatestBlockFromClient());
                requests[requests.getCount()] = "special:" + spec.getName() + "|" + val;
                foundOne = true;

            } else  {

                SFString ret = blocks.parseBlockList(arg, latestBlock);
                if (ret.endsWith("\n")) {
                    cerr << "\n  " << ret << "\n";
                    return false;
                } else if (!ret.empty()) {
                    return usage(ret);
                }
                SFString blockList = getBlockNumList();
                blocks.Init();
                while (!blockList.empty()) {
                    requests[requests.getCount()] = "block:" + nextTokenClear(blockList,'|');
                }
                foundOne = true;
            }
        }
    }

    if (isList) {
        if (requests.getCount())
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

    requests.Clear();
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
SFString COptions::postProcess(const SFString& which, const SFString& str) const {

    if (which == "options") {
        return str.Substitute("block date", "< block | date > [ block... | date... ]");

    } else if (which == "notes") {
        SFString ret = str;
        if (verbose || COptions::isReadme) {
            ret += "Add custom special blocks by editing ~/.quickBlocks/whenBlock.toml.\n";
        }
        ret += listSpecials(true);
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
SFTime grabDate(const SFString& strIn) {

    if (strIn.empty()) {
        return earliestDate;
    }

//#error
    SFString str = strIn;
    str.ReplaceAny(" -:",";");
    str.Replace(";UTC", "");
    str = nextTokenClear(str,'.');

    // Expects four number year, two number month and day at a minimum. Fields may be separated by '-' or ';'
    //    YYYYMMDD or YYYY;MM;DD
    str.ReplaceAll(";","");
    if (str.Contains("T")) {
        str.Replace("T","");
        if      (str.length() == 10) str += "0000";
        else if (str.length() == 12) str += "00";
        else if (str.length() != 14) { cerr << "Bad: " << str << "\n"; return earliestDate; }
    } else {
        str += "000000";
    }

#define NP ((uint32_t)-1)
    uint32_t y, m, d, h, mn, s;
    y = m = d = h = mn = s = NP;
    if (isUnsigned(str.substr( 0,4))) { y  = toLong32u(str.substr( 0, 4)); }
    if (isUnsigned(str.substr( 4,2))) { m  = toLong32u(str.substr( 4, 2)); }
    if (isUnsigned(str.substr( 6,2))) { d  = toLong32u(str.substr( 6, 2)); }
    if (isUnsigned(str.substr( 8,2))) { h  = toLong32u(str.substr( 8, 2)); }
    if (isUnsigned(str.substr(10,2))) { mn = toLong32u(str.substr(10, 2)); }
    if (isUnsigned(str.substr(12,2))) { s  = toLong32u(str.substr(12, 2)); }

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
SFString COptions::listSpecials(bool terse) const {
    if (specials.getCount() == 0)
        ((COptionsBase*)this)->loadSpecials();

    ostringstream os;
    if (!alone) {
        if (terse) {
            os << "Use the following names to represent `special` blocks:\n  ";
        } else {
            os << bYellow << "\n  Blocks:" << cOff;
        }
    }

    SFString extra;
    for (uint32_t i = 0 ; i < specials.getCount(); i++) {

        SFString name = specials[i].getName();
        SFString bn = specials[i].getValue();
        if (name == "latest") {
            bn = asStringU(getLatestBlockFromClient());
            if (isTestMode()) {
                bn = "";
            } else if (COptionsBase::isReadme) {
                bn = "--";
            } else if (i > 0 && specials[i-1].getValueU() >= getLatestBlockFromClient()) {
                extra = iWhite + " (syncing)" + cOff;
            }
        }

        if (alone && !terse) {
            if (!bn.Contains("tbd"))
                os << bn << " ";
        } else {
            if (terse) {
                os << name;
                os << " (" << cTeal << bn << extra << cOff << ")";
                if (!((i+1)%4)) {
                    if (i < specials.getCount()-1)
                        os << "\n  ";
                } else if (i < specials.getCount()-1)
                    os << ", ";
            } else {
                os << "\n      - " << padRight(name, 15);
                os << cTeal << padLeft(bn,9) << cOff;
                if (verbose) {
                    CBlock block;
                    getBlock(block, bn);
                    block.timestamp = (block.blockNumber == 0 ? 1438269973 : block.timestamp);
                    if (block.timestamp != 0) {
                        UNHIDE_FIELD(CBlock, "date");
                        os << block.Format(" [{DATE}] ([{TIMESTAMP}])");
                    }
                }
                os << extra ;
            }
        }
    }
    if (terse) {
        if (specials.getCount() % 4)
            os << "\n";
    } else {
        os << "\n";
    }
    return os.str().c_str();
}
