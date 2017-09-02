/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~date / block", "one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber"),
    CParams("-alone",        "show the found block or found date unadorned (useful for scripting)"),
    CParams("-list",         "list special blocks timestamps and dates"),
    CParams("", "Finds the nearest block prior to a JSON-formatted date, or the nearest date prior to\n"
            "the given block. Alternatively, search for one of the special blocks listed below.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

extern int sortByBlockNum(const void *v1, const void *v2);
extern SFTime parseDate(const SFString& strIn);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    bool isList = false;
    bool foundOne = false;
    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;

        if (arg == "UTC") {
            // do nothing

        } else if (arg == "-l" || arg == "--list") {
            isList = true;

        } else if (arg == "-a" || arg == "--alone") {

            alone = true;

        } else if (arg.ContainsAny(":-_ ") && !arg.startsWith("-")) {

            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            // If we're here, we better have a good date, assume we don't
            foundOne = false;
            SFString str = arg.Substitute(" ", ";").Substitute("-", ";").Substitute("_", ";")
                                .Substitute(":", ";").Substitute(";UTC", "");
            SFTime date = parseDate(str);
            if (date == earliestDate)
                return usage("Invalid date: '" + orig + "'. Quitting...");
            if (date < SFTime(2015,07,30,15,25,00)) { // first block was at 15:26:00
                cout << "The date you specified (";
                cout << cTeal << orig << cOff;
                cout << ") is before the first block. Quitting...\n";
                return false;
            }
            foundOne = true;
            requests[requests.getCount()] = "date:" + asString(toTimeStamp(date));

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: '" + orig + "'. Quitting...");
            }

        } else {

            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            // if we're here, we better have a good block, assume we don't
            foundOne = false;
            for (uint32_t i = 0 ; i < specials.getCount() ; i++) {
                SFString num = specials[i];
                SFString special = nextTokenClear(num, '|');
                if (special == arg) {
                    requests[requests.getCount()] = "special:" + special + "|" + num;
                    foundOne = true;
                }
            }

            if (!foundOne) {
                if (isUnsigned(arg)) {

                    if (toUnsigned(arg) > getLatestBlockFromClient()) {
                        cout << "The block number you requested (";
                        cout << cTeal << orig << cOff;
                        cout << ") is after the latest block (";
                        cout << cTeal << (isTestMode() ? "TESTING" : asStringU(getLatestBlockFromClient())) << cOff;
                        cout << "). Quitting...\n";
                        return false;
                    }
                    requests[requests.getCount()] = "block:" + asStringU(toUnsigned(arg));
                    foundOne = true;
                } else {
                    return usage("Invalid argument: '" + orig + "'. Please supply either a JSON formatted date or a blockNumber. Quitting...");
                }
            }
        }
    }

    if (isList) {
        if (alone || requests.getCount())
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

    specials.Clear();
    specials[specials.getCount()] = "first|0";
    specials[specials.getCount()] = "iceage|200000";
    specials[specials.getCount()] = "homestead|1150000";
    specials[specials.getCount()] = "daofund|1428756";
    specials[specials.getCount()] = "daohack|1718497";
    specials[specials.getCount()] = "daofork|1920000";
    specials[specials.getCount()] = "tangerine|2463000";
    specials[specials.getCount()] = "spurious|2675000";
    specials[specials.getCount()] = "stateclear|2718436";
    specials[specials.getCount()] = "latest|";

    SFString custom = asciiFileToString(configPath("whenBlock.toml"));
    while (!custom.empty()) {
        SFString line = nextTokenClear(custom, '\n');
        if (line == "[SPECIAL]") {
            line = nextTokenClear(custom, '\n');
            do {
                SFString block = line;
                SFString name = nextTokenClear(block, '=');
                if (toLongU(block))  // non zero means it's a number
                    specials[specials.getCount()] = Strip(name, ' ') + "|" + Strip(block, ' ');
                line = nextTokenClear(custom, '\n');
            } while (!custom.empty());
        }
    }
    specials.Sort(sortByBlockNum);
}

//--------------------------------------------------------------------------------
int sortByBlockNum(const void *v1, const void *v2) {
    SFString b1 = *(SFString *)v1;  // NOLINT
    SFString b2 = *(SFString *)v2;  // NOLINT
    if (b1 == "latest|")
        return 1;
    if (b2 == "latest|")
        return -1;
    nextTokenClear(b1, '|');
    nextTokenClear(b2, '|');
    return (int)(toLong32u(b1) - toLong32u(b2));
}

//--------------------------------------------------------------------------------
SFStringArray specials;

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString COptions::listSpecials(bool terse) const {
    ostringstream os;
    if (terse) {
        os << bYellow << "\n  Notes:\n\t" << cOff;
        os << "You may specify any of the following strings to represent 'special' blocks:\n\n\t    ";
        for (uint32_t i = 0 ; i < specials.getCount(); i++) {
            SFString block = specials[i];
            SFString name = nextTokenClear(block, '|');
            os << name;
            if (name != "latest")
                os << " (" << cTeal << block << cOff << ")";
            if (i < specials.getCount()-1)
                os << ", ";
            if (!((i+1)%5))
                os << "\n\t    ";
        }
        os << "\n\tSee the README for information on customizing this list.\n";
    } else {
        os << bYellow << "\n\tSpecial Blocks:" << cOff;
        for (uint32_t i = 0 ; i < specials.getCount(); i++) {
            SFString block = specials[i];
            SFString name = nextTokenClear(block, '|');
            os << cOff << "\n\t  " << padRight(name, 12) << cTeal << padLeft(block, 10);
        }
        os << "\n";
    }
    return os.str().c_str();
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "description")
        return str + listSpecials(true);
    return str;
}

//--------------------------------------------------------------------------------
SFTime parseDate(const SFString& strIn) {

    if (strIn.empty()) {
        return earliestDate;
    }

    SFString str = strIn.Substitute(";", "");
    if (str.length() != 14 && str.length() != 8)
        return earliestDate;

    if (str.length() == 8)
        str += "120000";

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

    return SFTime(y, m, d, h, mn, s);
}
