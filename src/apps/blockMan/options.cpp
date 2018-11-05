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
static COption params[] = {
    COption("~mode",              "one of 'list,' 'check,' or 'freshhen' (defaults to 'freshen')"),
    COption("-check:<num>",       "check that empty blocks are empty, and visa versa. Optionally start at :block"),
    COption("-list",              "list all non-empty block numbers"),
    COption("-refreshen",         "remove the full-block index and re-create it"),
    COption("-deep",              "when doing 'check', do a deep check"),
    COption("-emptyOnly",         "when doing 'check', skip non-empty blocks (i.e. check only blocks with no transactions)"),
    COption("-noEmpty",           "when doing 'check', skip empty blocks (i.e. check only blocks with transactions)"),
    COption("@skip",              "export once every 5,000 lines (applies only to --verbose --list mode - used for testing)"),
    COption("",                   "Indexes non-emtpy blocks (i.e. one or more transactions). Alternatively, "
                                  "lists non-empty blocks or checks for correctness.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();

    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-l" || arg == "--list") {
            if (!fileExists(fullBlockIndex) && !contains(mode, "freshen"))
                return usage("Could not open file '" + fullBlockIndex + "' for listing. Quitting...\n");
            mode += "list|";

        } else if (arg == "-s" || arg == "--skip") {
            skip = 5000;
            verbose = true;

        } else if (arg == "-n" || arg == "--noEmpty") {
            noEmpty = true;
            if (emptyOnly)
                return usage("Either --noEmpty or --emptyOnly may be true, not both.");

        } else if (arg == "-e" || arg == "--emptyOnly") {
            emptyOnly = true;
            if (noEmpty)
                return usage("Either --noEmpty or --emptyOnly may be true, not both.");

        } else if (contains(arg, "-c")) {
            string_q orig = arg;
            if (!fileExists(fullBlockIndex))
                return usage("Could not open file '" + fullBlockIndex + "' for checking. Quitting...\n");
            replace(arg, "--check", "");
            replace(arg, "-c", "");
            if (!arg.empty() && !startsWith(arg, ':'))
                return usage("Invalid option: " + orig);
            arg = extract(arg, 1);
            startBlock = str_2_Uint(arg);
            // add to end
            mode += "check|";

        } else if (arg == "-d" || arg == "--deep") {
            deepCheck = true;

        } else if (arg == "-r" || arg == "--refreshen") {
            if (fileExists(fullBlockIndex)) {
                cerr << "Are you sure you want to remove the full-block index? (y=yes)\n> ";
                int ch = getchar();
                if (ch == 'y' || ch == 'Y') {
                    remove(fullBlockIndex.c_str());
                } else {
                    cerr << "Not removed. Quitting.\n";
                    cerr.flush();
                    exit(0);
                }
            }
            // freshen first so it goes first
            mode = "freshen|" + mode;

        } else if (startsWith(arg, '-')) {
            if (arg != "-h" && !contains(arg, "-v"))
                return usage("Invalid option: " + arg);

        } else {
            mode += (arg + "|");
        }
    }

    bool hasCheck = contains(mode, "check");
    bool hasList  = contains(mode, "list");

    // fixup so things happen in a sensible order
    mode = "";
    mode += (hasCheck ? "check|"   : "");
    mode += (hasList  ? "list|"    : "");
    if (mode.empty())
        return usage("Mode must be one of --list, --check, or --refreshen.");

    if (deepCheck && !hasCheck)
        return usage("--deep only applies for mode 'check'");
    if (skip != 1 && (!verbose || mode != "list|"))
        return usage("Skip may only be used in --verbose --list mode");
    if ((noEmpty || emptyOnly) && !contains(mode, "check|"))
        return usage("--noEmpty and --emptyOnly are only available with --check mode.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    mode = "";
    deepCheck = false;
    if (fp)
        fclose(fp);
    fp = NULL;
    lastBlock = 0;
    skip = 1;
    startBlock = 0;
    noEmpty = false;
    emptyOnly = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    fp = NULL;
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
