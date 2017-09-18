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
    CParams("-testNum:[1|2]", "which test to run (either 1 and 2)"),
    CParams("",               "This program tests floating point printing and conversions.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg.startsWith("-t:") || arg.startsWith("--testNum:")) {
            SFString orig = arg;
            arg.ReplaceAny("--testNum:","");
            arg.ReplaceAny("-t:","");
            testNum = (int32_t)toLong(arg);
            if (!testNum || testNum > 2)
                return usage("Invalid argument: " + orig + ". Quitting...");

        } else {
            return usage("Invalid option: " + arg);

        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    testNum = -1;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
