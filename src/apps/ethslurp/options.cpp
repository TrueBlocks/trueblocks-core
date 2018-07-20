/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~addr",            "the address of the account or contract to slurp"),
    COption("-blocks:<range>",  "export records in block range (:0[:max])"),
    COption("-dates:<date>",    "export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]])"),
    COption("-fmt:<str>",       "pretty print, optionally add ':txt,' ':csv,' or ':html'"),
    COption("-income",          "include income transactions only"),
    COption("-expense",         "include expenditures only"),
    COption("@rerun",           "re-run the most recent slurp"),
    COption("@open",            "open the configuration file for editing"),
    COption("@max:<val>",       "maximum transactions to slurp (:250000)"),
    COption("@sleep:<val>",     "sleep for :x seconds"),
    COption("@func:<str>",      "display only --func:functionName records"),
    COption("@errFilt:<val>",   "display only non-error transactions or only errors with :errsOnly"),
    COption("@reverse",         "display results sorted in reverse chronological order (chronological by default)"),
    COption("@acct_id:<val>",   "for 'cache' mode, use this as the :acct_id for the cache (0 otherwise)"),
    COption("@cache",           "write the data to a local QuickBlocks cache"),
    COption("@name:<str>",      "name this address"),
    COption("",                 "Fetches data off the Ethereum blockchain for an arbitrary account or smart "
                                "contract. Optionally formats the output to your specification. Note: --income "
                                "and --expense are mutually exclusive as are --blocks and --dates.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        string_q orig = arg;
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

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {
            prettyPrint = true;
            exportFormat = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
            if (exportFormat.empty())
                return usage("Please provide a formatting option with " + orig + ". Quitting...");

        } else if (startsWith(arg, "--func:")) {
            funcFilter = substitute(arg, "--func:", "");
            if (funcFilter.empty())
                return usage("Please provide a function to filter on " + orig + ". Quitting...");

        } else if (startsWith(arg, "--errFilt")) {
            // weird, but 1 == no errors, 2 == errors only
            errFilt = true + contains(arg, ":errsOnly");

        } else if (startsWith(arg, "--reverse")) {
            reverseSort = true;

        } else if (startsWith(arg, "--acct_id:")) {
            arg = substitute(arg, "--acct_id:", "");
            acct_id = str_2_Uint(arg);

        } else if (startsWith(arg, "--cache")) {
            cache = true;

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--blocks:")) {

            if (firstDate != earliestDate || lastDate != latestDate)
                return usage("Specifiy either a date range or a block range, not both. Quitting...");

            string_q ret = blocks.parseBlockList(substitute(substitute(arg, "-b:", ""), "--blocks:", ""), latestBlock);
            if (contains(ret, "'stop' must be strictly larger than 'start'"))
                ret = "";
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
            // HACK ALERT: ethslurp used to provide inclusive block ranges (i.e. last was included in range).
            // New version does not include last in range, so we add one here to make it work the same.
            blocks.stop++;

        } else if (arg == "-d") {
            return usage("Invalid option -d. This option must include :firstDate or :first:lastDate range.");

        } else if (startsWith(arg, "-d:") || startsWith(arg, "--dates:")) {

            if (blocks.hasBlocks())
                return usage("Specifiy either a date range or a block range, not both. Quitting...");

            string_q lateStr = substitute(substitute(arg, "-d:", ""), "--dates:", "");
            string_q earlyStr = nextTokenClear(lateStr, ':');
            if (!earlyStr.empty() && !isNumeral(earlyStr))
                return usage("Invalid date: " + orig + ". Quitting...");
            if (!lateStr.empty() && !isNumeral(lateStr))
                return usage("Invalid date: " + orig + ". Quitting...");

            replaceAll(earlyStr, "-", "");
            replaceAll(lateStr,  "-", "");

            if (!earlyStr.empty() && earlyStr.length() != 8 && earlyStr.length() != 14)
                return usage("Option -d: Invalid date format for startDate. "
                                "Format must be either yyyymmdd or yyyymmddhhmmss.");

            if (!lateStr.empty() && lateStr.length() != 8 && lateStr.length() != 14)
                return usage("Option -d: Invalid date format for endDate. "
                                "Format must be either yyyymmdd or yyyymmddhhmmss.");

            firstDate = BOD(parseDate(earlyStr));
            lastDate  = EOD(parseDate(lateStr));
            if (lastDate == earliestDate)  // the default
                lastDate = latestDate;

            if (firstDate > lastDate) {
                return usage("lastDate (" + lastDate.Format(FMT_JSON) +
                             ") must be later than startDate (" + firstDate.Format(FMT_JSON) +
                             "). Quitting...");
            }

        } else if (arg == "-r" || arg == "--rerun") {
            rerun = true;

        } else if (startsWith(arg, "--sleep:")) {
            arg = substitute(arg, "--sleep:", "");
            if (arg.empty() || !isdigit(arg[0]))
                return usage("Sleep amount must be a numeral. Quitting...");
            useconds_t wait = (useconds_t)str_2_Uint(arg);
            if (wait) {
                cerr << "Sleeping " << wait << " seconds\n";
                usleep(wait * 1000000);
            }

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--max:")) {
            string_q val = substitute(substitute(arg, "-m:", ""), "--max:", "");
            if (val.empty() || !isdigit(val[0]))
                return usage("Please supply a value with the --max: option. Quitting...");
            maxTransactions = str_2_Uint(val);

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--name:")) {
            string_q val = substitute(substitute(arg, "-n:", ""), "--name:", "");
            if (val.empty())
                return usage("You must supply a name with the --name option. Quitting...");
            name = val;

        } else if (arg == "-o" || arg == "--open") {

            editFile(configPath("quickBlocks.toml"));
            editFile(configPath("ethslurp.toml"));
            exit(0);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (!isAddress(arg))
                 return usage(arg + " appears to be an invalid address. Valid addresses start with '0x' "
                              "and are 20 bytes (40 chars) long. Quitting...");
            addr = toAddress(arg);
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    prettyPrint = false;
    rerun = false;
    incomeOnly = false;
    expenseOnly = false;
    openFile = false;
    funcFilter = "";
    errFilt = false;
    reverseSort = false;
    blocks.Init();
    firstDate = earliestDate;
    lastDate = latestDate;
    maxTransactions = 250000;
    pageSize = 5000;
    exportFormat = "json";
    cache = false;
    acct_id = 0;
    addr = "";
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    UNHIDE_FIELD(CTransaction, "isError");
    UNHIDE_FIELD(CTransaction, "isInternal");
    UNHIDE_FIELD(CTransaction, "date");
    UNHIDE_FIELD(CTransaction, "ether");
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return str;

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "Portions of this software are Powered by Etherscan.io APIs.\n";
        return ret;
    }
    return str;
}
