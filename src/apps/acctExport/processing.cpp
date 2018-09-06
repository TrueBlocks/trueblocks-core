/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool upgradeData(COptions& options) {

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

    uint64_t size = fileSize(fileName);
    if (!size)
        return usage("Old style cache file is present, but empty. Remove it to continue.");

    uint64_t *buffer = new uint64_t[size];
    bzero(buffer, size);

    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
        uint64_t bytesRead = txCache.Read(buffer, sizeof(uint64_t), size);
        cout << "bytesRead: " << bytesRead << "\n";
        txCache.Release();
    } else {
        return usage("Could not open old style cache file. Quiting...");
    }

    for (size_t i = 0 ; i < size / 2 ; i++)
        cout << buffer[i*2] << "\t" << buffer[(i*2)+1] << "\n";

    return true;
}

//-----------------------------------------------------------------------
bool exportData(COptions& options) {

    // Make sure we have a cache file...
    string_q cacheFileName = "./cache/" + options.watches[0].address + ".acct.bin";
    if (!fileExists(cacheFileName))
        return false;

    // ...and that it's not locked...
    if (fileExists(cacheFileName + ".lck"))
        return usage("The cache lock file is present. The program is either already "
                         "running or it did not end cleanly the\n\tlast time it ran. "
                         "Quit the already running program or, if it is not running, "
                         "remove the lock\n\tfile: " + cacheFileName + ".lck'. Quitting...");

    CBlock latest;
    getBlock(latest, "latest");

    CBlock block;
    uint64_t lastBlock = 0;
    uint64_t endBlock = NOPOS;

    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(cacheFileName, binaryReadOnly, LOCK_NOWAIT)) {

        txCache.Seek( (-1 * (long)(2*sizeof(uint64_t))), SEEK_END);  // NOLINT
        txCache.Read(endBlock);
        txCache.Seek( 0, SEEK_SET);
        if (options.defaultFmt == "json")
            cout << "[";

        while (!txCache.Eof()) {

            uint64_t transID = NOPOS, blockNum = NOPOS;
            txCache >> blockNum >> transID;
            if (blockNum != NOPOS && transID != NOPOS && blockNum >= options.theStartBlock) {

                if (blockNum > lastBlock) {

                    if (!options.closeIncomeStatement(block)) {
                        cerr << "Quitting debugger.\n";
                        txCache.Release();
                        return false; // return false since user hit 'quit' on debugger
                    }

                    // If we switched blocks, try to read the next block...
                    block = CBlock();
                    bool ret = readBlockFromBinary(block, getBinaryFilename(blockNum));
                    if (!ret) {
                        // ... if we failed to read from some reason, try to pick it up from the node
                        getBlock(block, blockNum);
                        block.finalized = isBlockFinal(block.timestamp, latest.timestamp, (60 * 4));
                        writeBlockToBinary(block, getBinaryFilename(blockNum));
                        if (!fileExists(getBinaryFilename(blockNum))) {
                            cerr << "Read of block " << blockNum << " failed. Quitting cache read\n";
                            txCache.Release();
                            return false;
                        }
                    }

                    if (!options.openIncomeStatement(block))  {
                        cerr << "Quitting debugger.\n";
                        txCache.Release();
                        return false; // return false since user hit 'quit' on debugger
                    }

                    lastBlock = blockNum;
                }

                // this will always be true, but we just protect ourselves here
                if (transID < block.transactions.size()) {
                    CTransaction *trans = &block.transactions.at(transID);
                    trans->pBlock = &block;
                    // TODO(tjayrush) we only get traces on non-Ddos blocks. We need to handle this better
                    if (((options.defaultFmt == "json") || options.accounting_on || options.trace_on) &&
                        (!ddosRange(block.blockNumber) || getTraceCount(trans->hash) < 250)) {
                        getTraces(trans->traces, trans->hash);
                    }

                    ostringstream os;
                    options.accountForExtTransaction(block, trans);
                    articulateTransaction(trans);
                    options.displayTransaction(os, trans);
                    if ((options.defaultFmt == "json") && blockNum < endBlock)
                            os << ",";
                    os << endl;
                    os.flush();
                    cout << options.annotate(os.str());
                    cout.flush();
                    options.tx_nDisplayed++;
                }
            }
        }
        if ((options.defaultFmt == "json"))
            cout << "]";

        // Ignore the return since we're done anway
        options.closeIncomeStatement(block);
        txCache.Release();
    }
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
        } else {
            str = substitute(str, watch.address, watch.color + watch.displayName(false,true,true,8));
            str = substitute(str, "000000000000000000000000" + extract(watch.address, 2),
                                watch.color + "000000000000000000000000" + extract(watch.address, 2));
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
    }

    watches.push_back(CAccountWatch("Others", "Other Accts", 0, UINT32_MAX, ""));
    return true;
}
