/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~command", "one of [ leech | scrape | list | export | balances | stats | ls | rm | accounts | config | slurp | quotes | data | blocks | trans | receipts | logs | traces | state | abi ]"),
    COption("-sleep:<num>", "for the 'scrape' command, the number of seconds chifra should sleep between runs (default 14 if caught up, zero otherwise)"),
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
        if (startsWith(arg, "-s:") || startsWith(arg, "--sleep:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--sleep:", "");
            if (!isUnsigned(arg))
                return usage("--nBlocks must be a non-negative number. Quitting...");
            scrapeSleep = (useconds_t)str_2_Uint(arg);

        } else if (arg == "--tool_help" || (api_mode && arg == "--help")) {
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

            } else if (contains(arg, ",") && isAddress(arg.substr(0,42))) {
                if (contains(arg, ",--start=") && mode == "list") {
                    CStringArray parts;
                    explode(parts, arg, ',');
                    arg = parts[0];
                    freshen_flags = substitute(parts[1], "=", ":");

                } else if (!isAddress(arg)) {
                    EXIT_USAGE("Invalid address: " + arg);
                }
                addrs.push_back(toLower(arg));

            } else if (isAddress(arg)) {
                addrs.push_back(toLower(arg));

            } else {
                static format_t fmt = TXT1;
                if (arg == "csv") {
                    fmt = CSV1;
                    tool_flags += (arg + " ");
                } else if (arg == "--to_file") {
                    if (getEnvStr("DOCKER_MODE").empty()) {
                        arg = "--output:" + configPath("cache/tmp/" + makeValidName(Now().Format(FMT_EXPORT)) + (fmt == CSV1 ? ".csv" : ".txt"));
                        tool_flags += (arg + " ");
                    } else {
                        // ignore --to_file flag in docker mode
                    }
                } else {
                    if (arg == "--noBlooms")
                        freshen_flags = arg;
                    else
                        tool_flags += (arg + " ");
                }
            }
        }
    }

    if (mode == "blocks" ||
    	mode == "trans" ||
        mode == "receipts" ||
        mode == "accounts" ||
    	mode == "logs" ||
        mode == "traces" ||
        mode == "abi") {
        tool_flags += (" --" + mode);
        mode = "data";
    }

    if (mode == "state") {
        replace(tool_flags, "--mode code", "--mode some --code");
        replace(tool_flags, "--mode nonce", "--mode some --nonce");
        replace(tool_flags, "--mode balance", "--mode some --balance");
        mode = "data";
    }

    if (mode.empty())
        EXIT_USAGE("Please specify " + params[0].description + ".");

    if (mode != "scrape") {
        establishFolder(getMonitorPath("", FM_PRODUCTION));
        establishFolder(getMonitorPath("", FM_STAGING));
    }

    if (verbose && !contains(tool_flags, "-v"))
        tool_flags += (" -v:" + uint_2_Str(verbose));
    if (verbose && !contains(freshen_flags, "-v"))
        freshen_flags += (" -v:" + uint_2_Str(verbose));

    tool_flags = trim(tool_flags, ' ');
    freshen_flags = trim(freshen_flags, ' ');

    blknum_t unripe, ripe, staging, finalized, client;
    getLastBlocks(unripe, ripe, staging, finalized, client);
    if ((client - finalized) > 2500) {
        cerr << "Sleeping zero: " << scrapeSleep << " " << endl;
        scrapeSleep = 0;
    } else {
        if (mode == "scrape") {
            cerr << "Sleeping every " << scrapeSleep << " seconds." << endl;
        }
    }

    LOG4("API_MODE=", getEnvStr("API_MODE"));
    LOG4("DOCKER_MODE=", getEnvStr("DOCKER_MODE"));
    LOG4("IPFS_PATH=", getEnvStr("IPFS_PATH"));
    LOG4("TEST_MODE=", getEnvStr("TEST_MODE"));
    LOG4("NO_COLOR=", getEnvStr("NO_COLOR"));
    LOG4("EDITOR=", getEnvStr("EDITOR"));
    LOG4("tool_flags=", tool_flags);
    LOG4("freshen_flags=", freshen_flags);

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
    scrapeSleep   = 14;
    minArgs       = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
