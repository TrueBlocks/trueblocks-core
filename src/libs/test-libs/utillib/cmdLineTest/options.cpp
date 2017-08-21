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
    CParams("-bool:<0,1,true,false>",    "either '0,' 'false,' '1,' or 'true'"),
    CParams("-int",     "any number positive or negative"),
    CParams("-uint",    "any number greater than or equal to zero"),
    CParams("-string",  "all other values"),
    CParams("-range",   "a range of positive numbers"),
    CParams("-list",    "a list of values"),
    CParams("",      "Tests various command line behaviour.\n"),
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
            if (arg == "1" || arg == "true")
                boolOption = true;
            else if (arg == "0" || arg == "false")
                boolOption = false;
            else
                usage("Invalid bool: " + orig);
                
        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    boolOption = false;
    numOption = -1;
    stringOption = "";

    useVerbose = true;
    useTesting = false;
//    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
