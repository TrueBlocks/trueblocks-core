/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~command", "one of [ init | freshen | export | list | ls | seed | daemon | scrape | config ]"),
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

            } else {
                remainder += (arg + "|");
            }
        }
    }

    if (mode.empty())
        return usage("Please specify " + params[0].description + ". Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    remainder = "";
    mode      = "";
    minArgs   = 0;
    watches.clear();
    if (fileExists("./config.toml")) {
        CToml toml("./config.toml");
        loadMonitors(toml);
    }
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : txCache(WRITING_ARCHIVE) {
    Init();
    HIDE_FIELD(CAccountWatch, "statement");
    HIDE_FIELD(CAccountWatch, "balanceHistory");
    HIDE_FIELD(CAccountWatch, "nodeBal");
    SHOW_FIELD(CAccountWatch, "api_spec");
    SHOW_FIELD(CApiSpec,      "method");
    SHOW_FIELD(CApiSpec,      "uri");
    SHOW_FIELD(CApiSpec,      "headers");
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q colors[] = {
    "green_c", "blue_c", "red_c", "magenta_c", "yellow_c", "teal_c", "white_b",
    "green_b", "blue_b", "red_b", "magenta_b", "yellow_b", "teal_b", "black_b",
};
uint64_t nColors = sizeof(colors) / sizeof(string_q);

//-----------------------------------------------------------------------
bool COptions::loadMonitors(CToml& toml) {

    minWatchBlock = UINT32_MAX;
    maxWatchBlock = 0;

    string_q watchStr = toml.getConfigJson("watches", "list", "");
    if (watchStr.empty())
        return usage("Empty list of watches. Quitting.");

    CAccountWatch watch;
    while (watch.parseJson3(watchStr)) {
        // cleanup and report on errors
        watch.color   = convertColor(watch.color);
        watch.address = str_2_Addr(toLower(watch.address));
        watch.nodeBal = getBalanceAt(watch.address, watch.firstBlock-1);
        watch.api_spec.method = toml.getConfigStr("api_spec", "method", "");
        watch.api_spec.uri = toml.getConfigStr("api_spec", "uri", "");
        watch.api_spec.headers = toml.getConfigStr("api_spec", "headers", "");
        if (!watch.api_spec.uri.empty()) {
            watch.abi_spec.loadAbiByAddress(watch.address);
            watch.abi_spec.loadAbiKnown("all");
        }

        string_q msg;
        if (!isAddress(watch.address)) {
            msg = "invalid address " + watch.address;
        }
        if (watch.name.empty()) {
            if (!msg.empty())
                msg += ", ";
            msg += "no name " + watch.name;
        }

        // add to array or return error
        if (msg.empty()) {
            minWatchBlock = min(minWatchBlock, watch.firstBlock);
            maxWatchBlock = max(maxWatchBlock, watch.lastBlock);
            watches.push_back(watch);

        } else {
            return usage(msg);

        }
        watch = CAccountWatch();  // reset
    }
    monitorName = toml.getConfigStr("settings", "name", watches[0].name);
    return true;
}
