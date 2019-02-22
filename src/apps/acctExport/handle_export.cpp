/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------
extern bool exportTransaction(COptions& options, const CAcctCacheItem *item, bool first);
extern bool checkBloom(COptions& options, const CAcctCacheItem *item);
extern bool isInTransaction(CTransaction *trans, const address_t& addr);
extern bool transFilter(const CTransaction *trans, void *data);

//---------------------------------------------------------------------------------------------
inline bool isInRange(blknum_t ref, blknum_t start, blknum_t end) {
    return (start <= ref && end >= ref);
}

//-----------------------------------------------------------------------
bool exportData(COptions& options) {

    string_q transFmt = expContext().fmtMap["trans_fmt"];
    string_q traceFmt = expContext().fmtMap["trace_fmt"];
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
        CAcctCacheItem *item = &options.items[index];
        if ((options.showProgress && !(index%3)) || index == options.items.size() -1) {
            cerr << "bn: " << item->blockNum << " tx: " << item->transIndex << "\r";
            cerr.flush();
        }
        if (isInRange(item->blockNum, options.blk_minWatchBlock, options.blk_maxWatchBlock)) {
            exportTransaction(options, item, first);
            first = false;
        }
    }

    if (transFmt.empty())
        cout << "]";

    return true;
}

//-----------------------------------------------------------------------
bool exportTransaction(COptions& options, const CAcctCacheItem *item, bool first) {

    string_q transFmt = expContext().fmtMap["trans_fmt"];
//    string_q traceFmt = expContext().fmtMap["trace_fmt"];
    // If we've found a new block...
    if (item->blockNum > options.curBlock.blockNumber) {

        // We want to note that we're at a new block (order matters)
        options.curBlock = CBlock();
        getBlock(options.curBlock, item->blockNum);
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
    if (item->transIndex < options.curBlock.transactions.size()) {

        // If we need the traces, get them before we scan through the watches. Only get them
        // if we don't already have them.
        CTransaction *trans = &options.curBlock.transactions[item->transIndex];
        if (options.shouldTrace(trans)) {
            getTraces(trans->traces, trans->hash);
            for (size_t i = 0 ; i < trans->traces.size() ; i++)
                trans->traces[i].pTrans = trans;
        }

        // We show a transaction only once even if it was involved from more than one watch perspective
        bool found = false;
        for (size_t w = 0 ; w < options.watches.size() && !found ; w++) {
            CAccountWatch *watch = &options.watches[w];

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
                    cerr << cTeal << "skipping: " << *item << cOff << "  \r";
            }
            HIDE_FIELD(CFunction, "message");
        }

    } else {
        // TODO(tjayrush): This should never happen
//        cerr << "Invalid data at cache item: " << item->blockNum << "." << item->transIndex << "\n";
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
    string_q transFmt = expContext().fmtMap["trans_fmt"];
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
    renameItems(ret, watches);
    renameItems(ret, named);
    return ret;
}

//-----------------------------------------------------------------------
bool COptions::loadWatches(const CToml& toml) {

    // okay if it's empty
    loadWatchList(toml, named, "named");

    // not okay if it's empty
    loadWatchList(toml, watches, "list");

    if (watches.size() == 0)
        return usage("Empty list of watches. Quitting...\n");

    blk_minWatchBlock = UINT32_MAX;
    blk_maxWatchBlock = 0;

    // Check the watches for validity
    for (size_t w = 0 ; w < watches.size() ; w++) {

        CAccountWatch *watch = &watches.at(w);
        if (!isAddress(watch->address))
            return usage("Invalid watch address " + watch->address + "\n");

        if (watch->name.empty())
            return usage("Empty watch name " + watch->name + "\n");

        watch->nodeBal = getNodeBal(watch->balanceHistory, watch->address, watch->firstBlock-1);

        blk_minWatchBlock = min(blk_minWatchBlock, watch->firstBlock);
        blk_maxWatchBlock = max(blk_maxWatchBlock, watch->lastBlock);

        watch->abi_spec.loadAbiByAddress(watch->address);
        watch->abi_spec.loadAbiKnown("all");
        // We may as well articulate the named contracts while we're at it
        for (size_t n = 0 ; n < named.size() ; n++) {
            CAccountWatch *alt = &named.at(n);
            if (alt->enabled)
                watch->abi_spec.loadAbiByAddress(alt->address);
        }
    }

    watches.push_back(CAccountWatch("Others", "Other Accts", 0, UINT32_MAX, ""));
    return true;
}

//-----------------------------------------------------------------------
bool loadData(COptions& options) {
    string_q fileName = getTransCachePath(options.watches[0].address);

    // If we've already upgraded the file, we've deleted it and we're done...
    if (!fileExists(fileName))
        return true;

    // If the file is locked, we need to tell the user.
    if (fileExists(fileName + ".lck"))
        return options.usage("The cache lock file is present. The program is either already "
                     "running or it did not end cleanly the\n\tlast time it ran. "
                     "Quit the already running program or, if it is not running, "
                     "remove the lock\n\tfile: " + fileName + ".lck'. Quitting...");

    uint64_t nRecords = (fileSize(fileName) / (sizeof(uint64_t) * 2));
    if (!nRecords)
        return options.usage("Old style cache file is present, but empty. Remove it to continue.");

    uint64_t *buffer = new uint64_t[nRecords * 2];
    bzero(buffer, nRecords * 2);

    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        txCache.Read(buffer, sizeof(uint64_t) * 2, nRecords);
        txCache.Release();
    } else {
        return options.usage("Could not open old style cache file. Quiting...");
    }

    for (size_t i = 0 ; i < nRecords ; i++)
        options.items.push_back(CAcctCacheItem(buffer[i*2], buffer[(i*2)+1]));

    return true;
}

//----------------------------------------------------------------
bool visitAddrs(const CAddressAppearance& item, void *data) {
    CAddressAppearanceArray *array = reinterpret_cast<CAddressAppearanceArray*>(data);
    if (!isZeroAddr(item.addr))
        array->push_back(item);
    return true;
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    if (!ddosRange(trans->blockNumber))
        return false;
    static string_q exclusions;
    if (exclusions.empty())
        exclusions = getGlobalConfig("blockScrape")->getConfigStr("exclusions", "list", "");
    if (contains(exclusions, trans->to))
        return true;
    if (contains(exclusions, trans->from))
        return true;
    uint64_t count = getTraceCount(trans->hash);
    return (count > 250);
}

//-----------------------------------------------------------------------
bool isInTransaction(CTransaction *trans, const address_t& needle) {
    CAddressAppearanceArray haystack;
    trans->forEveryAddress(visitAddrs, transFilter, &haystack);
    for (auto const& hay : haystack)
        if (hay.addr % needle)
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool checkBloom(COptions& options, const CAcctCacheItem *item) {

    // If we are not checking blooms, assume it's in the block
    if (!options.useBloom)
        return true;

    // If the bloom doesn't exist, assume it's in the block
    string_q bloomFilename = substitute(getBinaryFilename(item->blockNum), "/blocks/", "/blooms/");
    if (!fileExists(bloomFilename))
        return true;

    // Check to see if any of the enabled watched accounts are in the bloom
    CBloomArray blooms;
    CArchive bloomCache(READING_ARCHIVE);
    if (bloomCache.Lock(bloomFilename, modeReadOnly, LOCK_NOWAIT)) {
        bloomCache >> blooms;
        bloomCache.Release();
    }

    for (auto const& bloom : blooms)
        for (auto const& watch : options.watches)
            if (watch.enabled)
                if (isBloomHit(makeBloom(watch.address), bloom))
                    return true;

    return false;
}
