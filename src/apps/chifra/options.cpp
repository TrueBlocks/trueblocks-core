/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("command", "", "list<enum[list|export|slurp|accounts|abi|state|tokens|data|blocks|transactions|receipts|logs|traces|quotes|scrape|status|config|rm|message|leech|seed]>", OPT_REQUIRED | OPT_POSITIONAL, "which command to run"),
    COption("sleep", "s", "<uint32>", OPT_FLAG, "for the 'scrape' and 'daemon' commands, the number of seconds chifra should sleep between runs (default 0)"),
    COption("", "", "", OPT_DESCRIPTION, "Create a TrueBlocks monitor configuration."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool visitIndexFiles(const string_q& path, void *data);
extern string_q addExportMode(format_t fmt);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER4("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG(false);

// BEG_CODE_LOCAL_INIT
// END_CODE_LOCAL_INIT

    bool copy_to_tool = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
// END_CODE_AUTO

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sleep:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--sleep:", "");
            if (!isUnsigned(arg))
                EXIT_USAGE("--sleep must be a non-negative number. Quitting...");
            scrapeSleep = (useconds_t)str_2_Uint(arg);

        } else if (arg == "--tool_help" || (isApiMode() && arg == "--help")) {
            tool_flags += (" --help");

        } else if (arg == "--set") {
            tool_flags += (arg + " ");
            copy_to_tool = true;

        } else if (mode.empty() && startsWith(arg, '-')) {

            if (!builtInCmd(arg))
                EXIT_USAGE("Missing mode: " + arg);

        } else {

            if (copy_to_tool) {
                tool_flags += (arg + '~');
                continue;
            }

            string descr = substitute(substitute(params[0].description, "[", "|"), "]", "|");
            if (isTestMode())
                descr += "where|when|tokens|blooms|";

            bool isStatus = (mode == "status" && (arg == "blocks" || arg == "transactions" || arg == "traces"));
            if (!isStatus && contains(descr, "|" + arg + "|")) {
                if (!mode.empty())
                    EXIT_USAGE("Please specify " + params[0].description + ". " + mode + ":" + arg);
                mode = arg;

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

            } else if (isAddress(arg) || arg == "--known") {
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
                    if (arg == "--staging") {
                        freshen_flags += (arg + " ");

                    } else if (startsWith(arg, "--start:") || startsWith(arg, "--end:")) {
                        freshen_flags += (arg + " ");
                        tool_flags += (arg + " ");

                    } else {
                        tool_flags += (arg + " ");
                    }
                }
            }
        }
    }

    if (mode == "blocks" ||
    	mode == "transactions" ||
        mode == "receipts" ||
        mode == "accounts" ||
    	mode == "logs" ||
        mode == "traces" ||
        mode == "state" ||
        mode == "message" ||
        mode == "abi") {
        tool_flags += (" --" + mode);
        mode = "data";
    }

    if (mode.empty())
        EXIT_USAGE("Please specify " + params[0].description + ".");

    if (mode != "scrape") {
        establishFolder(getMonitorPath("", FM_PRODUCTION));
        establishFolder(getMonitorPath("", FM_STAGING));
    }

    if (verbose) { freshen_flags += (" -v:" + uint_2_Str(verbose)); }
    freshen_flags += addExportMode(exportFmt);
    freshen_flags = trim(freshen_flags, ' ');

    if (verbose) { tool_flags += (" -v:" + uint_2_Str(verbose)); }
    if (expContext().asEther) { tool_flags += " --ether"; }
    if (expContext().asDollars) { tool_flags += " --dollars"; }
    if (expContext().isParity) { tool_flags += " --parity"; }
    tool_flags += addExportMode(exportFmt);
    tool_flags = trim(tool_flags, ' ');

    LOG_INFO("Connecting to node...");
    if (isNodeRunning()) {
        blknum_t unripe, ripe, staging, finalized, client;
        getLastBlocks(unripe, ripe, staging, finalized, client);
        if ((client - finalized) > 2500)
            scrapeSleep = 1;
        if (mode == "scrape" && !isTestMode())
            LOG_INFO("Sleeping every ", scrapeSleep, " seconds.");
    }

    if (mode == "config") {
        if (contains(tool_flags, "get") && !contains(tool_flags, "--get"))
            replace(tool_flags, "get", "--get"); // syntactic sugar for command line
        if (contains(tool_flags, "set") && !contains(tool_flags, "--set"))
            replace(tool_flags, "set", "--set"); // syntactic sugar for command line
        replaceAll(tool_flags, "--get", "--get_config");
        replaceAll(tool_flags, "--set", "--set_config");
        if (!startsWith(tool_flags, "--get_config") && !startsWith(tool_flags, "--set_config"))
            EXIT_USAGE("chifra config 'mode' must be either '--get' or '--set'.");
    }

    LOG4("tool_flags=", tool_flags);
    LOG4("freshen_flags=", freshen_flags);

    EXIT_NOMSG4(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

// BEG_CODE_INIT
// END_CODE_INIT

    addrs.clear();
    tool_flags    = "";
    freshen_flags = "";
    mode          = "";
    scrapeSleep   = 14;
    minArgs       = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q addExportMode(format_t fmt) {
    if (isApiMode() && fmt == API1)
        return "";
    if (!isApiMode() && fmt == TXT1)
        return "";
    switch (fmt) {
        case NONE1: return " --fmt none";
        case JSON1: return " --fmt json";
        case TXT1:  return " --fmt txt";
        case CSV1:  return " --fmt csv";
        case API1:  return " --fmt api";
        default:
            break;
    }
    return "";
}
