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
    CParams("", "Finds the nearest block before the JSON-formatted date, or the nearest date before the given block.\n"),
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

            cout << cYellow << "\n\tSpecial blocks:";
            for (uint32_t i = 0 ; i < specials.getCount() - 1 ; i++) {
                SFString block = specials[i];
                SFString name = nextTokenClear(block, '|');
                cout << cOff << "\n\t  " << padRight(name, 12) << cTeal << padLeft(block, 10);
            }
            cout << cOff << "\n";
            return false;

        } else if (arg == "-a" || arg == "--alone") {

            alone = true;

        } else if (arg.ContainsAny(":-_ ") && !arg.startsWith("-")) {
            SFString str = arg.Substitute(" ", ";").Substitute("-", ";").Substitute("_", ";")
                                .Substitute(":", ";").Substitute("T", ";").Substitute(";UTC", "");
            date = snagDate(str);

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (arg == "latest") {
                special = arg;
                blockNum = getLatestBlockFromClient();

            } else {

                if (arg.length() && isdigit(arg[0])) {
                    blockNum = toLongU(arg);

                } else {
                    for (uint32_t i = 0 ; i < specials.getCount() ; i++) {
                        SFString n = specials[i];
                        SFString name = nextTokenClear(n, '|');
                        if (name == arg) {
                            special = arg;
                            blockNum = toLongU(n);
                        }
                    }
                    if (special.empty()) {
                        return usage("Argument " + arg + " is not valid. Supply either JSON formatted date or blockNumber. Quitting...");
                    }
                }
            }
        }
    }

    if (blockNum == NOPOS) {
        if (date == earliestDate)
            return usage("Please supply either a JSON formatted date or a blockNumber. Quitting...");

    } else {
        blknum_t latest = getLatestBlockFromClient();
        if (blockNum > latest) {
            if (special.empty()) {
                return usage("Block number (" + asStringU(blockNum) + ") must be less than latest "
                             "block: " + asStringU(latest));

            } else {
                // For the cases where user's node is behind the head of the block and getLatestBlockFromClient
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
    special = "";
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

extern int sortByBlockNum(const void *v1, const void *v2);
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
