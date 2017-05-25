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
    CParams("~date/block", "one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber"),
    CParams("-list",       "list special blocks timestamps and dates"),
    CParams("", "Finds the nearest block before the JSON-formatted date, or the date of the given block.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        arg.ReplaceAll("{~}", " ");
        arg.ReplaceAll("T", " ");

        if (arg == "-l" || arg == "--list") {

            cout << "\n\tSpecial block: " << cYellow << arg << " " << cOff << "\n\n";
            return false;

        } else if (arg.ContainsAny(":- ")) {
            SFString str = arg.Substitute("-", ";").Substitute(" ", ";")
                                .Substitute(":", ";").Substitute(";UTC", "");
            date = snagDate(str);

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (arg == "latest") {
                special = arg;
                blockNum = getClientLatestBlk();

            } else {

                for (int i = 0 ; i < specials.getCount() ; i++) {
                    SFString n = specials[i];
                    SFString name = nextTokenClear(n, '|');
                    if (name == arg) {
                        special = arg;
                        blockNum = toLongU(n);
                    }
                }
                if (special.empty())
                    blockNum = toLongU(arg);
            }
        }
    }

    if (blockNum == NOPOS) {
        if (date == earliestDate)
            return usage("Please supply either a JSON formatted date or a blockNumber.");

    } else {
        blknum_t l = getClientLatestBlk();
        if (blockNum > l) {
            if (special.empty()) {
                return usage("Block number (" + asString(blockNum) + ") must be less than latest "
                             "block: " + asString(l));

            } else {
                // For the cases where user's node is behind the head of the block and getClientLatestBlk
                // returns a block number in the past, the only thing we can present is known block dates
                cout << "\n\tSpecial block: " << cYellow << special << " " << blockNum << cOff << "\n\n";
                return false;
            }
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;

    blockNum = NOPOS;
    date = earliestDate;
//  special = "";

    useVerbose = false;
    useTesting = false;

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
}

//--------------------------------------------------------------------------------
SFStringArray specials;

//--------------------------------------------------------------------------------
const char *STR_README_HEADER =
"The `whenBlock` tool answers one of two questions: (1) \"When did a given block occur?\" or "
"(2) \"What block occurred at or before a given date and time?\"\n"
"\n"
"In the first case, one simply supplies a block number and the date and time of that block is "
"displayed. In the later case, one provides a date (and optionally time) and the block number "
"that occurred at or just prior to that date and time is displayed.\n"
"\n"
"The `hour`/`minute`/`second` portions of the date are optional, and if omitted, default to zero in each case.\n";

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    header = STR_README_HEADER;
    footer = "  Special values: [ ";
    for (int i = 0 ; i < specials.getCount() ; i++) {
        SFString item = specials[i];
        footer += (nextTokenClear(item, '|') + " | ");
    }
    footer.ReplaceReverse("|", "");
    footer.ReplaceReverse(" ", "");
    footer += "]\n\n";
    // seeAlso = "";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
