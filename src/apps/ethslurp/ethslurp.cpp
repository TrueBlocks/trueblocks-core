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
#include <string>
#include "ethslurp.h"
#include "options.h"

extern bool sortReverseChron(const CTransaction& f1, const CTransaction& f2);
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
        string_q command = nextTokenClear(options.commandList, '\n');
        cerr << "Processing: " << command << "\n";

        if (!options.parseArguments(command))
            return 0;

        string_q message;

        // Setup the slurper
        if (!slurper.Initialize(options, message))
            return usage(message);

        // Slurp the address...
        if (!slurper.Slurp(options, message)) {
            if (startsWith(message, "No transactions")) {
                // Fix for issue #252.
                cerr << cRed << "\t" << message << cOff << "\n";
                return 0;
            } else {
                return usage(message);
            }
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
bool CSlurperApp::Initialize(COptions& options, string_q& message) {
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

    // If this is the first time we've ever run, make sure we have somewhere to write our data
    // Note: the config file will already exist by virtue of the installer
    if (!establishFolder(blockCachePath("slurps/"))) {
        message = "Unable to create data folders at " + blockCachePath("slurps/");
        return false;
    }

    // Note this may not return if user chooses to exit
    api.checkKey();

    // If we are told to get the address from the rerun address, and the
    // user hasn't supplied one, do so...
    string_q addr = options.addr;
    if (addr.empty() && options.rerun)
        addr = getGlobalConfig("ethslurp")->getConfigStr("settings", "rerun", "");

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

    // Save the address and name for later
    CToml toml(configPath("ethslurp.toml"));
    toml.setConfigStr("settings", "rerun", addr);
    toml.writeFile();

    // Load per address configurations if any
    string_q customConfig = blockCachePath("slurps/" + addr + ".toml");
    if (fileExists(customConfig) || !options.name.empty()) {
        CToml perAddr("");
        perAddr.setFilename(customConfig);
        if (fileExists(customConfig)) {
            perAddr.readFile(customConfig);
            ((CToml*)getGlobalConfig("ethslurp"))->mergeFile(&perAddr);  // NOLINT
        }
    }

    // Rerun will not reload the cache if it's already in memory
    if (theAccount.addr == addr)
        options.rerun = true;

    // If we're not re-running, we're slurping and we need an empty transaction list
    if (!options.rerun) {
        theAccount.transactions.clear();
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
bool CSlurperApp::Slurp(COptions& options, string_q& message) {
    double start = qbNow();

    // We always need the ABI
    loadABI(theAccount.abi, theAccount.addr);

    // Do we have the data for this address cached?
    string_q cacheFilename = blockCachePath("slurps/" + theAccount.addr + ".bin");
    bool needToRead = fileExists(cacheFilename);
    if (options.rerun && theAccount.transactions.size())
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
    uint64_t nSeconds = max((uint64_t)60, getGlobalConfig("ethslurp")->getConfigInt("settings", "update_freq", 300));
    if (uint64_t(now - fileTime) > nSeconds) {
        // This is how many records we currently have
        uint64_t origCount  = theAccount.transactions.size();
        uint64_t nNewBlocks = 0;

        if (!isTestMode())
            cerr << "\tSlurping new transactions from blockchain...\n";
        uint64_t nRead = 0;
        uint64_t nRequests = 0;

        // We already have 'page' pages, so start there.
        uint64_t page = max(theAccount.lastPage, (uint64_t)1);

        // Keep reading until we get less than a full page
        string_q contents;
        bool done = false;
// #define NO_INTERNET
#ifdef NO_INTERNET
        done = true;
#endif
        while (!done) {
            string_q url = string_q("https://api.etherscan.io/api?module=account&action=txlist&sort=asc") +
            "&address=" + theAccount.addr +
            "&page="    + toStringU(page) +
            "&offset="  + toStringU(options.pageSize) +
            "&apikey="  + api.getKey();

            // Grab a page of data from the web api
            string_q thisPage = urlToString(url);

            // See if it's good data, if not, bail
            message = nextTokenClear(thisPage, '[');
            if (!contains(message, "{\"status\":\"1\",\"message\":\"OK\"")) {
                if (contains(message, "{\"status\":\"0\",\"message\":\"No transactions found\",\"result\":"))
                    message = "No transactions were found for address '" + theAccount.addr + "'.";
                return options.fromFile;
            }
            contents += thisPage;

            uint64_t nRecords = countOf(thisPage, '}') - 1;
            nRead += nRecords;
            if (!isTestMode())
                cerr << "\tDownloaded " << nRead << " potentially new transactions.\r";

            // If we got a full page, there are more to come
            done = (nRecords < options.pageSize);
            if (!done)
                page++;

            // Etherscan.io doesn't want more than five pages per second, so sleep for a second
            if (++nRequests == 4) {
                usleep(1000000);
                nRequests = 0;
            }

            // Make sure we don't spin forever
            if (nRead >= options.maxTransactions)
                done = true;
        }

        size_t minBlock = 0, maxBlock = 0;
        findBlockRange(contents, minBlock, maxBlock);
#ifndef NO_INTERNET
        if (!isTestMode())
            cerr << "\n\tDownload contains blocks from " << minBlock << " to " << maxBlock << "\n";
#endif

        // Keep track of which last full page we've read
        theAccount.lastPage = page;
        theAccount.pageSize = options.pageSize;

        // pre allocate the array (probably wrong input here--reserve takes max needed size, not addition size needed)
        theAccount.transactions.reserve(nRead);

        int64_t lastBlock = 0;  // DO NOT CHANGE! MAKES A BUG IF YOU MAKE IT UNSIGNED NOLINT
        char *p = cleanUpJson((char *)(contents.c_str()));  // NOLINT
        while (p && *p) {
            CTransaction trans;
            size_t nFields = 0;
            p = trans.parseJson(p, nFields);
            if (nFields) {
                int64_t transBlock = (int64_t)trans.blockNumber;  // NOLINT
                if (transBlock > theAccount.lastBlock) {  // add the new transaction if it's in a new block
                    theAccount.transactions.push_back(trans);
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
        size_t newRecords = (theAccount.transactions.size() - origCount);
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

    double stop = qbNow();
    double timeSpent = stop-start;
    string_q timeRep = (isTestMode() ? "--time--" : double2Str(timeSpent));
    cout << "\tLoaded " << theAccount.transactions.size() << " total records in "
            << timeRep << " seconds\n";
    fflush(stderr);

    return (options.fromFile || theAccount.transactions.size() > 0);
}

//--------------------------------------------------------------------------------
bool CSlurperApp::Filter(COptions& options, string_q& message) {
    message = "";
    double start = qbNow();

    size_t nFuncFilts = 0;
    string_q funcFilts[20];
    string_q filtList = options.funcFilter;
    while (!filtList.empty())
        funcFilts[nFuncFilts++] = nextTokenClear(filtList, ',');

    theAccount.nVisible = 0;
    for (size_t i = 0 ; i < theAccount.transactions.size() ; i++) {
        CTransaction *trans = &theAccount.transactions.at(i);

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

extern bool isFunction(const CTransaction *trans, const string_q& func);
        if (!options.funcFilter.empty()) {
            bool show = false;
            for (uint64_t jj = 0 ; jj < nFuncFilts ; jj++)
                show = (show || isFunction(trans, funcFilts[jj]));
            trans->m_showing = show;
        }

        // We only apply this if another filter has not already hidden the transaction
        if (trans->m_showing && options.errFilt) {
            // The filter is either equal to '2' (errOnly) in which case
            // we show only errors. Otherwise, show only non-errors.
            bool isError = str_2_Int(trans->Format("[{ISERROR}]"));
            trans->m_showing = (options.errFilt == 2 ? isError : !isError);
        }

        theAccount.nVisible += trans->m_showing;
        int64_t nFiltered = int64_t(theAccount.nVisible + 1);  // NOLINT
        if (!(nFiltered % REP_INFREQ) && !isTestMode()) {
            cerr << "\t" << "Filtering..." << nFiltered << " records passed.\r";
            cerr.flush();
        }
    }

    double stop = qbNow();
    double timeSpent = stop-start;
    string_q timeRep = (isTestMode() ? "--time--" : double2Str(timeSpent));
    cerr << "\tFilter passed " << theAccount.nVisible << " visible records of "
            << theAccount.transactions.size() << " in " << timeRep << " seconds\n";
    cerr.flush();

    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSlurperApp::Display(COptions& options, string_q& message) {
    message = "";
    double start = qbNow();

    if (options.reverseSort) {
        sort(theAccount.transactions.begin(), theAccount.transactions.end(), sortReverseChron);
    }

    if (options.cache) {
        for (size_t i = 0 ; i < theAccount.transactions.size() ; i++) {
            const CTransaction *t = &theAccount.transactions[i];
            cout << t->Format("[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t" + toStringU(options.acct_id)) << "\n";
        }
    } else {

        theAccount.Format(cout, getFormatString(options, "file", false));
    }

    double stop = qbNow();
    double timeSpent = stop-start;
    string_q timeRep = (isTestMode() ? "--time--" : double2Str(timeSpent));
    cerr << "\tExported " << theAccount.nVisible << " records in " << timeRep << " seconds             \n\n";
    cerr.flush();

    return true;
}

//--------------------------------------------------------------------------------
string_q CSlurperApp::getFormatString(COptions& options, const string_q& which, bool ignoreBlank) {

    if (which == "file")
        buildDisplayStrings(options);

    string_q errMsg;

    string_q formatName = "fmt_" + options.exportFormat + "_" + which;
    string_q ret = getGlobalConfig("ethslurp")->getConfigStr("display", formatName, "");
    if (contains(ret, "file:")) {
        string_q file = substitute(ret, "file:", "");
        if (!fileExists(file))
            errMsg = string_q("Formatting file '") + file +
                        "' for display string '" + formatName + "' not found. Quiting...\n";
        else
            ret = asciiFileToString(file);

    } else if (contains(ret, "fmt_")) {  // it's referring to another format string...
        string_q newName = ret;
        ret = getGlobalConfig("ethslurp")->getConfigStr("display", newName, "");
        formatName += ":" + newName;
    }
    ret = substitute(substitute(ret, "\\n", "\n"), "\\t", "\t");

    // some sanity checks
    if (countOf(ret, '{') != countOf(ret, '}') || countOf(ret, '[') != countOf(ret, ']')) {
        errMsg = string_q("Mismatched brackets in display string '") + formatName + "': '" + ret + "'. Quiting...\n";

    } else if (ret.empty() && !ignoreBlank) {
const char *ERR_NO_DISPLAY_STR =
"You entered an empty display string with the --format (-f) option. The format string 'fmt_[{FMT}]_file'\n"
"  was not found in the configuration file (which is stored here: ~/.quickBlocks/quickBlocks.toml).\n"
"  Please see the full documentation for more information on display strings.";
        errMsg = usageStr(substitute(string_q(ERR_NO_DISPLAY_STR), "[{FMT}]", options.exportFormat));
    }

    if (!errMsg.empty()) {
        cerr << errMsg;
        exit(0);
    }

    return ret;
}

//---------------------------------------------------------------------------------------------------
bool buildFieldList(const CFieldData& fld, void *data) {
    string_q *s = (string_q*)data;  // NOLINT
    *s += (fld.getName() + "|");
    return true;
}

//---------------------------------------------------------------------------------------------------
void CSlurperApp::buildDisplayStrings(COptions& options) {
    // Set the default if it's not set
    if (options.exportFormat.empty())
        options.exportFormat = "json";

    // This is what we're really after...
    const string_q fmtForRecords = getFormatString(options, "record", false);
    ASSERT(!fmtForRecords.empty());

    // ...we may need this to build it.
    const string_q fmtForFields  = getFormatString(options, "field", !contains(fmtForRecords, "{FIELDS}"));
    ASSERT(!fmtForFields.empty());

    string_q defList = getGlobalConfig("ethslurp")->getConfigStr("display", "fmt_fieldList", "");
    string_q fieldList = getGlobalConfig("ethslurp")->getConfigStr("display",
                                                   "fmt_" + options.exportFormat + "_fieldList", defList);
    if (fieldList.empty())
        GETRUNTIME_CLASS(CTransaction)->forEveryField(buildFieldList, &fieldList);

    string_q origList = fieldList;

    theAccount.displayString = "";
    theAccount.header = "";
    while (!fieldList.empty()) {
        string_q fieldName = nextTokenClear(fieldList, '|');
        bool force = contains(fieldName, "*");
        replace(fieldName, "*", "");

        const CFieldData *field = GETRUNTIME_CLASS(CTransaction)->findField(fieldName);
        if (!field) {
            cerr << "Field '" << fieldName << "' not found in fieldList '" << origList << "'. Quitting...\n";
            exit(0);
        }
        if (field->isHidden() && force) ((CFieldData*)field)->setHidden(false);  // NOLINT
        if (!field->isHidden()) {
            string_q resolved = fieldName;
            if (options.exportFormat != "json")
                resolved = getGlobalConfig("ethslurp")->getConfigStr("field_str", fieldName, fieldName);
            theAccount.displayString +=
                substitute(
                substitute(fmtForFields, "{FIELD}", "{" + toUpper(resolved)+"}"), "{p:FIELD}", "{p:"+resolved+"}");
            theAccount.header +=
                substitute(
                substitute(
                substitute(
                substitute(fmtForFields, "{FIELD}", resolved), "[", ""), "]", ""), "<td ", "<th ");
        }
    }
    theAccount.displayString = trimWhitespace(theAccount.displayString);
    theAccount.header        = trimWhitespace(theAccount.header);

    theAccount.displayString = trim(substitute(fmtForRecords, "[{FIELDS}]", theAccount.displayString), '\t');
    if (options.exportFormat == "json") {
        // One little hack to make raw json more readable
        replaceReverse(theAccount.displayString, "}]\",", "}]\"\n");
        if (options.prettyPrint) {
            replaceAll(theAccount.displayString, "\"[{p:", "            \"[{p:");
            replaceAll(theAccount.displayString, "}]\",",  "}]\",\n");
            replaceAll(theAccount.displayString, "\":\"", "\": \"");
        }
    }
}

//--------------------------------------------------------------------------------
void findBlockRange(const string_q& json, size_t& minBlock, size_t& maxBlock) {
    string_q search = "\"blockNumber\":\"";
    size_t len = search.length();

    minBlock = 0;
    size_t first = json.find(search);
    if (first != string::npos) {
        string_q str = extract(json, first + len);
        minBlock = str_2_Uint(str);
    }

    string_q end = extract(json, json.rfind('{'));  // pull off the last transaction
    size_t last = end.find(search);
    if (last != string::npos) {
        string_q str = extract(end, last + len);
        maxBlock = str_2_Uint(str);
    }
}

//----------------------------------------------------------------------------------------
bool sortReverseChron(const CTransaction& t1, const CTransaction& t2) {
    if (t1.timestamp != t2.timestamp)
        return t1.timestamp > t2.timestamp;
    return sortTransactionsForWrite(t1, t2);
}

//---------------------------------------------------------------------------
string_q abis[1000][2];
size_t nAbis = 0;

//---------------------------------------------------------------------------
void clearAbis(void) {
    nAbis = 0;
}

//---------------------------------------------------------------------------
string_q findEncoding(const string_q& addr, CFunction& func) {
    if (!nAbis) {
        string_q contents = asciiFileToString(blockCachePath("abis/" + addr + ".abi"));
        while (!contents.empty()) {
            abis[nAbis][1] = nextTokenClear(contents, '\n');
            abis[nAbis][0] = nextTokenClear(abis[nAbis][1], '|');
            nAbis++;
        }
    }
    for (uint64_t i = 0 ; i < nAbis ; i++)
        if (abis[i][0] == func.name)
            return abis[i][1];
    return "";
}

//---------------------------------------------------------------------------
static bool getEncoding(const string_q& abiFilename, const string_q& addr, CFunction& func) {
    if (func.type != "function")
        return false;
    func.name     = nextTokenClear(func.name, '(');
    func.encoding = findEncoding(addr, func);
    return !func.encoding.empty();
}

//---------------------------------------------------------------------------
bool loadABI(CAbi& abi, const string_q& addr) {
    // Already loaded?
    if (abi.abiByName.size() && abi.abiByEncoding.size())
        return true;

    string_q abiFilename = blockCachePath("abis/" + addr + ".json");
    if (!fileExists(abiFilename))
        return false;

    cerr << "\tLoading abi file: " << abiFilename << "...\n";
    if (abi.loadABIFromFile(abiFilename)) {

        string_q abis1;

        // Get the encodings
        for (size_t i = 0 ; i < abi.abiByName.size() ; i++) {
            CFunction funct = abi.abiByName[i];
            getEncoding(abiFilename, addr, funct);
            abis1 += abi.abiByName[i].Format("[{NAME}]|[{ENCODING}]\n");
        }

        // We need to do both since they are copies
        for (size_t i = 0 ; i < abi.abiByEncoding.size() ; i++) {
            CFunction funct = abi.abiByEncoding[i];
            getEncoding(abiFilename, addr, funct);
        }

        if (!fileExists(blockCachePath("abis/" + addr + ".abi")) && !abis1.empty())
            stringToAsciiFile(blockCachePath("abis/" + addr + ".abi"), abis1);

        if (verbose) {
            for (size_t i = 0 ; i < abi.abiByName.size() ; i++) {
                const CFunction *f = &abi.abiByName[i];
                if (f->type == "function")
                    cerr << substitute(f->Format("[\"{NAME}|][{ENCODING}\"]"), "\n", " ") << "\n";
            }
        }
    }
    return abi.abiByName.size();
}

//---------------------------------------------------------------------------
bool isFunction(const CTransaction *trans, const string_q& func) {
    if (func == "none")
        return (trans->inputToFunction() == " ");
    return (trans->funcPtr ? trans->funcPtr->name == func : false);
}
