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
    CParams("-freshen",     "freshen only -- default (do not display transactions from the cache)"),
    CParams("-k",           "start at block :k"),
    CParams("-break",       "if debugging, run until (that is break at) this :block"),
    CParams("-cacheOnly",   "display transactions from the cache only (do not freshen)"),
    CParams("-debug",       "enter debug mode (pause after each transaction)"),
    CParams("-showCache",   "show transactions from the cache, and then freshen"),
    CParams("-logsDisp",    "display smart contract events (logs)"),
    CParams("-acctDisp",    "display accounting table of credits and debits"),
    CParams("-nocolor",     "display using plain text, no colors"),
    CParams("-rebuild",     "clear cache and reprocess all transcations (may take a long time)"),
    CParams("",             "Index transactions for a given Ethereum address (or series of addresses).\r\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-f" || arg == "--freshen") {
            // do nothing -- this is the default
            mode = "freshen|";  // last in wins

        } else if (arg.Contains("-b")) {
            SFString a = arg.Substitute("-b:","").Substitute("--break:","");
            bp = toLongU(a);
            if (!bp) {
                cerr << usageStr("You must specify a block number").Substitute("\n","\r\n");
                return false;
            }
            verbose |= 0x10;  // force debug

        } else if (arg.Contains("-k:")) {
            SFString a = arg.Substitute("-k:","");
            start = toLongU(a);
            if (!start) {
                cerr << usageStr("You must specify a block number").Substitute("\n","\r\n");
                return false;
            }

        } else if (arg == "-c" || arg == "--cacheOnly") {
            mode = "showCache|";  // last in wins

        } else if (arg == "-s" || arg == "--showCache") {
            mode = "showCache|freshen|";  // last in wins

        } else if (arg == "-a" || arg == "--acctDisp") {
            verbose |= 0x8;

        } else if (arg == "-l" || arg == "--logsDisp") {
            verbose |= 0x4;

        } else if (arg == "-d" || arg == "--debug") {
            verbose |= 0x10;

        } else if (arg == "-n" || arg == "--nocolor") {
            nocolor = true;

        } else if (arg == "-r" || arg == "--rebuild") {
            cerr << cRed << "Warning: " << cOff
            << "Rebuilding the cache may take a very long time. Are you sure? Type 'y' or 'yes' to continue...\r\n"
            << "  > ";
            cerr.flush();
            char buffer[256];
            cin >> buffer;
            SFString res = buffer;

            if (res == "y" || res == "yes") {
                CFilename fn("./cache/");
                SFString path = fn.getFullPath();
                removeFolder(path);
                establishFolder(path);
                cerr << cYellow << "The cache was cleared. Quitting...\r\n" << cOff;
            } else {
                cerr << cYellow << "The cache was not removed. Quitting...\r\n" << cOff;
            }
            return false;

        } else if (arg.startsWith('-')) {
            if (!arg.Contains("-h") && !arg.Contains("-v") && !arg.Contains("-t")) {
                cerr << usageStr("Invalid option: " + arg).Substitute("\n","\r\n");
                return false;
            }
       }
    }

    if (mode.empty())
        mode = "freshen|";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    CTrace::registerClass();
    CTraceAction::registerClass();
    CTraceResult::registerClass();

    mode = "freshen|";
    nocolor = false;
    bp = start = 0;

    useVerbose = true;
    useTesting = false;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    // header = "";
    // footer = "";
    // seeAlso = "";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
