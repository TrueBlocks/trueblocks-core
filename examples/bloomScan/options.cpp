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
    CParams( "~source",      "source for the data (either 'remote', 'pairty', 'binary')"),
    CParams( "~begin",       "block to start with"),
    CParams( "~end",         "block to end on"),
    CParams( "-mode:<mode>", "if not present, display in 'short' mode, otherwise 'full' mode"),
    CParams( "-data",        "show results as data instead of displaying bloom filters"),
    CParams( "~@skip",       "optional skip step (default 100)"),
    CParams( "",             "Scans blocks looking for saturated bloomFilters.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    SFString source;
    while (!command.empty()) {
        SFString arg = nextTokenClear(command,' ');
        SFString orig = arg;
        if (arg.startsWith("-m:") || arg.startsWith("--mode:")) {
            arg = arg.Substitute("-m:", "").Substitute("--mode:", "");
            if (arg != "short" && arg != "full")
                return usage("Mode must be either 'full' or 'short'. Quitting...");
            mode = arg;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!isUnsigned(arg)) {
                if (arg != "remote" && arg != "binary" && arg != "local")
                    return usage("Incorrect value (" + orig + ") for mode. Quitting.");
                etherlib_init(arg);
                source = arg;
            } else {
                if (start == NOPOS)
                    start = toUnsigned(arg);
                else if (stop == NOPOS)
                    stop = toUnsigned(arg);
                else if (skip == NOPOS)
                    skip = toUnsigned(arg);
                else
                    return usage("Too many parameters");
            }
        }
    }

    if (source.empty())
        return usage("You must provide a source of either 'local', 'remote', or 'binary'. Quitting...");
    if (start == NOPOS || stop == NOPOS)
        return usage("Please provide both a start block and an end block. Quitting...");
    if (start >= stop)
        return usage("The end block must be greater than the start block. Quitting...");
    if (skip == NOPOS)
        skip = 100;

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;
    start = NOPOS;
    stop  = NOPOS;
    skip  = NOPOS;
    mode  = "short";
    asData = false;
    optionOff(OPT_VERBOSE);
    minArgs = 2;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
