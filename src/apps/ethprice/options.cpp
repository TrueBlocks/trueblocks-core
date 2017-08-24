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
    CParams("-at:<timestamp>",                 "Report the price since nearest five mintes to the given timestamp"),
    CParams("-freshen",                        "Freshen database (append new data)"),
    CParams("-period:<5|15|30|*120|240|1440>", "Display prices in this increment. One of [5|15|30|120*|240|1440]"),
    CParams("-when:<0-23>",                    "Hour on which to start display. Integer between 0-23"),
    CParams("",                                "Freshen and/or display Ethereum price data and other purposes.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {

        SFString arg  = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg.startsWith("-a:") || arg.startsWith("--at:")) {
            arg = orig.Substitute("-a:","").Substitute("--at:","");
            at = newTimestamp(arg);
            if (!isUnsigned(arg))
                return usage("Timestamp expected: " + orig);

        } else if (arg.startsWith("-w:") || arg.startsWith("--when:")) {
            arg = orig.Substitute("-w:","").Substitute("--when:","");
            hour = newUnsigned32(arg);
            if (!isUnsigned(arg) || hour > 23)
                return usage("Number between 0 and 23 expected: " + orig);

        } else if (arg.startsWith("-p:") || arg.startsWith("--period:")) {
            arg = orig.Substitute("-p:","").Substitute("--period:","");
            freq = newUnsigned32(arg);
            if (!isUnsigned(arg) || freq % 5)
                return usage("Positive multiple of 5 expected: " + orig);

        } else {
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    SFString fileName = configPath("prices/poloniex.bin");
    if (!fileExists(fileName)) {
        if (verbose)
            cerr << "First run, establishing price cache folder.\n";
        establishFolder(fileName);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    freshen = false;
    freq = 120;
    hour = 0;
    at = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    needsOption = true;
    Init();
}
