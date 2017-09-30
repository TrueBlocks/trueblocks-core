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
    CParams("-list",         "list the names and block numbers of special blocks"),
    CParams("",              "Finds the nearest block prior to a JSON-formatted date, or the nearest date prior to\n"
                             " the given block. Alternatively, search for one of the special blocks listed below.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

extern int sortByBlockNum(const void *v1, const void *v2);
extern SFTime parseDate(const SFString& strIn);
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

        } else if (arg == "-a" || arg == "--alone") {

            alone = true;

        } else if (arg.ContainsAny(":- ") && !arg.startsWith("-")) {

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
            requests[requests.getCount()] = "date:" + asString(toTimestamp(date));

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: '" + orig + "'. Quitting...");
            }

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
                SFString blockList = blocks.toString();
                blocks.Init();
                while (!blockList.empty()) {
                    requests[requests.getCount()] = "block:" + nextTokenClear(blockList,'|');
                }
                foundOne = true;
            }
        }
    }

    if (isList) {
        if (alone || requests.getCount())
            return usage("The --list option must appear alone on the line. Quitting...");
        cout << listSpecials(false).Substitute("[{LATEST}]", asStringU(getLatestBlockFromClient()));
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
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "description")
        return str + listSpecials(true).Substitute("[{LATEST}]", asStringU(getLatestBlockFromClient()));
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

