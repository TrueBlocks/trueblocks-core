/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("-clear",   "Clear the database and re-download price data"),
    CParams("-freshen", "Freshen database even if not needed"),
    CParams("-period",  "Time increment for display. Default 120 mins or :t where t is in [5|15|30|120|240|1440]"),
    CParams("-when",    "Time of day to start the display. :h determines the hour to start"),
    CParams("-output",  "Output level (default 1). :2 exposes more data per record"),
    CParams("",         "Freshen and/or print price quotes for the Ethereum network.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
extern bool removePriceFile(const SFString& path, void *data);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {

        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-c" || arg == "--clear") {

            SFString path = configPath("prices/");
            forEveryFileInFolder(path, removePriceFile, NULL);
            cerr << "Price cache was cleared.\n";
            freshen = true;

        } else if (arg == "-f" || arg == "--freshen") {

            freshen = true;

        } else if (arg.Contains("-w")) {

            SFString orig = arg;
            SFString arg1 = nextTokenClear(arg, ':');
            if (arg1 != "-w" && arg1 != "--when")
                return usage("Unknown parameter: " + orig);
            hour = toLong(arg);

        } else if (arg.Contains("-p")) {

            SFString orig = arg;
            SFString arg1 = nextTokenClear(arg, ':');
            if (arg1 != "-p" && arg1 != "--period")
                return usage("Unknown parameter: " + orig);

            freq = toLong(arg);
            if (freq % 5)
                return usage("Frequency must be a multiple of five minutes: " + orig);

        } else if (arg.Contains("-o")) {

            SFString orig = arg;
            SFString arg1 = nextTokenClear(arg, ':');
            if (arg1 != "-o" && arg1 != "--output")
                return usage("Unknown parameter: " + orig);

            dispLevel = toLong(arg);
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    freshen = false;
    dispLevel = 1;
    freq = 120;
    hour = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    CPriceQuote::registerClass();
    COptionsBase::useVerbose = true;
    COptionsBase::useTesting = true;
    Init();
    // header = "";
    footer = "  Powered, in part, by the Poloniex APIs\n";
    // seeAlso = "";
}

//--------------------------------------------------------------------------
bool removePriceFile(const SFString& path, void *data) {

    if (path.endsWith("/")) {
        forAllFiles(path + "*", removePriceFile, data);

    } else {
        if (path.startsWith(configPath("prices/")) && path.Contains(".bin"))
            removeFile(path);
    }
    return true;
}
