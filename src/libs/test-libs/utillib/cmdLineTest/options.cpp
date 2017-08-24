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
    CParams("~testNum",         "the number of the test to run"),
    CParams("-bool:<bool>",     "enter a boolean value (either `0`, `1`, `false`, or `true`)"),
    CParams("-int:<int>",       "enter any numeric value"),
    CParams("-uint:<uint>",     "enter any any numeric value greater than or equal to zero"),
    CParams("-string:<string>", "enter any value"),
    CParams("-range:<range>",   "enter a range of numeric values"),
    CParams("-list:<list>",     "enter a list of value separated by commas (no spaces or quoted)"),
    CParams("",                 "Tests various command line behaviour.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg.startsWith("-b:") || arg.startsWith("--bool:")) {
            arg = arg.Substitute("-b:","").Substitute("--bool:","");
            if (arg == "1" || arg == "true") {
                boolOption = true;
                boolSet = true;
            } else if (arg == "0" || arg == "false") {
                boolOption = false;
                boolSet = true;
            } else
                usage("Invalid bool: " + orig);

        } else if (arg.startsWith("-i:") || arg.startsWith("--int:")) {
            arg = arg.Substitute("-i:","").Substitute("--int:","");
            if (arg.empty() || (arg[0] != '-' && arg[0] != '+' && !isdigit(arg[0])))
                return usage("--int requires a number. Quitting");
            numOption = toLong(arg);
            
        } else if (arg.startsWith("-u:") || arg.startsWith("--uint:")) {
            arg = arg.Substitute("-u:","").Substitute("--uint:","");
            if (arg.empty() || (arg[0] != '+' && !isdigit(arg[0]))) {
                //return usage("--uint requires a non-negative number. Quitting");
            } else {
                numOption = toLong(arg);
            }
            
        } else if (arg.startsWith('-')) {  // do not collapse
            
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            testNum = (int32_t)toLong(arg);
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    boolOption = false;
    boolSet = false;
    numOption = -1;
    stringOption = "";
    testNum = -1;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
