/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
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
    for (int i = 0; i < argc; i++)
        command += (string_q(argv[i]) + " ");
    if (!standardOptions(command))
        return false;

    ENTER("call_command");

    CStringArray unused;
    prePrepareArguments(unused, argc, argv);

    string_q mode;
    bool has_help = false;
    for (int i = 1; i < argc; i++) {
        string_q arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            has_help = true;

        } else if (arg == "-th" || arg == "-ht") {
            isReadme = true;
            hiUp1 = hiUp2 = hiDown = '`';

        } else if (!cmdMap[arg].empty()) {
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
    LOG_TEST("mode", mode, mode == "");
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        EXIT_NOMSG(false);

    setProgName("chifra");

    // show chifra's help in limited cases, the tool's help otherwise
    if (has_help && (argc == 2 || mode == "serve"))
        EXIT_USAGE("");

    // Make sure user sent a real subcommand
    if (argc > 1 && cmdMap[argv[1]].empty())
        EXIT_USAGE("Unknown subcommand '" + string_q(argv[1]) + "'.");

    if (mode.empty())
        EXIT_USAGE("The first argument you provide must be a chifra subcommand.");

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
    os << cmdMap[mode];
    for (int i = 2; i < argc; i++) {
        // If we're not removing it...
        if (!removeMap[argv[i]]) {
            string_q arg = argv[i];
            // ... and it's either not an address or not already in the map, pass it through
            if (!isAddress(arg) || !addressMap[arg])
                os << " " << argv[i];
            if (prev != "--emitted_by" && prev != "--emitter" && prev != "--relevant")
                addressMap[arg] = isAddress(arg);
            prev = arg;
        }
    }

    LOG_CALL(os.str());

    // Make the actual system call and return the result
    EXIT_NOMSG(system(os.str().c_str()));
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_DEFAULT);
    optionOn(OPT_MOCKDATA);

    // BEG_CODE_INIT
    // END_CODE_INIT
}

extern const char* STR_FULL_HELP;
//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    overrideStr = STR_FULL_HELP;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//------------------------------------------------------------------------------------------------
map<string, string> cmdMap = {{"monitor", "acctExport --appearances"},
                              {"export", "acctExport"},
                              {"entities", "ethNames --entities"},
                              {"names", "ethNames"},
                              {"tags", "ethNames --tags"},
                              {"abis", "grabABI"},
                              {"blocks", "getBlock"},
                              {"transactions", "getTrans"},
                              {"receipts", "getReceipt"},
                              {"logs", "getLogs"},
                              {"traces", "getTrace"},
                              {"state", "getState"},
                              {"tokens", "getTokenInfo"},
                              {"when", "whenBlock"},
                              {"init", "pinMan local --init"},
                              {"scrape", "blockScrape"},
                              {"serve", "tbServer"},
                              {"pins", "pinMan"},
                              {"status", "cacheStatus"},
                              {"explore", "ethscan.py"},
                              {"slurp", "ethslurp"},
                              {"quotes", "ethQuote"},
                              {"where", "env DICT_MODE=true whereBlock"},
                              {"list", "acctExport --appearances"},
                              {"dive", "turboDive"}};

//------------------------------------------------------------------------------------------------
const char* STR_FULL_HELP =
    "MONITORS|"
    "  list          list every appearance of an address anywhere on the chain|"
    "  export        export details for each appearance (as txs, logs, traces, balances, reconciliations, etc.)|"
    "  monitor       add, remove, clean, and list appearances of address(es) on the chain|"
    "SHARED DATA|"
    "  entities      list and/or share entities (groups of addresses)|"
    "  names         list and/or share named addresses|"
    "  tags          list and/or share tags (subgroups of addresses)|"
    "  abis          list and/or share abi signatures|"
    "BLOCKCHAIN DATA|"
    "  blocks        export block-related data|"
    "  transactions  export transaction-related data|"
    "  receipts      export receipt-related data|"
    "  logs          export log-related data|"
    "  traces        export trace-related data|"
    "  state         export parts of the state for given address(es)|"
    "  tokens        export data related to ERC20 and/or ERC721 token(s)|"
    "  when          return a date given a block number or a block number given a date|"
    "ADMIN|"
    "  init          initialize TrueBlocks databases by downloading pinned bloom filters|"
    "  scrape        scrape the chain and build an index of address appearances (aka digests)|"
    "  serve         serve the TrueBlocks API via tbServer|"
    "  pins          query the status of the pinning system|"
    "  status        query the status of the system|"
    "OTHER|"
    "  explore       open the configured block explorer for the given address|"
    "  slurp         export details by querying EtherScan (note: will not return as many appearances as --list)|"
    "  quotes        return prices collected from configured remote API|"
    "  where         determine the location of block(s), either local or remote cache, or on-chain";
