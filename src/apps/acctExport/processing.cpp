/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------
extern bool exportTransaction(COptions& options, const CAcctCacheItem *item);
inline bool isInRange(blknum_t ref, blknum_t start, blknum_t end) {
    return (start <= ref && end >= ref);
}
//-----------------------------------------------------------------------
bool exportData(COptions& options) {

    if (options.transFmt.empty())
        cout << "[";

    for (size_t index = 0 ; index < options.items.size() ; index++) {
        CAcctCacheItem *item = &options.items[index];
        if (isInRange(item->blockNum, options.blk_minWatchBlock, options.blk_maxWatchBlock))
            exportTransaction(options, item);
    }

    if (options.transFmt.empty())
        cout << "]";

    return true;
}

bool isInTransaction(CBlock& block, blknum_t tx_id, const address_t& addr);
//-----------------------------------------------------------------------
bool exportTransaction(COptions& options, const CAcctCacheItem *item) {

    static bool first = true;

    // visit the addresses in the block
    if (item->blockNum > options.curBlock.blockNumber) {
        options.curBlock = CBlock();
        getBlock(options.curBlock, item->blockNum);
        for (auto trans : options.curBlock.transactions)
            trans.pBlock = &options.curBlock;
    }

    bool useBloom = (getEnvStr("USEBLOOM") == "true");
    bool final = (getEnvStr("FINAL") == "true");
    if (final)
        useBloom = false;

    // This happens on every transaction, but it could happen on every block
    string_q bloomFilename = substitute(getBinaryFilename(item->blockNum), "/blocks/", "/blooms/");
    if (useBloom && fileExists(bloomFilename)) {
        CBloomArray blooms;
        CArchive bloomCache(READING_ARCHIVE);
        if (bloomCache.Lock(bloomFilename, binaryReadOnly, LOCK_NOWAIT)) {
            bloomCache >> blooms;
            bloomCache.Release();
        }

        // This happens on every transaction, but it could happen on every block
        bool foundOne = false;
        for (CAccountWatch& watch : options.watches) {
            if (!foundOne && watch.enabled) {
                for (auto bloom : blooms) {
                    if (isBloomHit(makeBloom(watch.address), bloom)) {
                        if (isInTransaction(options.curBlock, item->transIndex, watch.address)) {
                            foundOne = true;
                            break;
                        }
                    }
                }
            }
        }

        if (!foundOne) {
            cerr << item->blockNum << " " << item->transIndex << "\r";
            cerr.flush();
            return true;
        }
    }

    if (item->transIndex < options.curBlock.transactions.size()) {
        CTransaction *trans = &options.curBlock.transactions[item->transIndex];
        if (!options.transFmt.empty() || !IS_HIDDEN(CTransaction, "traces"))
            if (trans->traces.size() == 0)
                getTraces(trans->traces, trans->hash);

        bool found = false;
        for (size_t w = 0 ; w < options.watches.size() && !found ; w++) {
            CAccountWatch *watch = &options.watches[w];
            if (options.transFmt.empty() || contains(toLower(options.transFmt), "articulate"))
                articulateTransaction(watch->abi, trans);
            if (watch->enabled) {
                ostringstream os;
                if (options.transFmt.empty() && !first)
                    os << ",";
                options.displayTransaction(os, trans);
                os << endl;
                cout << options.annotate(substitute(os.str(),"++WATCH++",watch->address));
                cout.flush();
                found = true;
                first = false;
            }
        }
    } else {
        // TODO(tjayrush): This should never happen
        // cerr << "Invalid data at cache item: " << item->blockNum << "." << item->transIndex << "\n";
        // cerr.flush();
    }

    return true;
}

//-----------------------------------------------------------------------
void COptions::displayTransaction(ostream& os, const CTransaction *theTrans) const {

    string_q fmt = transFmt;
    if (expContext().asDollars) {
        replaceAll(fmt, "VALUE}",    "VALUE}++USD_V++");
        replaceAll(fmt, "ETHER}",    "ETHER}++USD_V++");
        replaceAll(fmt, "GASCOST}",  "GASCOST}++USD_GC++");
    }
    string_q transStr = theTrans->Format(fmt);
    if (contains(transStr, "++PRICE++")) {
        timestamp_t ts = str_2_Ts(theTrans->Format("[{TIMESTAMP}]"));
        transStr = substitute(transStr, "++PRICE++", asDollars(ts, weiPerEther));
    }
    os << transStr;

    return;
}

//-----------------------------------------------------------------------
void COptions::renameItems(string_q& str, const CAccountWatchArray& watchArray) const {
    for (auto watch : watchArray) {
        if (transFmt.empty()) {
            CStringArray fields = { "to", "from", "address", "contractAddress" };
            for (auto field : fields) {
                string_q target = "\"" + field + "\": \"" + watch.address + "\"";
                str = substitute(str, target, target + ", \"" + field + "Name\": \"" + watch.name + "\"");
            }
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

        watch->abi.loadABIFromFile(blockCachePath("abis/" + watch->address + ".json"));
        watch->abi.loadABIFromFile(blockCachePath("abis/0xTokenLib.json"));
        watch->abi.loadABIFromFile(blockCachePath("abis/0xWalletLib.json"));
        // We may as well articulate the named contracts while we're at it
        for (size_t n = 0 ; n < named.size() ; n++) {
            CAccountWatch *alt = &named.at(n);
            if (alt->enabled)
                watch->abi.loadABIFromFile(blockCachePath("abis/" + alt->address + ".json"));
        }
    }

    watches.push_back(CAccountWatch("Others", "Other Accts", 0, UINT32_MAX, ""));
    return true;
}

//-----------------------------------------------------------------------
bool loadData(COptions& options) {
    string_q fileName = "./cache/" + options.watches[0].address + ".acct.bin";

    // If we've already upgraded the file, we've deleted it and we're done...
    if (!fileExists(fileName))
        return true;

    // If the file is locked, we need to tell the user.
    if (fileExists(fileName + ".lck"))
        return usage("The cache lock file is present. The program is either already "
                     "running or it did not end cleanly the\n\tlast time it ran. "
                     "Quit the already running program or, if it is not running, "
                     "remove the lock\n\tfile: " + fileName + ".lck'. Quitting...");

    uint64_t nRecords = (fileSize(fileName) / (sizeof(uint64_t) * 2));
    if (!nRecords)
        return usage("Old style cache file is present, but empty. Remove it to continue.");

    uint64_t *buffer = new uint64_t[nRecords * 2];
    bzero(buffer, nRecords * 2);

    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
        txCache.Read(buffer, sizeof(uint64_t) * 2, nRecords);
        txCache.Release();
    } else {
        return usage("Could not open old style cache file. Quiting...");
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
    return (getTraceCount(trans->hash) > 250);
}

//-----------------------------------------------------------------------
bool isInTransaction(CBlock& block, blknum_t tx_id, const address_t& needle) {
    if (tx_id < block.transactions.size()) {
        CAddressAppearanceArray haystack;
        block.transactions[tx_id].forEveryAddress(visitAddrs, transFilter, &haystack);
        for (auto hay : haystack)
            if (hay.addr % needle)
                return true;
    }
    return false;
}
