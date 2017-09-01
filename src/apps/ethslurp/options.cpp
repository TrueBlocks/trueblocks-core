/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
CFileExportContext outScreen;

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~addr",            "the address of the account or contract to slurp"),
    CParams("-archive:<str>",   "filename of output (stdout otherwise)"),
    CParams("-blocks:<range>",  "export records in block range (:0[:max])"),
    CParams("-dates:<date>",    "export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]])"),
    CParams("-fmt:<str>",       "pretty print, optionally add ':txt,' ':csv,' or ':html'"),
    CParams("-income",          "include income transactions only"),
    CParams("-expense",         "include expenditures only"),
    CParams("@--rerun",         "re-run the most recent slurp"),
    CParams("@--open",          "open the configuration file for editing"),
    CParams("@--list",          "list previously slurped addresses in cache"),
    CParams("@--max:<val>",     "maximum transactions to slurp (:250000)"),
    CParams("@--sleep:<val>",   "sleep for :x seconds"),
    CParams("@--func:<str>",    "display only --func:functionName records"),
    CParams("@--errFilt:<val>", "display only non-error transactions or only errors with :errsOnly"),
    CParams("@--reverse",       "display results sorted in reverse chronological order (chronological by default)"),
    CParams("@--acct_id:<val>", "for 'cache' mode, use this as the :acct_id for the cache (0 otherwise)"),
    CParams("@--cache",         "write the data to a local QuickBlocks cache"),
    CParams("@--name:<str>",    "name this address"),
    CParams("",                 "Fetches data off the Ethereum blockchain for an arbitrary account or smart "
                                "contract. Optionally formats the output to your specification. Note: --income "
                                "and --expense are mutually exclusive as are --blocks and --dates.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg == "-i" || arg == "--income") {
            if (expenseOnly)
                return usage("Only one of --income or --expense may be specified.");
            incomeOnly = true;

        } else if (arg == "-e" || arg == "--expense") {
            if (incomeOnly)
                return usage("Only one of --income or --expense may be specified.");
            expenseOnly = true;

        } else if (arg == "-f") {
            // -f by itself is json prettyPrint
            prettyPrint = true;
            exportFormat = "json";

        } else if (arg.startsWith("-f:") || arg.startsWith("--fmt:")) {
            prettyPrint = true;
            exportFormat = arg.Substitute("-f:", "").Substitute("--fmt:", "");
            if (exportFormat.empty())
                return usage("Please provide a formatting option with " + orig + ". Quitting...");

        } else if (arg.startsWith("--func:")) {
            funcFilter = arg.Substitute("--func:", "");
            if (funcFilter.empty())
                return usage("Please provide a function to filter on " + orig + ". Quitting...");

        } else if (arg.startsWith("--errFilt")) {
            // weird, but 1 == no errors, 2 == errors only
            errFilt = true + arg.Contains(":errOnly");

        } else if (arg.startsWith("--reverse")) {
            reverseSort = true;

        } else if (arg.startsWith("--acct_id:")) {
            arg = arg.Substitute("--acct_id:", "");
            acct_id = toLong32u(arg);

        } else if (arg.startsWith("--cache")) {
            cache = true;

        } else if (arg == "-l" || arg == "--list") {
            uint32_t nFiles = 0;
            listFilesOrFolders(nFiles, NULL, configPath("slurps/*.bin"));
            if (nFiles) {
                SFString *files = new SFString[nFiles];
                listFilesOrFolders(nFiles, files, configPath("slurps/*.bin"));
                for (uint32_t i = 0 ; i < nFiles ; i++)
                    outScreen << files[i] << "\n";
                delete [] files;
            } else {
                outScreen << "No files found in cache folder '" << configPath("slurps/") << "'\n";
            }
            exit(0);

        } else if (arg == "-b") {
            return usage("Invalid option -b. This option must include :firstBlock or :first:lastBlock range.");

        } else if (arg.startsWith("-b:") || arg.startsWith("--blocks:")) {

            if (firstDate != earliestDate || lastDate != latestDate)
                return usage("Specifiy either a date range or a block range, not both. Quitting...");

            SFString lastStr = arg.Substitute("-b:", "").Substitute("--blocks:", "");
            if (lastStr.startsWith("0x"))
                return usage("Invalid block specified (" + orig + "). Blocks must be integers. Quitting...");

            SFString firstStr = nextTokenClear(lastStr, ':');
            if (firstStr.startsWith("0x"))
                return usage("Invalid block specified (" + orig + "). Blocks must be integers. Quitting...");

            firstBlock2Read = max(toLongU("0"), toLongU(firstStr));
            if (!lastStr.empty())
                lastBlock2Read = max(firstBlock2Read, toLongU(lastStr));

            // Don't have to check that last is later than first since the clamp

        } else if (arg == "-d") {
            return usage("Invalid option -d. This option must include :firstDate or :first:lastDate range.");

        } else if (arg.startsWith("-d:") || arg.startsWith("--dates:")) {

            if (firstBlock2Read != 0 || lastBlock2Read != NOPOS)
                return usage("Specifiy either a date range or a block range, not both. Quitting...");

            SFString lateStr = arg.Substitute("-d:", "").Substitute("--dates:", "");
            SFString earlyStr = nextTokenClear(lateStr, ':');
            if (!earlyStr.empty() && !isNumeral(earlyStr))
                return usage("Invalid date: " + orig + ". Quitting...");
            if (!lateStr.empty() && !isNumeral(lateStr))
                return usage("Invalid date: " + orig + ". Quitting...");

            earlyStr.ReplaceAll("-", "");
            lateStr.ReplaceAll("-", "");

            if (!earlyStr.empty() && earlyStr.length() != 8 && earlyStr.length() != 14)
                return usage("Option -d: Invalid date format for startDate. "
                                "Format must be either yyyymmdd or yyyymmddhhmmss.");

            if (!lateStr.empty() && lateStr.length() != 8 && lateStr.length() != 14)
                return usage("Option -d: Invalid date format for endDate. "
                                "Format must be either yyyymmdd or yyyymmddhhmmss.");

            firstDate = snagDate(earlyStr, -1);
            lastDate = snagDate(lateStr, 1);
            if (lastDate == earliestDate)  // the default
                lastDate = latestDate;

            if (firstDate > lastDate) {
                return usage("lastDate (" + lastDate.Format(FMT_DEFAULT) +
                             ") must be later than startDate (" + firstDate.Format(FMT_DEFAULT) +
                             "). Quitting...");
            }

        } else if (arg == "-r" || arg == "--rerun") {
            rerun = true;

        } else if (arg.startsWith("--sleep:")) {
            arg = arg.Substitute("--sleep:", "");
            if (arg.empty() || !isdigit(arg[0]))
                return usage("Sleep amount must be a numeral. Quitting...");
            uint32_t wait = toLong32u(arg);
            if (wait) {
                cerr << "Sleeping " << wait << " seconds\n";
                qbSleep(wait);
            }

        } else if (arg.startsWith("-m:") || arg.startsWith("-max:")) {
            SFString val = arg.Substitute("-m:", "").Substitute("--max:", "");
            if (val.empty() || !isdigit(val[0]))
                return usage("Please supply a value with the --max: option. Quitting...");
            maxTransactions = toLong32u(val);

        } else if (arg.startsWith("-n:") || arg.startsWith("-name:")) {
            SFString val = arg.Substitute("-m:", "").Substitute("--max:", "");
            if (val.empty())
                return usage("You must supply a name with the --name option. Quitting...");
            name = val;

        } else if (arg == "-a") {
            // -a is acceptable but only if we get a -name (or we have only already)
            // checked during slurp since we don't have an address yet
            wantsArchive = true;
            archiveFile = "";

        } else if (arg.startsWith("-a:") || arg.startsWith("--archive:")) {
            SFString fileName = arg.Substitute("-a:", "").Substitute("--archive:", "");

            CFilename filename(fileName);
            if (!filename.getPath().startsWith('/'))
                return usage("Archive file '" + arg + "' does not resolve to a full path. "
                             "Use ./path/filename, ~/path/filename, or a fully qualified path.");
            archiveFile = filename.getFullPath();
            wantsArchive = true;

        } else if (arg == "-o" || arg == "--open") {

            editFile(configPath("quickBlocks.toml"));
            exit(0);

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            addr = fixAddress(arg);
            if (!isAddress(addr))
                 return usage(addr + " appears to be an invalid address. Valid addresses start with '0x' and are 20 bytes (40 chars) long. Quitting...");
        }
    }

    if (wantsArchive && archiveFile.empty() && name.empty())
        return usage("If -a is provided without an archive name, -n must be provided. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    prettyPrint = false;
    rerun = false;
    incomeOnly = false;
    expenseOnly = false;
    openFile = false;
    funcFilter = "";
    errFilt = false;
    reverseSort = false;
    firstBlock2Read = 0;
    lastBlock2Read = NOPOS;
    firstDate = earliestDate;
    lastDate = latestDate;
    maxTransactions = 250000;
    pageSize = 5000;
    exportFormat = "json";
    archiveFile = "";
    wantsArchive = false;
    cache = false;
    acct_id = 0;
    addr = "";

    outScreen.setOutput(stdout);  // so we know where it is at the start of each run
    output = NULL;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
    outScreen.setOutput(stdout);  // flushes and clears archive file if any
    output = NULL;
}
