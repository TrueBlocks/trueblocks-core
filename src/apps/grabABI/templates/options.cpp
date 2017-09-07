/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "processing.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("-freshen",     "mode: freshen only -- default (do not display transactions from the cache)"),
    CParams("-showCache",   "mode: show transactions from the cache, and then freshen"),
    CParams("-cacheOnly",   "mode: display transactions from the cache only (do not freshen)"),
    CParams("-kBlock",      "start processing at block :k"),
    CParams("-parse",       "display parsed input data"),
    CParams("-lo(g)s",      "display smart contract lo(g)s or events"),
    CParams("-trace",       "display smart contract internal traces"),
    CParams("-bloom",       "display bloom filter matching"),
    CParams("-accounting",  "display credits and debits per account and reconcile at each block"),
    CParams("-list",        "display list of monitored accounts"),
    CParams("-debug",       "enter debug mode (pause after each transaction)"),
    CParams("-single",      "if debugging is enable, single step through transactions"),
    CParams("-rebuild",     "clear cache and reprocess all transcations (may take a long time)"),
    CParams("",             "Index transactions for a given Ethereum address (or series of addresses).\r\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    bool hasOnly = false;
    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-f" || arg == "--freshen") {
            // do nothing -- this is the default
            mode = "freshen|";  // last in wins

        } else if (arg.Contains("-k:") || arg.Contains("-kBlock:")) {

            arg = arg.Substitute("-k:","").Substitute("-kBlock:","");
            kBlock = toLongU(arg);
            if (!kBlock) {
                cerr << usageStr("You must specify a block number").Substitute("\n","\r\n");
                return false;
            }

        } else if (arg == "-c" || arg == "--cacheOnly") {
            mode = "showCache|";  // last in wins
            hasOnly = true;

        } else if (arg == "-s" || arg == "--showCache") {
            if (!hasOnly) // don't add freshen if we've already been told not to
                mode = "showCache|freshen|";  // last in wins

        } else if (arg == "--single" || arg == "--single_step") {
            single_on = true;

        } else if (arg == "-a" || arg == "--accounting") {
            accounting_on = true;

        } else if (arg == "-g" || arg == "--logs") {
            logs_on = true;

        } else if (arg == "-l" || arg == "--list") {
            CVisitor visitor;
            CToml toml("./config.toml");
            visitor.loadWatches(toml);
            cout << "[";
            for (uint32_t i=0;i<visitor.watches.getCount()-1;i++) {
                cout << " { ";
                cout << "\"address\": \""  << visitor.watches[i].color << visitor.watches[i].address    << cOff << "\", ";
                cout << "\"firstBlock\": " << bRed                     << visitor.watches[i].firstBlock << cOff << ", ";
                cout << "\"name\": \""     << visitor.watches[i].color << visitor.watches[i].name       << cOff << "\"";
                cout << " }" << (i<visitor.watches.getCount()-2 ? ",\r\n " : " ]\r\n");
            }
            exit(0);

        } else if (arg == "-t" || arg == "--trace") {
            trace_on = true;

        } else if (arg == "-p" || arg == "--parse") {
            parse_on = true;

        } else if (arg == "-b" || arg == "--bloom") {
            bloom_on = true;

        } else if (arg == "-d" || arg == "--debug") {
            debugger_on = true;

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

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (mode.empty())
        mode = "freshen|";

    if (debugger_on && !accounting_on)
        return usage("If you want to use the debugger, you must use the --accounting option as well.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    mode = "freshen|";
    single_on = false;
    accounting_on = false;
    logs_on = false;
    trace_on = false;
    bloom_on = false;
    debugger_on = false;
    parse_on = false;
    autocorrect_on = false;
    kBlock = 0;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
