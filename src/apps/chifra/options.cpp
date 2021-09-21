/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("command", "", "<string>", OPT_REQUIRED | OPT_POSITIONAL, "which command to run"),
    COption("", "", "", OPT_DESCRIPTION, "Access to all TrueBlocks tools (`chifra <cmd> --help` for more)."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//------------------------------------------------------------------------------------------------
bool COptions::call_command(int argc, const char* argv[]) {
    string_q command;
    for (int i = 1; i < argc; i++)
        command += (string_q(argv[i]) + " ");
    if (!standardOptions(command))
        return false;

    CStringArray unused;
    if (!prePrepareArguments(unused, argc, argv))  // returns false if readme is requested
        return usage();

    setProgName("chifra");

    string_q mode, scrape_mode;
    bool has_help = false;
    for (int i = 1; i < argc; i++) {
        string_q arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            has_help = true;

        } else if (!chifraCmdMap[arg].empty()) {
            if (mode == "status") {
                string_q validModes = "|index|monitors|collections|names|abis|caches|some|all|";
                if (!contains(validModes, arg)) {
                    ostringstream os;
                    os << "Invalid submode '" << arg << "' provided to status mode.";
                    return usage(os.str());
                }
                // pass it through
            } else if (mode == "scrape") {
                string_q validModes = "|index|monitors|both|";
                if (!contains(validModes, arg)) {
                    ostringstream os;
                    os << "Invalid submode '" << arg << "' provided to scrape mode.";
                    return usage(os.str());
                }
                scrape_mode = arg;
                // pass it through
            } else if (!mode.empty()) {
                ostringstream os;
                os << "Provide only a single mode. Encountered both '" << mode << "' and '" << arg << "'.";
                return usage(os.str());
            } else {
                mode = arg;
            }
        }
    }

    // BEG_DEBUG_DISPLAY
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    // show chifra's help in limited cases, the tool's help otherwise
    if (has_help && (argc == 2))
        return usage("");

    // Make sure user sent a real subcommand
    if (argc > 1 && chifraCmdMap[argv[1]].empty())
        return usage("Unknown subcommand '" + string_q(argv[1]) + "'.");

    if (mode.empty())
        return usage("");

    // We want the help screen to display 'chifra subcommand' and not the program's name
    if (!mode.empty() && mode != "serve")
        setenv("PROG_NAME", ("chifra " + string_q(argv[1])).c_str(), true);

    // Everything past the mode gets sent to the tool...execpt a few syntactic sugars
    CStringBoolMap removeMap;
    removeMap["--names"] = true;
    removeMap["--terms"] = true;
    removeMap["--addrs"] = true;

    CAddressBoolMap addressMap;

    string_q realCmd = chifraCmdMap[mode];
    if (!has_help)
        replace(realCmd, "blockScrape", "flame");
    string_q prev;
    ostringstream os;
    os << getCommandPath(realCmd);
    for (int i = 2; i < argc; i++) {
        // If we're not removing it...
        if (!removeMap[argv[i]]) {
            string_q arg = argv[i];
            // ... and it's either not an address or not already in the map, pass it through
            if (!isAddress(arg) || !addressMap[arg])
                os << " " << argv[i];
            if (prev != "--source" && prev != "--emitter" && prev != "--relevant")
                addressMap[arg] = isAddress(arg);
            prev = arg;
        }
    }
    if (mode == "scrape") {
        if (scrape_mode == "index" || scrape_mode == "both" || scrape_mode.empty())
            os << " --scrape";
        if (scrape_mode == "monitors" || scrape_mode == "both")
            os << " --monitor";
    }

    LOG_TEST_CALL(os.str());

    // Make the actual system call and return the result
    return system(os.str().c_str());
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, OPT_MOCKDATA, OPT_DEFAULT);

    // BEG_CODE_INIT
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    overrides.push_back(STR_CHIFRA_HELP);
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//------------------------------------------------------------------------------------------------
map<string, string> chifraCmdMap = {
    // BEG_CODE_CHIFRA_CMDMAP
    // -- Accounts
    {"list", "acctExport --appearances"},
    {"export", "acctExport"},
    {"monitors", "acctExport --appearances"},
    {"names", "ethNames"},
    {"abis", "grabABI"},
    // -- Chain Data
    {"blocks", "getBlocks"},
    {"transactions", "getTrans"},
    {"receipts", "getReceipts"},
    {"logs", "getLogs"},
    {"traces", "getTraces"},
    {"when", "whenBlock"},
    // -- Chain State
    {"state", "getState"},
    {"tokens", "getTokens"},
    // -- Admin
    {"status", "cacheStatus"},
    {"serve", "flame"},
    {"scrape", "blockScrape"},
    {"init", "pinMan local --init"},
    {"pins", "pinMan"},
    // -- Other
    {"quotes", "getQuotes"},
    {"explore", "fireStorm"},
    {"slurp", "ethslurp"},
    // END_CODE_CHIFRA_CMDMAP
};

//------------------------------------------------------------------------------------------------
const char* STR_CHIFRA_HELP =
    // BEG_CODE_CHIFRA_HELP
    "ACCOUNTS|"
    "  list          list every appearance of an address anywhere on the chain|"
    "  export        export full detail of transactions for one or more addresses|"
    "  monitors      add, remove, clean, and list address monitors|"
    "  names         query addresses or names of well known accounts|"
    "  abis          fetches the ABI for a smart contract|"
    "CHAIN DATA|"
    "  blocks        retrieve one or more blocks from the chain or local cache|"
    "  transactions  retrieve one or more transactions from the chain or local cache|"
    "  receipts      retrieve receipts for the given transaction(s)|"
    "  logs          retrieve logs for the given transaction(s)|"
    "  traces        retrieve traces for the given transaction(s)|"
    "  when          find block(s) based on date, blockNum, timestamp, or 'special'|"
    "CHAIN STATE|"
    "  state         retrieve account balance(s) for one or more addresses at given block(s)|"
    "  tokens        retrieve token balance(s) for one or more addresses at given block(s)|"
    "ADMIN|"
    "  status        report on the status of the TrueBlocks system|"
    "  serve         serve the TrueBlocks API using the flame server|"
    "  scrape        scan the chain and update the TrueBlocks index of appearances|"
    "  init          initialize the index of appearances by downloading Bloom filters|"
    "  pins          manage pinned index of appearances and associated Bloom filters|"
    "OTHER|"
    "  quotes        freshen and/or display Ethereum price data|"
    "  explore       open an explorer for a given address, block, or transaction|"
    "  slurp         fetch data from EtherScan for any address|"
    // END_CODE_CHIFRA_HELP
    ;
