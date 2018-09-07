/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

extern bool visitAddrs(const CAddressAppearance& item, void *data);

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

//-----------------------------------------------------------------------
bool exportData(COptions& options) {

    if (options.defaultFmt == "json")
        cout << "[";

    CBlock block;
    for (size_t index = 0 ; index < options.items.size() ; index++) {

        const CAcctCacheItem *item = &options.items[index];

        // visit the addresses in the block
        if (item->blockNum > block.blockNumber) {
            block = CBlock();
            getBlock(block, item->blockNum);
            for (auto trans : block.transactions)
                trans.pBlock = &block;

            block.forEveryAddress(visitAddrs, NULL, &options.addrsInBlock);
        }
    }

    if (options.defaultFmt == "json")
        cout << "]";

    return true;
}

//-----------------------------------------------------------------------
void COptions::renameItems(string_q& str, const CAccountWatchArray& watchArray) const {
    for (auto watch : watchArray) {
        if ((defaultFmt == "json")) {
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
    return substitute(substitute(ret, "+=+", "{"), "=+=", "}");
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
    for (uint32_t i = 0 ; i < watches.size() ; i++) {

        CAccountWatch *watch = &watches.at(i);
        if (!isAddress(watch->address))
            return usage("Invalid watch address " + watch->address + "\n");

        if (watch->name.empty())
            return usage("Empty watch name " + watch->name + "\n");

        watch->nodeBal = getNodeBal(watch->balanceHistory, watch->address, watch->firstBlock-1);

        blk_minWatchBlock = min(blk_minWatchBlock, watch->firstBlock);
        blk_maxWatchBlock = max(blk_maxWatchBlock, watch->lastBlock);

        string_q abiFile = blockCachePath("abis/" + watch->address + ".json");
        watch->abi.loadABIFromFile(abiFile);
        //        cout << fileExists(abiFile) << " : " << watch->abi << "\n";
    }

    watches.push_back(CAccountWatch("Others", "Other Accts", 0, UINT32_MAX, ""));
    return true;
}

//----------------------------------------------------------------
bool visitAddrs(const CAddressAppearance& item, void *data) {
    CAddressAppearanceArray *array = reinterpret_cast<CAddressAppearanceArray*>(data);
    if (!isZeroAddr(item.addr))
        array->push_back(item);
    return true;
}

