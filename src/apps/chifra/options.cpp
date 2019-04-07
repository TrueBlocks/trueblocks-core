/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~command", "one of [ freshen | export | list | init | seed | daemon | scrape | ls | rm | names | config ]"),
    COption("",         "Create a TrueBlocks monitor configuration.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool visitIndexFiles(const string_q& path, void *data);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {

        if (mode.empty() && startsWith(arg, '-')) {

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (contains(params[0].description, " " + arg + " ")) {
                if (!mode.empty())
                    return usage("Please specify " + params[0].description + ". Quitting...");
                mode = arg;

            } else if (isAddress(arg)) {
                addrs.push_back(toLower(arg));

            } else {
                if (arg == "--new_version")
                    freshen_flags = (arg + " ");
                else
                    tool_flags += (arg + " ");

            }
        }
    }

    if (mode.empty())
        return usage("Please specify " + params[0].description + ". Quitting...");
    monitorsPath = getMonitorPath("");
    establishFolder(monitorsPath);
    tool_flags = trim(tool_flags, ' ');
    freshen_flags = trim(freshen_flags, ' ');

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    addrs.clear();
    tool_flags    = "";
    freshen_flags = "";
    mode          = "";
    minArgs       = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : txCache(WRITING_ARCHIVE) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
