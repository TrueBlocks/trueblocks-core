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
    CParams("-at",      "Report the price since nearest five minutes to :timestamp"),
    CParams("-clear",   "Clear the database and re-download price data"),
    CParams("-freshen", "Freshen database even if not needed"),
    CParams("-period",  "Time increment for display. Default 120 mins or :t where t is in [5|15|30|120|240|1440]"),
    CParams("-when",    "Time of day to start the display. :h determines the hour to start"),
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

        } else if (arg.startsWith("-a:") || arg.startsWith("--at:")) {
            SFString orig = arg;
            arg.ReplaceAny("-at:","");
            at = toLong(arg);
            if (arg.empty() || at == 0)
                return usage("Unknown parameter: " + orig);

        } else if (arg.startsWith("-w:") || arg.startsWith("--when:")) {
            SFString orig = arg;
            arg.ReplaceAny("-when:","");
            hour = toLong(arg);
            if (arg.empty() || hour > 23)
                return usage("Unknown parameter: " + orig);

        } else if (arg.startsWith("-p:") || arg.startsWith("--period:")) {
            SFString orig = arg;
            arg.ReplaceAny("-period:","");
            freq = toLong(arg);
            if (freq % 5)
                return usage("Frequency must be a multiple of five minutes: " + orig);
            if (arg.empty() || freq == 0)
                return usage("Unknown parameter: " + orig);

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            return usage("Invalid option: '" + arg + "'. Quiting...");
        }
    }
    if (verbose)
        dispLevel = verbose+1;

    SFString fileName = configPath("prices/poloniex.bin");
    if (!fileExists(fileName) && !freshen)
        return usage("The file " + fileName + " does not exist. Please run with the --freshen option. Quitting.");
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
    at = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    CPriceQuote::registerClass();
    COptionsBase::useVerbose = true;
    COptionsBase::useTesting = true;
    // At least one of the options must be used
    COptionsBase::needsOption = true;
    Init();
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
