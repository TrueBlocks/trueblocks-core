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
#include "ethslurp.h"
#include "options.h"

extern int sortReverseChron(const void *rr1, const void *rr2);
//--------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {

    etherlib_init();

    // We keep only a single slurper. If the user is using the --file option and they
    // are reading the same account repeatedly, we only need to read the cache once.
    CSlurperApp slurper;
    COptions options;

    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        cerr << "Processing: " << command << "\n";

        if (!options.parseArguments(command))
            return 0;

        SFString message;

        // Setup the slurper
        if (!slurper.Initialize(options, message))
            return usage(message);

        // Slurp the address...
        if (!slurper.Slurp(options, message)) {
            if (message.startsWith("No transactions")) {
                // Fix for issue #252.
                cerr << cRed << "\t" << message << cOff << "\n";
                return 0;
            } else
                return usage(message);
        }

        // Apply the filters if any...
        if (!slurper.Filter(options, message))
            return usage(message);

        // Report on the address...
        if (!slurper.Display(options, message))
            return usage(message);
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------
bool CSlurperApp::Initialize(COptions& options, SFString& message) {
    // This allows us to spin through these classes' lists of fields without explicit display strings
    CFunction::registerClass();
    CParameter::registerClass();
    CAccount::registerClass();
    CTransaction::registerClass();
    CReceipt::registerClass();
    CLogEntry::registerClass();
    HIDE_FIELD(CTransaction, "toContract");
    HIDE_FIELD(CTransaction, "receipt");
    HIDE_FIELD(CTransaction, "traces");

    // If this is the first time we've ever run, build the toml file
    if (!establishFolders(toml)) {
        message = "Unable to create data folders at " + blockCachePath("slurps/");
        return false;
    }

    // Note this may not return if user chooses to exit
    api.checkKey(toml);

    // If we are told to get the address from the rerun address, and the
    // user hasn't supplied one, do so...
    SFString addr = options.addr;
    if (addr.empty() && options.rerun)
        addr = toml.getConfigStr("settings", "rerun", EMPTY);

    // Ethereum addresses are case insensitive. Force all address to lower case
    // to avoid mismatches with Mist browser for example
    addr = toLower(addr);

    // We can't run without an address...
    if (addr.empty()) {
        message = "You must supply an Ethereum account or contract address. ";
        if (!options.rerun)
            message += "Use -r flag to rerun the most recently slurped address.";
        return false;
    }

    // This seemingly out of place code dumps an error message if the fmt_X_file format
    // string is not in the config file. Don't remove it.
    getFormatString(options, "file", false);

    if (options.wantsArchive) {
        if (options.archiveFile.empty() && options.name.empty())
            return usage("-a and -n may not both be empty. Specify either an archive file or a name. Quitting...");

        SFString fn = (options.name.Contains("/") ? options.name : options.exportFormat + "/" + options.name) +
                        (options.name.Contains(".")?"":"." + options.exportFormat);
        CFilename filename(fn);
        if (options.archiveFile.empty())
            options.archiveFile = filename.getFullPath();
        ASSERT(options.output == NULL);
        options.output = fopen((const char*)options.archiveFile, asciiWriteCreate);
        if (!options.output)
            return usage("file '" + options.archiveFile + "' could not be opened. Quitting.");
        outScreen.setOutput(options.output);
    }

    // Save the address and name for later
    toml.setConfigStr("settings", "rerun", addr);
    toml.writeFile();

    // Load per address configurations if any
    SFString customConfig = blockCachePath("slurps/" + addr + ".toml");
    if (fileExists(customConfig) || !options.name.empty()) {
        CToml perAddr("");
        perAddr.setFilename(customConfig);
        if (fileExists(customConfig)) {
            perAddr.readFile(customConfig);
            toml.mergeFile(&perAddr);
        }
    }

    // Rerun will not reload the cache if it's already in memory
    if (theAccount.addr == addr)
        options.rerun = true;

    // If we're not re-running, we're slurping and we need an empty transaction list
    if (!options.rerun) {
        theAccount.transactions.Clear();
        theAccount = CAccount();
        clearAbis();
    }

    // We are ready to slurp
    theAccount.addr = addr;

    cerr << "\t" << "Slurping " << theAccount.addr << "\n";

    // Finished
    return true;
}

//--------------------------------------------------------------------------------
bool CSlurperApp::Slurp(COptions& options, SFString& message) {
    double start = qbNow();

    // We always need the ABI
    theAccount.abi.loadABI(theAccount.addr);

    // Do we have the data for this address cached?
    SFString cacheFilename = blockCachePath("slurps/" + theAccount.addr + ".bin");
    bool needToRead = fileExists(cacheFilename);
    if (options.rerun && theAccount.transactions.getCount())
        needToRead = false;
    if (needToRead) {
        // Once a transaction is on the blockchain, it will never change
        // therefore, we can store them in a binary cache. Here we read
        // from a previously stored cache.
        SFArchive archive(READING_ARCHIVE);
        if (!archive.Lock(cacheFilename, binaryReadOnly, LOCK_NOWAIT)) {
            message = "Could not open file: '" + cacheFilename + "'\n";
            return options.fromFile;
        }
        theAccount.Serialize(archive);
        archive.Close();
    }

    SFTime now = Now();
    SFTime fileTime = fileLastModifyDate(cacheFilename);

    // If the user tells us he/she wants to update the cache, or the cache
    // hasn't been updated in five minutes, then update it
    uint32_t nSeconds = (uint32_t)max((uint64_t)60, toml.getConfigInt("settings", "update_freq", 300));
    if ((now - fileTime) > nSeconds) {
        // This is how many records we currently have
        uint32_t origCount  = theAccount.transactions.getCount();
        uint32_t nNewBlocks = 0;

        if (!isTestMode())
            cerr << "\tSlurping new transactions from blockchain...\n";
        uint32_t nextRecord = origCount;
        uint32_t nRead = 0;
        uint32_t nRequests = 0;

        // We already have 'page' pages, so start there.
        uint32_t page = max((uint32_t)theAccount.lastPage, (uint32_t)1);

        // Keep reading until we get less than a full page
        SFString contents;
        bool done = false;
// #define NO_INTERNET
#ifdef NO_INTERNET
        done = true;
#endif
        while (!done) {
            SFString url = SFString("https://api.etherscan.io/api?module=account&action=txlist&sort=asc") +
            "&address=" + theAccount.addr +
            "&page="    + asString(page) +
            "&offset="  +
            asString(options.pageSize) +
            "&apikey="  + api.getKey();

            // Grab a page of data from the web api
            SFString thisPage = urlToString(url);

            // See if it's good data, if not, bail
            message = nextTokenClear(thisPage, '[');
            if (!message.Contains("{\"status\":\"1\",\"message\":\"OK\"")) {
                if (message.Contains("{\"status\":\"0\",\"message\":\"No transactions found\",\"result\":"))
                    message = "No transactions were found for address '" + theAccount.addr + "'.";
                return options.fromFile;
            }
            contents += thisPage;

            uint64_t nRecords = countOf('}', thisPage) - 1;
            nRead += nRecords;
            if (!isTestMode())
                cerr << "\tDownloaded " << nRead << " potentially new transactions.\r";

            // If we got a full page, there are more to come
            done = (nRecords < options.pageSize);
            if (!done)
                page++;

            // Etherscan.io doesn't want more than five pages per second, so sleep for a second
            if (++nRequests == 4) {
                qbSleep(1.0);
                nRequests = 0;
            }

            // Make sure we don't spin forever
            if (nRead >= options.maxTransactions)
                done = true;
        }

        uint32_t minBlock = 0, maxBlock = 0;
        findBlockRange(contents, minBlock, maxBlock);
#ifndef NO_INTERNET
        if (!isTestMode())
            cerr << "\n\tDownload contains blocks from " << minBlock << " to " << maxBlock << "\n";
#endif

        // Keep track of which last full page we've read
        theAccount.lastPage = page;
        theAccount.pageSize = options.pageSize;

        // pre allocate the array (probably wrong input here--Grow takes max needed size, not addition size needed)
        theAccount.transactions.Grow(nRead);

        int64_t lastBlock = 0;  // DO NOT CHANGE! MAKES A BUG IF YOU MAKE IT UNSIGNED NOLINT
        char *p = cleanUpJson((char *)(contents.c_str()));  // NOLINT
        while (p && *p) {
            CTransaction trans;
            uint32_t nFields = 0;
            p = trans.parseJson(p, nFields);
            if (nFields) {
                int64_t transBlock = (int64_t)trans.blockNumber;  // NOLINT
                if (transBlock > theAccount.lastBlock) {  // add the new transaction if it's in a new block
                    theAccount.transactions[nextRecord++] = trans;
                    lastBlock = transBlock;
                    if (!(++nNewBlocks % REP_FREQ) && !isTestMode()) {
                        cerr << "\tFound new transaction at block " << transBlock << ". Importing...\r";
                        cerr.flush();
                    }
                }
            }
        }
        if (!isTestMode() && nNewBlocks) {
            cerr << "\tFound new transaction at block " << lastBlock << ". Importing...\n";
            cerr.flush();
        }

        theAccount.lastBlock = max(theAccount.lastBlock, lastBlock);
        // Write the data if we got new data
        uint32_t newRecords = (theAccount.transactions.getCount() - origCount);
        if (newRecords) {
            if (!isTestMode())
                cerr << "\tWriting " << newRecords << " new records to cache\n";
            SFArchive archive(WRITING_ARCHIVE);
            if (archive.Lock(cacheFilename, binaryWriteCreate, LOCK_CREATE)) {
                theAccount.Serialize(archive);
                archive.Close();

            } else {
                message = "Could not open file: '" + cacheFilename + "'\n";
                return options.fromFile;
            }
        }
    }

    if (!isTestMode()) {
        double stop = qbNow();
        double timeSpent = stop-start;
        fprintf(stderr, "\tLoaded %d total records in %f seconds\n", theAccount.transactions.getCount(), timeSpent);
        fflush(stderr);
    }

    return (options.fromFile || theAccount.transactions.getCount() > 0);
}

//--------------------------------------------------------------------------------
bool CSlurperApp::Filter(COptions& options, SFString& message) {
    message = "";
    double start = qbNow();

    uint32_t nFuncFilts = 0;
    SFString funcFilts[20];
    SFString filtList = options.funcFilter;
    while (!filtList.empty())
        funcFilts[nFuncFilts++] = nextTokenClear(filtList, ',');

    theAccount.nVisible = 0;
    for (uint32_t i = 0 ; i < theAccount.transactions.getCount() ; i++) {
        CTransaction *trans = &theAccount.transactions[i];

        // Turn every transaction on and then turning them off if they match the filter.
        trans->m_showing = true;

        // The -blocks and -dates filters are mutually exclusive, -dates predominates.
        if (options.firstDate != earliestDate || options.lastDate != latestDate) {
            SFTime date = dateFromTimeStamp((timestamp_t)trans->timestamp);
            bool isVisible = (date >= options.firstDate && date <= options.lastDate);
            trans->m_showing = isVisible;

        } else if (options.blocks.hasBlocks()) {
            uint64_t bN = trans->blockNumber;
            trans->m_showing = options.blocks.isInRange(bN);
        }

        // The -incomeOnly and -expensesOnly filters are also mutually exclusive
        ASSERT(!(options.incomeOnly && options.expenseOnly));  // can't be both
        if (options.incomeOnly && fromAddress(trans->to) != theAccount.addr) {
            if (verbose)
                cerr << trans->Format("\tskipping expenditure [{HASH}]\n");
            trans->m_showing = false;

        } else if (options.expenseOnly && fromAddress(trans->from) != theAccount.addr) {
            if (verbose)
                cerr << trans->Format("\tskipping inflow [{HASH}]\n");
            trans->m_showing = false;
        }

        if (!options.funcFilter.empty()) {
            bool show = false;
            for (uint64_t jj = 0 ; jj < nFuncFilts ; jj++)
                show = (show || trans->isFunction(funcFilts[jj]));
            trans->m_showing = show;
        }

        // We only apply this if another filter has not already hidden the transaction
        if (trans->m_showing && options.errFilt) {
            // The filter is either equal to '2' (errOnly) in which case
            // we show only errors. Otherwise, show only non-errors.
            bool isError = toLong(trans->Format("[{ISERROR}]"));
            trans->m_showing = (options.errFilt == 2 ? isError : !isError);
        }

        theAccount.nVisible += trans->m_showing;
        int64_t nFiltered = int64_t(theAccount.nVisible + 1);  // NOLINT
        if (!(nFiltered % REP_INFREQ) && !isTestMode()) {
            cerr << "\t" << "Filtering..." << nFiltered << " records passed.\r";
            cerr.flush();
        }
    }

    if (!isTestMode()) {
        double stop = qbNow();
        double timeSpent = stop-start;
        cerr << "\tFilter passed " << theAccount.nVisible
                << " visible records of " << theAccount.transactions.getCount()
                << " in " << timeSpent << " seconds\n";
        cerr.flush();
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSlurperApp::Display(COptions& options, SFString& message) {
    message = "";
    double start = qbNow();

    if (options.reverseSort)
        theAccount.transactions.Sort(sortReverseChron);

    if (options.cache) {
        for (uint32_t i = 0 ; i < theAccount.transactions.getCount() ; i++) {
            const CTransaction *t = &theAccount.transactions[i];
            outScreen << t->Format("[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t" + asString(options.acct_id)) << "\n";
        }
    } else {

        theAccount.Format(outScreen, getFormatString(options, "file", false));
    }

    if (!isTestMode()) {
        double stop = qbNow();
        double timeSpent = stop-start;
        cerr << "\tExported " << theAccount.nVisible
                << " records in " << timeSpent << " seconds             \n\n";
        cerr.flush();
    }
    return true;
}

//--------------------------------------------------------------------------------
SFString CSlurperApp::getFormatString(COptions& options, const SFString& which, bool ignoreBlank) {

    if (which == "file")
        buildDisplayStrings(options);

    SFString errMsg;

    SFString formatName = "fmt_" + options.exportFormat + "_" + which;
    SFString ret = toml.getConfigStr("display", formatName, EMPTY);
    if (ret.Contains("file:")) {
        SFString file = ret.Substitute("file:", EMPTY);
        if (!fileExists(file))
            errMsg = SFString("Formatting file '") + file +
                        "' for display string '" + formatName + "' not found. Quiting...\n";
        else
            ret = asciiFileToString(file);

    } else if (ret.Contains("fmt_")) {  // it's referring to another format string...
        SFString newName = ret;
        ret = toml.getConfigStr("display", newName, EMPTY);
        formatName += ":" + newName;
    }
    ret = ret.Substitute("\\n", "\n").Substitute("\\t", "\t");

    // some sanity checks
    if (countOf('{', ret) != countOf('}', ret) || countOf('[', ret) != countOf(']', ret)) {
        errMsg = SFString("Mismatched brackets in display string '") + formatName + "': '" + ret + "'. Quiting...\n";

    } else if (ret.empty() && !ignoreBlank) {
const char *ERR_NO_DISPLAY_STR =
"You entered an empty display string with the --format (-f) option. The format string 'fmt_[{FMT}]_file`\n"
"  was not found in the configuration file (which is stored here: ~/.quickBlocks/quickBlocks.toml).\n"
"  Please see the full documentation for more information on display strings.";
        errMsg = usageStr(SFString(ERR_NO_DISPLAY_STR).Substitute("[{FMT}]", options.exportFormat));
    }

    if (!errMsg.empty()) {
        cerr << errMsg;
        exit(0);
    }

    return ret;
}

//---------------------------------------------------------------------------------------------------
void CSlurperApp::buildDisplayStrings(COptions& options) {
    // Set the default if it's not set
    if (options.exportFormat.empty())
        options.exportFormat = "json";

    // This is what we're really after...
    const SFString fmtForRecords = getFormatString(options, "record", false);
    ASSERT(!fmtForRecords.empty());

    // ...we may need this to build it.
    const SFString fmtForFields  = getFormatString(options, "field", !fmtForRecords.Contains("{FIELDS}"));
    ASSERT(!fmtForFields.empty());

    SFString defList = toml.getConfigStr("display", "fmt_fieldList", EMPTY);
    SFString fieldList = toml.getConfigStr("display", "fmt_"+options.exportFormat+"_fieldList", defList);
    if (fieldList.empty())
        fieldList = GETRUNTIME_CLASS(CTransaction)->listOfFields();
    SFString origList = fieldList;

    theAccount.displayString = EMPTY;
    theAccount.header = EMPTY;
    while (!fieldList.empty()) {
        SFString fieldName = nextTokenClear(fieldList, '|');
        bool force = fieldName.Contains("*");
        fieldName.Replace("*", EMPTY);

        const CFieldData *field = GETRUNTIME_CLASS(CTransaction)->FindField(fieldName);
        if (!field) {
            cerr << "Field '" << fieldName << "' not found in fieldList '" << origList << "'. Quitting...\n";
            exit(0);
        }
        if (field->isHidden() && force) ((CFieldData*)field)->setHidden(false);  // NOLINT
        if (!field->isHidden()) {
            SFString resolved = fieldName;
            if (options.exportFormat != "json")
                resolved = toml.getConfigStr("field_str", fieldName, fieldName);
            theAccount.displayString += fmtForFields
                .Substitute("{FIELD}", "{" + toUpper(resolved)+"}")
                .Substitute("{p:FIELD}", "{p:"+resolved+"}");
            theAccount.header += fmtForFields
                .Substitute("{FIELD}", resolved)
                .Substitute("[", EMPTY)
                .Substitute("]", EMPTY)
                .Substitute("<td ", "<th ");
        }
    }
    theAccount.displayString = StripAny(theAccount.displayString, "\t ");
    theAccount.header = StripAny(theAccount.header, "\t ");

    theAccount.displayString = Strip(fmtForRecords.Substitute("[{FIELDS}]", theAccount.displayString), '\t');
    theAccount.displayString.ReplaceAll("[{NAME}]", options.archiveFile);
    if (options.exportFormat == "json") {
        // One little hack to make raw json more readable
        theAccount.displayString.ReplaceReverse("}]\",", "}]\"\n");
        if (options.prettyPrint) {
            theAccount.displayString.ReplaceAll("\"[{p:", "            \"[{p:");
            theAccount.displayString.ReplaceAll("}]\",",  "}]\",\n");
            theAccount.displayString.ReplaceAll("\":\"", "\": \"");
        }
    }
}

//--------------------------------------------------------------------------------
void findBlockRange(const SFString& json, uint32_t& minBlock, uint32_t& maxBlock) {
    SFString search = "\"blockNumber\":\"";
    size_t len = search.length();

    minBlock = 0;
    int64_t first = (int64_t)json.find(search);
    if (first != (int64_t)NOPOS) {
        SFString str = json.substr(((size_t)first+len));
        minBlock = toLong32u(str);
    }

    SFString end = json.substr(json.ReverseFind('{'));  // pull off the last transaction
    size_t last = end.find(search);
    if (last != NOPOS) {
        SFString str = end.substr(last+len);
        maxBlock = toLong32u(str);
    }
}

//--------------------------------------------------------------------------------
// Make sure our data folder exist, if not establish it
bool establishFolders(CToml& toml) {

    SFString configFilename = configPath("quickBlocks.toml");
    toml.setFilename(configFilename);
    if (folderExists(blockCachePath("slurps/")) && fileExists(configFilename)) {
        toml.readFile(configFilename);
        return true;
    }

    // create the main folder
    mkdir((const char*)configPath(""), (mode_t)0755);
    if (!folderExists(configPath("")))
        return false;

    // create the folder for the data
    mkdir(blockCachePath("slurps/"), (mode_t)0755);
    if (!folderExists(blockCachePath("slurps/")))
        return false;

    toml.setConfigStr("settings", "api_key",          "<NOT_SET>");
    toml.setConfigStr("settings", "blockCachePath",   "<NOT_SET>");

    toml.setConfigStr("display", "fmt_fieldList",     "");
    toml.setConfigStr("display", "fmt_txt_file",      "[{HEADER}]\\n[{RECORDS}]");
    toml.setConfigStr("display", "fmt_txt_record",    "[{FIELDS}]\\n");
    toml.setConfigStr("display", "fmt_txt_field",     "\\t[{FIELD}]");
    toml.setConfigStr("display", "fmt_csv_file",      "[{HEADER}]\\n[{RECORDS}]");
    toml.setConfigStr("display", "fmt_csv_record",    "[{FIELDS}]\\n");
    toml.setConfigStr("display", "fmt_csv_field",     "[\"{FIELD}\"],");
    toml.setConfigStr("display", "fmt_html_file",     "<table>\\n[{HEADER}]\\n[{RECORDS}]</table>\\n");
    toml.setConfigStr("display", "fmt_html_record",   "\\t<tr>\\n[{FIELDS}]</tr>\\n");
    toml.setConfigStr("display", "fmt_html_field",    "\\t\\t<td>[{FIELD}]</td>\\n");
    toml.setConfigStr("display", "fmt_json_file",     "[{RECORDS}]\\n");
    toml.setConfigStr("display", "fmt_json_record",   "\\n        {\\n[{FIELDS}]        },");
    toml.setConfigStr("display", "fmt_json_field",    "\"[{p:FIELD}]\":\"[{FIELD}]\",");
    toml.setConfigStr("display", "fmt_custom_file",   "file:custom.txt");
    toml.setConfigStr("display", "fmt_custom_record", "fmt_txt_record");
    toml.setConfigStr("display", "fmt_custom_field",  "fmt_txt_field");

    toml.writeFile();
    return fileExists(toml.getFilename());
}

//----------------------------------------------------------------------------------------
int sortReverseChron(const void *rr1, const void *rr2) {
    const CTransaction *tr1 = reinterpret_cast<const CTransaction*>(rr1);
    const CTransaction *tr2 = reinterpret_cast<const CTransaction*>(rr2);

    int32_t ret = ((int32_t)tr2->timestamp - (int32_t)tr1->timestamp);
    if (ret != 0)
        return ret;
    return sortTransactionsForWrite(rr1, rr2);
}
