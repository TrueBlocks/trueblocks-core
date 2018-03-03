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
    CParams("-kBlock:<num>", "start processing at block :k"),
    CParams("-offset:<num>", "offset to kBlock"),
    CParams("-parse",        "display parsed input data"),
    CParams("-lo(g)s",       "display smart contract lo(g)s or events"),
    CParams("-trace",        "display smart contract internal traces"),
    CParams("-accounting",   "display credits and debits per account and reconcile at each block"),
    CParams("-list",         "display list of monitored accounts"),
    CParams("-debug",        "enter debug mode (pause after each transaction)"),
    CParams("-single",       "if debugging is enable, single step through transactions"),
    CParams("",              "Index transactions for a given Ethereum address (or series of addresses).\r\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg.Contains("-k:") || arg.Contains("--kBlock:")) {

            arg = arg.Substitute("-k:","").Substitute("--kBlock:","");
            if (!isNumeral(arg)) {
                cerr << usageStr("You must specify a block number (" + arg + ")").Substitute("\n","\r\n");
                return false;
            }
            kBlock = toLongU(arg);

        } else if (arg.Contains("-o:") || arg.Contains("--offset:")) {

            arg = arg.Substitute("-o:","").Substitute("--offset:","");
            if (!isNumeral(arg)) {
                cerr << usageStr("You must specify a number for offset (" + arg + ")").Substitute("\n","\r\n");
                return false;
            }
            offset = toLongU(arg);

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

        } else if (arg == "-d" || arg == "--debug") {
            debugger_on = true;

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (debugger_on && !accounting_on)
        return usage("If you want to use the debugger, you must use the --accounting option as well.");

    kBlock = max((blknum_t)0, kBlock-offset);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    single_on = false;
    accounting_on = false;
    logs_on = false;
    trace_on = false;
    debugger_on = false;
    parse_on = false;
    autocorrect_on = false;
    kBlock = 0;
    offset = 0;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
