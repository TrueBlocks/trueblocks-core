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
    CParams("~addr",        "the address of the account or contract to slurp"),
    CParams("-archive:<str>","filename of output (stdout otherwise)"),
    CParams("-blocks:<range>","export records in block range (:0[:max])"),
    CParams("-dates:<date>","export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]])"),
    CParams("-name:<str>",  "name this address"),
    CParams("-rerun",       "re-run the most recent slurp"),
    CParams("-fmt:<str>",   "pretty print, optionally add ':txt,' ':csv,' or ':html'"),
    CParams("-income",      "include income transactions only"),
    CParams("-expense",     "include expenditures only"),
    CParams("-open",        "open the configuration file for editing"),
    CParams("-list",        "list previously slurped addresses in cache"),
    CParams("@--max",       "maximum transactions to slurp (:250000)"),
    CParams("@--sleep",     "sleep for :x seconds"),
    CParams("@--func",      "display only --func:functionName records"),
    CParams("@--errFilt",   "display only non-error transactions or :errsOnly"),
    CParams("@--reverse",   "display results sorted in reverse chronological order (chronological by default)"),
    CParams("@--acct_id",   "for 'cache' mode, use this as the :acct_id for the cache (0 otherwise)"),
    CParams("@--cache",     "write the data to a local QuickBlocks cache"),
    CParams("@--clear",     "clear all previously cached slurps"),
    CParams("",             "Fetches data off the Ethereum blockchain for an arbitrary account or smart contract. "
                            "Optionally formats the output to your specification.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-i" || arg == "-income") {
            if (expenseOnly)
                return usage("Only one of -income or -expense may be specified.");
            incomeOnly = true;

        } else if (arg == "-e" || arg == "-expense") {
            if (incomeOnly)
                return usage("Only one of -income or -expense may be specified.");
            expenseOnly = true;

        } else if (arg == "-f") {
            // -f by itself is json prettyPrint
            prettyPrint = true;
            exportFormat = "json";

        } else if (arg.startsWith("-f")) {
            // any other -f has the format attached  or is invalid
            prettyPrint = true;
            exportFormat = arg;
            arg = nextTokenClear(exportFormat,  ':');
            if (arg != "-f" && arg != "-fmt")
                return usage("Unknown parameter: " + arg);

        } else if (arg.startsWith("--func")) {
            funcFilter = arg.Substitute("--func:", EMPTY);

        } else if (arg.startsWith("--errFilt")) {
            errFilt = true + arg.Contains(":errOnly");

        } else if (arg.startsWith("--reverse")) {
            reverseSort = true;

        } else if (arg.startsWith("--acct_id:")) {
            arg.Replace("--acct_id:","");
            acct_id = toLong32u(arg);

        } else if (arg.startsWith("--cache")) {
            cache = true;

        } else if (arg == "-l" || arg == "-list") {
            uint32_t nFiles = 0;
            listFilesOrFolders(nFiles, NULL, cachePath("*.*"));
            if (nFiles) {
                SFString *files = new SFString[nFiles];
                listFilesOrFolders(nFiles, files, cachePath("*.*"));
                for (uint32_t i = 0 ; i < nFiles ; i++)
                    outScreen << files[i] << "\n";
                delete [] files;
            } else {
                outScreen << "No files found in cache folder '" << cachePath() << "'\n";
            }
            exit(0);

        } else if (arg == "-b") {
            return usage("Invalid option -b. This option must include :firstBlock or :first:lastBlock range.");

        } else if (arg.Left(3) == "-b:" || arg.Left(8) == "-blocks:") {
            arg = arg.Substitute("-b:", EMPTY).Substitute("-blocks:", EMPTY);
            firstBlock2Read = max(0U, toLong32u(arg));
            if (arg.Contains(":")) {
                nextTokenClear(arg, ':');
                lastBlock2Read = max(firstBlock2Read, toLong32u(arg));
            }

        } else if (arg == "-d") {
            return usage("Invalid option -d. This option must include :firstDate or :first:lastDate range.");

        } else if (arg.Left(3) == "-d:" || arg.Left(8) == "-dates:") {
            SFString lateStr = arg.Substitute("-d:", EMPTY).Substitute("-dates:", EMPTY);
            SFString earlyStr = nextTokenClear(lateStr, ':');

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

        } else if (arg == "-r" || arg == "-rerun") {
            rerun = true;

        } else if (arg.startsWith("--sleep:")) {
            nextTokenClear(arg, ':');
            uint32_t wait = toLong32u(arg);
            if (wait) {
                cerr << "Sleeping " << wait << " seconds\n";
                qbSleep(wait);
            }

        } else if (arg.startsWith("-m")) {
            SFString val = arg;
            arg = nextTokenClear(val, ':');
            if (arg != "-m" && arg != "-max")
                return usage("Unknown parameter: " + arg);
            maxTransactions = toLong32u(val);

        } else if (arg.startsWith("-n")) {
            SFString val = arg;
            arg = nextTokenClear(val, ':');
            if (arg != "-n" && arg != "-name")
                return usage("Unknown parameter: " + arg);
            name = val;

        } else if (arg == "-a" || arg.startsWith("-a:") || arg.startsWith("--archive:")) {
            SFString fileName = arg.Substitute("-a:", "").Substitute("--archive:", "");
            if (fileName == "-a") {
                // -a is acceptable but only if we get a -name (or we have only already)
                // checked during slurp since we don't have an address yet
                wantsArchive = true;
                archiveFile = "";

            } else {
                CFilename filename(fileName);
                if (!filename.getPath().startsWith('/'))
                    return usage("Archive file '" + arg + "' does not resolve to a full path. "
                                    "Use ./path/filename, ~/path/filename, or a fully qualified path.");
                archiveFile = filename.getFullPath();
                wantsArchive = true;
            }

        } else if (arg == "-o" || arg == "-open") {
            // open command stuff
            openFile = true;
            if (isTestMode()) {
                outScreen << "Testing only for open command:\n"
                            << asciiFileToString(configPath("quickBlocks.toml")) << "\n";
            } else {
                SFString cmd = "nano -I " + configPath("quickBlocks.toml");
                if (system(cmd.c_str())) {}  // do not remove. Silences compiler warnings
            }
            exit(0);

        } else if (arg == "-c" || arg == "-clear") {
            if (isTestMode()) {
                //removeFolder(cachePath());
                cerr << "Cached slurp files were NOT cleared!\n";
            } else {
                cerr << "Clearing the cache is not implemented. You may, if you wish, remove all\n";
                cerr << "files in " << cachePath() << " to acheive the same thing. If you\n";
                cerr << "do delete the cache, large contracts may take a very long time to re-generate.\n";
            }
            exit(1);

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            addr = fixAddress(arg);
            if (!isAddress(addr))
                 return usage(addr + " appears to be an invalid address. Valid addresses start with '0x' and are 20 bytes long. Quitting...");
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
    lastBlock2Read = ((uint32_t)-1);
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
