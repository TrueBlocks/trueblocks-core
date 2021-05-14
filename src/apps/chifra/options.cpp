/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
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
    prePrepareArguments(unused, argc, argv);

    setProgName("chifra");

    string_q mode;
    bool has_help = false;
    for (int i = 1; i < argc; i++) {
        string_q arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            has_help = true;

        } else if (arg == "-th" || arg == "-ht") {
            has_help = true;
            isReadme = true;
            hiUp1 = hiUp2 = hiDown = '`';
            return usage("");

        } else if (!chifraCmdMap[arg].empty()) {
            if (mode == "status") {
                string_q validModes = "|index|monitors|entities|names|abis|caches|some|all|";
                if (!contains(validModes, arg)) {
                    ostringstream os;
                    os << "Invalid submode '" << arg << "' provided to status mode.";
                    return usage(os.str());
                }
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

    string_q prev;
    ostringstream os;
    os << chifraCmdMap[mode];
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

    LOG_TEST_CALL(os.str());

    // Make the actual system call and return the result
    return system(os.str().c_str());
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_DEFAULT);
    optionOn(OPT_MOCKDATA);

    // BEG_CODE_INIT
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    overrideStr = STR_CHIFRA_HELP;
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
    {"entities", "ethNames --entities"},
    {"tags", "ethNames --tags"},
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
    {"explore", "ethscan.py"},
    {"slurp", "ethslurp"},
    // END_CODE_CHIFRA_CMDMAP
};

//------------------------------------------------------------------------------------------------
const char* STR_CHIFRA_HELP =
    // BEG_CODE_CHIFRA_HELP
    "ACCOUNTS|"
    "  list          list every appearance of an address anywhere on the chain|"
    "  export        export details for each appearance (as txs, logs, traces, etc.)|"
    "  monitors      add, remove, clean, and list appearances of address(es)|"
    "  names         list and/or share named addresses|"
    "  abis          list and/or share abi signatures|"
    "CHAIN DATA|"
    "  blocks        export block-related data|"
    "  transactions  export transaction-related data|"
    "  receipts      export receipt-related data|"
    "  logs          export log-related data|"
    "  traces        export trace-related data|"
    "  when          return a date given a block number or a block number given a date|"
    "CHAIN STATE|"
    "  state         export parts of the state for given address(es)|"
    "  tokens        export data related to ERC20 and/or ERC721 token(s)|"
    "ADMIN|"
    "  status        query the status of the system|"
    "  serve         serve the TrueBlocks API via the flame server|"
    "  scrape        scrape the chain and build an index of address appearances (aka digests)|"
    "  init          initialize TrueBlocks databases by downloading pinned bloom filters|"
    "  pins          query the status of the pinning system|"
    "OTHER|"
    "  quotes        return prices collected from configured remote API|"
    "  explore       open the configured block explorer for the given address|"
    "  slurp         export details by querying EtherScan|"
    // END_CODE_CHIFRA_HELP
    ;
