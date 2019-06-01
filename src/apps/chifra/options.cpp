/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~command", "one of [ "
                            "leech | scrape | daemon | "
                            "list | export | stats | ls | rm | "
                            "accounts | config | slurp | quotes | data | "
                            "blocks | trans | receipts | logs | traces "
                            "]"),
    COption("",         "Create a TrueBlocks monitor configuration.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool visitIndexFiles(const string_q& path, void *data);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    ENTER4("parseArguments");

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "--tool_help" || (api_mode && arg == "--help")) {
            tool_flags += (" --help");

        } else if (mode.empty() && startsWith(arg, '-')) {

            if (!builtInCmd(arg))
                EXIT_USAGE("Invalid option: " + arg);

        } else {

            if (contains(params[0].description, " " + arg + " ")) {
                if (!mode.empty())
                    EXIT_USAGE("Please specify " + params[0].description + ". " + mode + ":" + arg);
                mode = arg;
                if (mode == "stats") {
                    mode = "ls";
                    stats = true;
                }

            } else if (isAddress(arg)) {
                addrs.push_back(toLower(arg));

            } else {
                if (arg == "--useBlooms")
                    freshen_flags = (arg + " ");
                else
                    tool_flags += (arg + " ");

            }
        }
    }

    if (mode == "blocks" ||
    	mode == "trans" ||
    	mode == "receipts" ||
    	mode == "logs" ||
    	mode == "traces") {
        tool_flags += (" --" + mode);
        mode = "data";
    }

    if (mode.empty())
        EXIT_USAGE("Please specify " + params[0].description + ".");
    establishFolder(getMonitorPath("", FM_PRODUCTION));
    establishFolder(getMonitorPath("", FM_STAGING));

    if (verbose && !contains(tool_flags, "-v"))
        tool_flags += (" -v:" + uint_2_Str(verbose));
    if (verbose && !contains(freshen_flags, "-v"))
        freshen_flags += (" -v:" + uint_2_Str(verbose));

    tool_flags = trim(tool_flags, ' ');
    freshen_flags = trim(freshen_flags, ' ');

    LOG4("API_MODE", getEnvStr("API_MODE"));
    LOG4("IPFS_PATH", getEnvStr("IPFS_PATH"));
    LOG4("TEST_MODE", getEnvStr("TEST_MODE"));
    LOG4("NO_COLOR", getEnvStr("NO_COLOR"));
    LOG4("EDITOR", getEnvStr("EDITOR"));

    EXIT_NOMSG4(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    addrs.clear();
    tool_flags    = "";
    freshen_flags = "";
    mode          = "";
    stats         = false;
    minArgs       = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : txCache(WRITING_ARCHIVE) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
