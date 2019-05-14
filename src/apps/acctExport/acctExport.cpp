/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------
bool isInTrace(CTrace& trace, const address_t& addr) {
    string_q justBytes = toLower(extract(addr, 2));
    if (contains(trace.action.from, justBytes)) return true;
    if (contains(trace.action.to, justBytes)) return true;
    if (contains(trace.action.refundAddress, justBytes)) return true;
    if (contains(trace.action.address, justBytes)) return true;
    if (contains(trace.result.address, justBytes)) return true;
    if (contains(trace.action.input, justBytes)) return true;
    if (contains(trace.result.output, justBytes)) return true;
    return false;
}

//----------------------------------------------------------------
bool excludeTrace(const CTransaction *trans, size_t maxTraces) {
    if (!ddosRange(trans->blockNumber))
        return false; // be careful, it's backwards
    static string_q exclusions;
    if (getGlobalConfig("blockScrape")->getConfigBool("exclusions", "enabled", false)) {
        if (exclusions.empty())
            exclusions = getGlobalConfig("blockScrape")->getConfigStr("exclusions", "list", "");
        if (contains(exclusions, trans->to))
            return true;
        if (contains(exclusions, trans->from))
            return true;
    }
    return (getTraceCount(trans->hash) > maxTraces);
}

//-----------------------------------------------------------------------
bool exportData(COptions& options) {

    ENTER("exportData");
    if (options.exportFmt != JSON1) {
        string_q transFmt = expContext().fmtMap["transaction_fmt"];
        string_q header = toLower(transFmt);
        for (uint32_t i = 0 ; i < 10 ; i++) {
            string_q str = "w:" + uint_2_Str(i);
            header = substitute(header, str, "");
        }
        for (auto const& ch : header)
            if (ch != '[' && ch != '{' && ch != '}' && ch != ']' && ch != ':')
                cout << ch;
        cout << endl;
    } else {
        cout << "[";
    }

    bool first = true;
    for (size_t i = 0 ; i < options.items.size() && !shouldQuit() ; i++) {
        const CAppearance_base *item = &options.items[i];

        CBlock block; // do not move this from this scope
        CTransaction trans;
        string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);

        if (fileExists(txFilename)) {
            readTransFromBinary(trans, txFilename);
            trans.finishParse();

        } else {
            getBlock(block, item->blk);
            if (item->txid < block.transactions.size()) {
                CTransaction *tx = &block.transactions[item->txid];
                tx->timestamp = block.timestamp;
                if (options.writeTrxs && !fileExists(txFilename))
                    writeTransToBinary(*tx, txFilename);
                trans = *tx;
                tx->pBlock = &block;
            } else {
                // silently skip over this
            }
        }

        if (true) { //}!(i%1)) {
            cerr << " " << i << " of " << options.items.size() << ": " << trans.hash << "\r";
            cerr.flush();
        }

        bool needsTrace = !IS_HIDDEN(CTransaction, "traces") &&
                            (getTraceCount(trans.hash) > 1) &&
                            !excludeTrace(&trans, options.maxTraces);

        if (needsTrace) {  // (THIS IS A BUG!  LOOK AT THE NEXT else if BELOW!)
                           // Even if we don't need traces, if we can read it quickly, we might as well use them (as we don't need dDos)...
            string_q markerFile = getBinaryCacheFilename(CT_TRACES, item->blk, item->txid, 1);
            if (options.skipDdos && fileExists(markerFile)) {
                // If the first trace for a transaction exists, the rest of them do too as well. We use whatever
                // traces we find in this folder
                size_t count = getTraceCount(trans.hash);
                for (txnum_t tc = 1 ; tc < count ; tc++) { // start at trace one since trace zero is same as transaction
                    string_q path = getBinaryCacheFilename(CT_TRACES, item->blk, item->txid, tc);
                    if (fileExists(path)) {
                        cout << path << endl;
                        CTrace trace;
                        readNodeFromBinary(trace, path);
                        trace.pTrans = &trans;
                        trans.traces.push_back(trace);
                    }
                }

            } else if (needsTrace) {  // else, only if we need the traces do we read them (avoid traces if we can)

                if (!options.skipDdos || !excludeTrace(&trans, options.maxTraces)) {

                    // Only get traces if the user has told us to
                    getTraces(trans.traces, trans.hash);
                    for (size_t tc = 1 ; tc < trans.traces.size() ; tc++) { // start at trace one since trace zero is same as transaction
                        trans.traces[tc].pTrans = &trans; // we need this so we don't dump
                        if (options.writeTraces) {
                            string_q traceFilename = getBinaryCacheFilename(CT_TRACES, item->blk, item->txid, tc);
                            writeNodeToBinary(trans.traces[tc], traceFilename);
                        }
                    }
                }
            }
        }

        // TODO(tjayrush): We turned this off by default because of performance reasons
        if (options.articulate) {
            for (size_t w = 0 ; w < options.monitors.size() ; w++) {
                options.monitors[w].abi_spec.articulateTransaction(&trans);
                HIDE_FIELD(CFunction, "message");
                if (!trans.articulatedTx.message.empty())
                    SHOW_FIELD(CFunction, "message");
            }
        }

        if (options.exportFmt == JSON1 && !first)
            cout << ", ";
        ostringstream os;
        os << trans.Format() << endl;
        string_q str = os.str();
        for (size_t w = 0 ; w < options.monitors.size() ; w++)
            replaceAll(str, options.monitors[w].address, options.monitors[w].color + options.monitors[w].address + cOff);
        for (size_t w = 0 ; w < options.named.size() ; w++) {
            CAccountWatch name = options.named[w];
            string_q newName = name.name.substr(0,10);
            if (!newName.empty()) {
                newName = name.color + newName + cOff + "-" + name.address.substr(0,name.address.length()-newName.length()-1);
            } else {
                newName = name.address;
            }
            replaceAll(str, options.named[w].address, newName);
        }
        cout << str;
        first = false;
        HIDE_FIELD(CFunction, "message");
    }

    cerr << "Exported " << options.items.size() << " of " << options.items.size() << " records.                                           \n";
    cerr.flush();

    if (options.exportFmt == JSON1)
        cout << "]";

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
bool loadMonitorData(CAppearanceArray_base& apps, const address_t& addr) {

    ENTER("loadMonitorData");
    string_q fn = getMonitorPath(addr);

    size_t nRecords = (fileSize(fn) / sizeof(CAppearance_base));
    ASSERT(nRecords);

    CAppearance_base *buffer = new CAppearance_base[nRecords];
    if (buffer) {
        bzero(buffer, nRecords * sizeof(CAppearance_base));

        CArchive txCache(READING_ARCHIVE);
        if (txCache.Lock(fn, modeReadOnly, LOCK_NOWAIT)) {
            txCache.Read(buffer, sizeof(CAppearance_base), nRecords);
            txCache.Release();
        } else {
            EXIT_FAIL("Could not open cache file.");
        }

        // Add to the apps which may be non-empty
        apps.reserve(apps.size() + nRecords);
        for (size_t i = 0 ; i < nRecords ; i++)
            apps.push_back(buffer[i]);

        delete [] buffer;

    } else {
        EXIT_FAIL("Could not allocate memory for address " + addr);

    }
    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
bool loadData(COptions& options) {

    ENTER("loadData");
    CAppearanceArray_base tmp;
    for (auto monitor : options.monitors) {
        if (!loadMonitorData(tmp, monitor.address))
            EXIT_FAIL("Could not load data.");
    }
    if (tmp.size() == 0)
        EXIT_MSG("Nothing to export.", false);

    sort(tmp.begin(), tmp.end());

    bool hasFuture = false;
    options.items.push_back(tmp[0]);
    for (auto item : tmp) {
        CAppearance_base *prev = &options.items[options.items.size() - 1];
        // TODO(tjayrush): I think this removes dups. Is it really necessary?
        if (item.blk != prev->blk || item.txid != prev->txid) {
            if (item.blk > options.lastAtClient)
                hasFuture = true;
            else
                options.items.push_back(item);
        }
    }
    LOG1("Items array: " +
         uint_2_Str(options.items.size()) + " - " +
         uint_2_Str(options.items.size() * sizeof(CAppearance_base)));

    if (hasFuture)
        LOG_WARN("Cache file contains blocks ahead of the chain. Some items will not be exported.");

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        loadData(options);
        exportData(options);
    }

    acctlib_cleanup();
    return 0;
}


#if 0
/*
 //---------------------------------------------------------------------------------------------
 extern bool exportTransaction(COptions& options, const CAppearance_base *item, bool first);
 extern bool checkBloom(COptions& options, const CAppearance_base *item);
 extern bool isInTransaction(CTransaction *trans, const address_t& addr);
 extern bool transFilter(const CTransaction *trans, void *data);

 //---------------------------------------------------------------------------------------------
 inline bool isInRange(blknum_t ref, blknum_t start, blknum_t end) {
 return (start <= ref && end >= ref);
 }

 //-----------------------------------------------------------------------
 bool exportData(COptions& options) {

 string_q transFmt = expContext().fmtMap["trans_ fmt"];
 string_q traceFmt = expContext().fmtMap["trace_ fmt"];
 string_q header = toLower(transFmt);
 for (uint32_t i = 0 ; i < 10 ; i++) {
 string_q str = "w:" + uint_2_Str(i);
 header = substitute(header, str, "");
 }
 for (auto const& ch : header)
 if (ch != '[' && ch != '{' && ch != '}' && ch != ']' && ch != ':')
 cout << ch;
 cout << endl;

 // We want to articulate if we're producing JSON or if we're producing text and the format includes the fields
 options.needsArt =
 (transFmt.empty() ||
 contains(toLower(transFmt), "articulate") ||
 contains(toLower(transFmt), "function") ||
 contains(toLower(transFmt), "events"));
 if (!options.needsArt) {
 options.needsArt =
 (traceFmt.empty() ||
 contains(toLower(traceFmt), "articulate") ||
 contains(toLower(traceFmt), "function") ||
 contains(toLower(traceFmt), "events"));
 }

 // We need traces if traces are not hidden, or we're doing JSON, or we're doing text and the format includes traces
 options.needsTrace = !IS_HIDDEN(CTransaction, "traces");
 if (options.needsTrace)
 options.needsTrace = (transFmt.empty() || contains(toLower(transFmt), "traces"));

 if (transFmt.empty())
 cout << "[";

 bool first = true;
 for (size_t index = 0 ; index < options.items.size() ; index++) {
 CAppearance_base *item = &options.items[index];
 if ((options.showProgress && !(index%3)) || index == options.items.size() -1) {
 cerr << "bn: " << item->blk << " tx: " << item->txid << "\r";
 cerr.flush();
 }
 exportTransaction(options, item, first);
 first = false;
 }

 if (transFmt.empty())
 cout << "]";

 return true;
 }

 //-----------------------------------------------------------------------
 bool exportTransaction(COptions& options, const CAppearance_base *item, bool first) {

 string_q transFmt = expContext().fmtMap["trans_ fmt"];
 //    string_q traceFmt = expContext().fmtMap["trace_fmt"];
 // If we've found a new block...
 if (item->blk > options.curBlock.blockNumber) {

 // We want to note that we're at a new block (order matters)
 options.curBlock = CBlock();
 getBlock(options.curBlock, item->blk);
 // And make sure to note which block is holding the transaction
 for (CTransaction& trans : options.curBlock.transactions)
 trans.pBlock = &options.curBlock;

 // We check the bloom (if we're told to), otherwise we assume the cache
 // is correct. Generally, we don't have to do this since acctScrape did
 // it already, but in some cases the user may wish to do it anyway.
 if (!checkBloom(options, item))
 return true;  // continue the scan
 }

 // TODO(tjayrush): This weird protection should not be needed, but for some reason, it is.
 if (item->txid < options.curBlock.transactions.size()) {

 // If we need the traces, get them before we scan through the watches. Only get them
 // if we don't already have them.
 CTransaction *trans = &options.curBlock.transactions[item->txid];
 if (options.shouldTrace(trans)) {
 getTraces(trans->traces, trans->hash);
 for (size_t i = 0 ; i < trans->traces.size() ; i++)
 trans->traces[i].pTrans = trans;
 }

 // We show a transaction only once even if it was involved from more than one watch perspective
 bool found = false;
 for (size_t w = 0 ; w < options.monitors.size() && !found ; w++) {
 CAccountWatch *watch = &options.monitors[w];

 // Note: we do this outside of the check for enablement becuase even disabled watches can
 // be useful when articulating data
 if (options.needsArt) {
 watch->abi_spec.articulateTransaction(trans);
 if (!trans->articulatedTx.message.empty())
 SHOW_FIELD(CFunction, "message");
 }

 if (watch->enabled && isInTransaction(trans, watch->address)) {

 // Only report once even if it appears for more than one watch
 found = true;

 // We need to use a string stream here because we want to annotate later
 ostringstream os;

 // We're exporting JSON, so we need commas
 if (transFmt.empty() && !first)
 os << ",";
 os << trans->Format(transFmt);
 os << endl;

 cout << options.annotate(substitute(os.str(),"++WATCH++",watch->address));
 cout.flush();

 } else {
 if (verbose)
 cerr << cTeal << "skipping: " << item->blk << "." << item->txid << cOff << "  \r";
 }
 HIDE_FIELD(CFunction, "message");
 }

 } else {
 // TODO(tjayrush): This should never happen
 //        cerr << "Invalid data at cache item: " << item->blk << "." << item->txid << "\n";
 //        cerr.flush();
 //        exit(0);
 }

 return true;
 }

 //-----------------------------------------------------------------------
 bool COptions::shouldTrace(const CTransaction *trans) const {
 if (!needsTrace)
 return false;

 if (trans->traces.size() > 0)  // we already have the traces
 return false;

 // Returns 'true' if we want the caller NOT to visit the traces of this transaction
 return !transFilter(trans, NULL);
 }

 //-----------------------------------------------------------------------
 void COptions::renameItems(string_q& str, const CAccountWatchArray& watchArray) const {
 string_q transFmt = expContext().fmtMap["trans_ fmt"];
 //    string_q traceFmt = expContext().fmtMap["trace_fmt"];

 for (auto const& watch : watchArray) {
 if (transFmt.empty()) {
 CStringArray fields = { "to", "from", "address", "contractAddress" };
 for (auto const& field : fields) {
 string_q target = "\"" + field + "\": \"" + watch.address + "\"";
 str = substitute(str, target, target + ", \"" + field + "Name\": \"" + watch.name + "\"");
 }
 } else {
 string_q rep = watch.color + watch.displayName(false, true) + cOff;
 str = substitute(str, watch.address, rep);
 }
 }
 }

 //-----------------------------------------------------------------------
 string_q COptions::annotate(const string_q& strIn) const {
 string_q ret = strIn;
 renameItems(ret, monitors);
 renameItems(ret, named);
 return ret;
 }

 //----------------------------------------------------------------
 bool visitAddrs(const CAppearance& item, void *data) {
 CAppearanceArray *array = reinterpret_cast<CAppearanceArray*>(data);
 if (!isZeroAddr(item.addr))
 array->push_back(item);
 return true;
 }
 //-----------------------------------------------------------------------
 bool isInTransaction(CTransaction *trans, const address_t& needle) {
 CAppearanceArray haystack;
 trans->forEveryAddress(visitAddrs, transFilter, &haystack);
 for (auto const& hay : haystack)
 if (hay.addr % needle)
 return true;
 return false;
 }

 //----------------------------------------------------------------
 // Return 'true' if we want the caller NOT to visit the traces of this transaction

 //-----------------------------------------------------------------------
 bool isInTransaction(CTransaction *trans, const address_t& needle) {
 CAppearanceArray haystack;
 trans->forEveryAddress(visitAddrs, transFilter, &haystack);
 for (auto const& hay : haystack)
 if (hay.addr % needle)
 return true;
 return false;
 }
 */
#endif
