/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("commands", "", "list<enum[list|export|slurp|names|abi|state|tokens|data|blocks|transactions|receipts|logs|traces|quotes|scrape|status|config|rm|message|leech|seed]>", OPT_REQUIRED | OPT_POSITIONAL, "which command to run"),  // NOLINT
    COption("sleep", "s", "<uint32>", OPT_FLAG, "for the 'scrape' and 'daemon' commands, the number of seconds chifra should sleep between runs (default 14)"),  // NOLINT
    COption("set", "e", "", OPT_HIDDEN | OPT_SWITCH, "for status config only, indicates that this is config --sef"),
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Create a TrueBlocks monitor configuration."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool visitIndexFiles(const string_q& path, void* data);
extern string_q addExportMode(format_t fmt);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER8("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG8(false);

    // BEG_CODE_LOCAL_INIT
    blknum_t start = 0;
    blknum_t end = NOPOS;
    // END_CODE_LOCAL_INIT

    bool tool_help = false;
    bool copy_to_tool = false;
    blknum_t latest = getLatestBlock_client();

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sleep:")) {
            if (!confirmUint("sleep", sleep, arg))
                return false;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmBlockNum("start", start, arg, latest))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmBlockNum("end", end, arg, latest))
                return false;

            // END_CODE_AUTO

        } else if (arg == "--set") {
            tool_flags += (arg + " ");
            copy_to_tool = true;

        } else if (arg == "-h" || arg == "--help") {
            if (mode.empty()) {
                optionOn(OPT_HELP);
                return usage();
            }
            setenv("PROG_NAME", ("chifra " + mode).c_str(), true);
            tool_help = true;

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

            bool isStatus =
                (mode == "status" && (arg == "blocks" || arg == "transactions" || arg == "traces" || arg == "names"));
            if (!isStatus && contains(descr, "|" + arg + "|")) {
                if (!mode.empty())
                    EXIT_USAGE("Please specify " + params[0].description + ". " + mode + ":" + arg);
                mode = arg;

            } else if (contains(arg, ",")) {
                if (isAddress(arg.substr(0, 42))) {
                    if (mode == "list") {
                        CStringArray parts;
                        explode(parts, arg, ',');
                        arg = parts[0];
                        freshen_flags = substitute(parts[1], "=", ":");

                    } else if (!isAddress(arg)) {
                        EXIT_USAGE("Invalid address: " + arg);
                    }
                    addrs.push_back(toLower(arg));
                } else {
                    tool_flags += substitute(arg, ",", " ");
                }

            } else if (isAddress(arg) || arg == "--known") {
                addrs.push_back(toLower(arg));

            } else {
                static format_t fmt = TXT1;
                if (arg == "csv") {
                    fmt = CSV1;
                    tool_flags += (arg + " ");
                } else if (arg == "--to_file") {
                    if (getEnvStr("DOCKER_MODE").empty()) {
                        arg = "--output:" + configPath("cache/tmp/" + makeValidName(Now().Format(FMT_EXPORT)) +
                                                       (fmt == CSV1 ? ".csv" : ".txt"));
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

    scrapeSleep = (useconds_t)sleep;

    if (mode == "blocks" || mode == "transactions" || mode == "receipts" || mode == "names" || mode == "logs" ||
        mode == "traces" || mode == "state" || mode == "message" || mode == "abi") {
        tool_flags += (" --" + mode);
        mode = "data";
    }

    if (mode.empty()) {
        optionOn(OPT_HELP);
        EXIT_USAGE("Please specify " + params[0].description + ".");
    }

    if (mode != "scrape") {
        establishFolder(getMonitorPath("", FM_PRODUCTION));
        establishFolder(getMonitorPath("", FM_STAGING));
    }

    if (tool_help) {
        tool_flags += " --help";
    }
    if (isNoHeader) {
        tool_flags += " --no_header";
    }
    if (expContext().asEther) {
        tool_flags += " --ether";
    }
    if (expContext().asDollars) {
        tool_flags += " --dollars";
    }
    if (expContext().isParity) {
        tool_flags += " --parity";
    }
    if (verbose) {
        tool_flags += " -v:" + uint_2_Str(verbose);
        freshen_flags += (" -v:" + uint_2_Str(verbose));
    }
    if (start != 0) {
        tool_flags += " --start " + uint_2_Str(start);
        freshen_flags += " --start " + uint_2_Str(start);
    }
    if (end != NOPOS) {
        tool_flags += " --end " + uint_2_Str(end);
        freshen_flags += " --end " + uint_2_Str(end);
    }
    if (true) {
        tool_flags += addExportMode(exportFmt);
        freshen_flags += addExportMode(exportFmt);
    }
    if (true) {
        tool_flags = trim(tool_flags, ' ');
        freshen_flags = trim(freshen_flags, ' ');
    }

    if (isNodeRunning()) {
//        LOG_INFO("Connecting to node...");
        blknum_t unripe, ripe, staging, finalized, client;
        getLatestBlocks(unripe, ripe, staging, finalized, client);
        if ((client - finalized) > 2500)
            scrapeSleep = 1;
        if (mode == "scrape" && !isTestMode())
            LOG_INFO("Sleeping every ", scrapeSleep, " seconds.");
    }

    LOG3("tool_flags=", tool_flags);
    LOG3("freshen_flags=", freshen_flags);

    EXIT_NOMSG8(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_HELP);

    // BEG_CODE_INIT
    sleep = 14;
    // END_CODE_INIT

    addrs.clear();
    tool_flags = "";
    freshen_flags = "";
    mode = "";
    scrapeSleep = 14;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
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
        case NONE1:
            return " --fmt none";
        case JSON1:
            return " --fmt json";
        case TXT1:
            return " --fmt txt";
        case CSV1:
            return " --fmt csv";
        case API1:
            return " --fmt api";
        default:
            break;
    }
    return "";
}
