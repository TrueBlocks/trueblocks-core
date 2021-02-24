/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern string_q getSubcommands(void);
//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("command", getSubcommands(), "<string>", OPT_REQUIRED | OPT_POSITIONAL, "which command to run"),
    COption("", "", "", OPT_DESCRIPTION, "Access to all TrueBlocks tools (`chifra <cmd> --help` for more)."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//------------------------------------------------------------------------------------------------
bool COptions::call_command(int argc, const char* argv[]) {
    verbose = 10;  // rather be more than less verbose here
    ENTER("call_command");

    CStringArray unused;
    prePrepareArguments(unused, argc, argv);

    string_q mode;
    bool has_help = false;
    bool has_run = false;
    for (int i = 1; i < argc; i++) {
        string_q arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            has_help = true;

        } else if (arg == "run") {
            has_run = true;

        } else if (arg == "-th" || arg == "-ht") {
            isReadme = true;
            hiUp1 = hiUp2 = hiDown = '`';

        } else if (!cmdMap[arg].empty()) {
            if (mode.empty()) {
                mode = arg;
            } else {
                ostringstream os;
                os << "Provide only a single mode. Encountered both '" << mode << "' and '" << arg << "'.";
                return usage(os.str());
            }
        }
    }

    // BEG_DEBUG_TEST
    LOG_TEST("mode", mode, mode == "");
    // END_DEBUG_TEST

    setProgName("chifra");
    // show chifra's help in limited cases, the tool's help otherwise
    if (has_help && (argc == 2 || mode == "serve"))
        EXIT_USAGE("");

    if (!mode.empty() && mode != "serve")
        setenv("PROG_NAME", ("chifra " + string_q(argv[1])).c_str(), true);

    if (argc > 1 && cmdMap[argv[1]].empty())
        EXIT_USAGE("Unknown subcommand '" + string_q(argv[1]) + "'.");

    if (mode.empty())
        EXIT_USAGE("The first argument you provide must be a chifra subcommand.");

    if (isApiMode() && mode == "scrape" && has_run) {
        cout << "{ \"status\": \"cannot run\" }";
        LOG_ERR(
            "Use the API to pause, restart, or quit the scraper -- not to run it. Instead "
            "open a new terminal window and enter ",
            cTeal, "chifra scrape run", cOff, ".");
        EXIT_NOMSG(false);
    }

    CStringBoolMap removeMap;
    removeMap["--names"] = true;
    removeMap["--terms"] = true;
    removeMap["--addrs"] = true;

    // everything past the mode gets sent to the tool...
    ostringstream os;
    os << cmdMap[mode];
    for (int i = 2; i < argc; i++)
        if (!removeMap[argv[i]])
            os << " " << argv[i];

    LOG_CALL(os.str());
    EXIT_NOMSG(system(os.str().c_str()));
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_DEFAULT);
    optionOn(OPT_MOCKDATA);

    // BEG_CODE_INIT
    // END_CODE_INIT

    // addrs.clear();
    // tool_flags = "";
    // mode = "";
    // minArgs = 0;
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
map<string, string> cmdMap = {{"slurp", "ethslurp"},
                              {"entities", "ethNames --entities"},
                              {"names", "ethNames"},
                              {"tags", "ethNames --tags"},
                              {"abis", "grabABI"},
                              {"blocks", "getBlock"},
                              {"transactions", "getTrans"},
                              {"receipts", "getReceipt"},
                              {"logs", "getLogs"},
                              {"traces", "getTrace"},
                              {"quotes", "ethQuote"},
                              {"state", "getState"},
                              {"tokens", "getTokenInfo"},
                              {"when", "whenBlock"},
                              {"where", "whereBlock"},
                              {"status", "cacheStatus"},
                              {"monitor", "acctExport --appearances"},
                              {"list", "acctExport --appearances"},
                              {"export", "acctExport"},
                              {"scrape", "blockScrape"},
                              {"init", "pinStatus local --init"},
                              {"dive", "turboDive"},
                              {"serve", "tbServer"},
                              {"pins", "pinStatus"},
                              {"explore", "ethscan.py"}};

//------------------------------------------------------------------------------------------------
const char* cmdList =
    "monitor|"
    "export|"
    "entities|"
    "names|"
    "tags|"
    "abis|"
    "blocks|"
    "transactions|"
    "receipts|"
    "logs|"
    "traces|"
    "state|"
    "tokens|"
    "when|"
    "init|"
    "scrape|"
    "serve|"
    "pins|"
    "status|"
    "explore|"
    "slurp|"
    "quotes|"
    "where|";

//------------------------------------------------------------------------------------------------
string_q getSubcommands(void) {
    CStringArray cmds;
    explode(cmds, cmdList, '|');
    ostringstream out;
    for (auto cmd : cmds)
        out << cmd << "|";
    string_q l = trim(out.str(), '|');
    return "list<enum[" + l + "]>";
}

//------------------------------------------------------------------------------------------------
const char* STR_FULL_HELP =
    "MONITORS|"
    "  monitor       add, remove, clean, and list appearances of address(es) on the chain|"
    "  export        export details for each appearance (as txs, logs, traces, balances, reconciliations, etc.)|"
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

// if (mocked) {
//     string_q which = origMode;
//     if (origMode == "names") {
//         if (contains(tool_flags, "tags")) {
//             origMode = "tags";
//         } else if (contains(tool_flags, "entities")) {
//             origMode = "entities";
//         }
//     } else if (origMode == "status") {
//         if (contains(tool_flags, "monitors")) {
//             origMode = "monitors";
//         }
//     }
//     uint64_t nMocked = getGlobalConfig("")->getConfigInt("dev", "n_mocked", 100);
//     string_q path = configPath("mocked/" + origMode + ".json");
//     if (fileExists(path)) {
//         if (origMode == "export") {
//             for (size_t i = 0; i < nMocked; i++) {
//                 LOG_PROGRESS1("Extracting", i, nMocked, "\r");
//                 usleep(30000);
//             }
//             CStringArray lines;
//             asciiFileToLines(path, lines);
//             size_t cnt = 0;
//             size_t record = 0;
//             size_t recordSize = lines.size() / nMocked;
//             for (auto line : lines) {
//                 cout << line << endl;
//                 if (!(++cnt % recordSize)) {
//                     LOG_PROGRESS1("Displaying", record++, nMocked, "\r");
//                     usleep(10000);
//                 }
//             }
//             return false;
//         } else {
//             cout << asciiFileToString(path);
//             return false;
//         }
//     }
//     tool_flags += " --mocked ";
// }

// This will probably break in the real usage.
// CStringArray scraper = {"--restart", "--pause", "--quit"};
// for (auto cmd : scraper)
//     replace(tool_flags, cmd, substitute(cmd, "--", ""));

// TODO(tjayrush): Do not allow duplicate addresses in the command line
// TODO(tjayrush): Re-enable mocked data
// TODO(tjayrush): Re-enable the ability to start, stop, pause, and quit the block scraper
