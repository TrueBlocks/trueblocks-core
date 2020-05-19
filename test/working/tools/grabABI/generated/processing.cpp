/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "parselib.h"
#include "debug.h"
#include "ncurses.h"

//-----------------------------------------------------------------------
bool COptions::displayFromCache(uint64_t st artBlock) {
    // Make sure we have a cache file...
    string_q cacheFileName = get MonitorPath(watches[0].address);
    if (!fileExists(cacheFileName))
        return false;

    // ...and that it's not locked...
    if (fileExists(cacheFileName + ".lck"))
        return usage(
            "The cache lock file is present. The program is either already "
            "running or it did not end cleanly the\n\tlast time it ran. "
            "Quit the already running program or, if it is not running, "
            "remove the lock\n\tfile: " +
            cacheFileName + ".lck'. Quitting...");

    if (export_on) {
        CTransaction t;
        cout << toLower(t.Format(
                    substitute(substitute(substitute(transFmt, "{IS", "{IS_"), "w:10:INPUT", "encoding"), "{", "{p:")))
             << "\n";
    }

    CBlock latest;
    getBlock_light(latest, "latest");

    CBlock block;
    uint64_t lastBlock = 0;
    uint64_t endBlock = NOPOS;

    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(cacheFileName, modeReadOnly, LOCK_NOWAIT)) {
        txCache.Seek((-1 * (long)(2 * sizeof(uint64_t))), SEEK_END);  // NOLINT
        txCache.Read(endBlock);
        txCache.Seek(0, SEEK_SET);
        if (json_on)
            cout << "[";

        while (!txCache.Eof()) {
            uint64_t transID = NOPOS, blockNum = NOPOS;
            txCache >> blockNum >> transID;
            if (blockNum != NOPOS && transID != NOPOS && blockNum >= startBlock) {
                if (blockNum > lastBlock) {
                    if (!closeIncomeStatement(block)) {
                        cerr << "Quitting debugger.\r\n";
                        txCache.Release();
                        return false;  // return false since user hit 'quit' on debugger
                    }

                    // If we switched blocks, try to read the next block...
                    block = CBlock();
                    bool ret = readBlockFromBinary(block, getBinaryCacheFilename(CT_BLOCKS, blockNum));
                    if (!ret) {
                        // ... if we failed to read from some reason, try to pick it up from the node
                        getBlock(block, blockNum);
                        block.finalized = is BlockFinal(block.timestamp, latest.timestamp, (6 0 * 4));
                        writeBlockToBinary(block, getBinaryCacheFilename(CT_BLOCKS, blockNum));
                        if (!fileExists(getBinaryCacheFilename(CT_BLOCKS, blockNum))) {
                            cerr << "Read of block " << blockNum << " failed. Quitting cache read\r\n";
                            txCache.Release();
                            return false;
                        }
                    }

                    if (!openIncomeStatement(block)) {
                        cerr << "Quitting debugger.\r\n";
                        txCache.Release();
                        return false;  // return false since user hit 'quit' on debugger
                    }

                    lastBlock = blockNum;
                }

                // this will always be true, but we just protect ourselves here
                if (transID < block.transactions.size()) {
                    CTransaction* trans = &block.transactions.at(transID);
                    trans->pBlock = &block;
                    // TODO(tjayrush) we only get traces on non-Ddos blocks. We need to handle this better
                    if ((json_on || accounting_on || trace_on) &&
                        (!ddosRange(block.blockNumber) || getTraceCount(trans->hash) < 250)) {
                        getTraces(trans->traces, trans->hash);
                    }

                    accountForExtTransaction(block, trans);
                    ostringstream os;
                    displayTrans(os, trans);
                    if (json_on && blockNum < endBlock)
                        os << ",";
                    os << endl;
                    os.flush();
                    cout << annotate(os.str());
                    cout.flush();
                    transStats.nDisplayed++;
                    if (debugger_on && !esc_hit) {
                        nodelay(stdscr, true);
                        int ch = getch();
                        esc_hit = (ch == 27 || ch == 'q');
                        if (ch == 27)  // esc comes with an extra key
                            getch();
                        nodelay(stdscr, false);
                    }
                }
            }
        }
        if (json_on)
            cout << "]";

        // Ignore the return since we're done anway
        closeIncomeStatement(block);
        txCache.Release();
    }
    return true;
}

//-----------------------------------------------------------------------
void COptions::renameItems(string_q& str, const C AccountWatchArray& watchArray) const {
    for (auto watch : watchArray) {
        if (json_on) {
            CStringArray fields = {"to", "from", "address", "contractAddress"};
            for (auto field : fields) {
                string_q target = "\"" + field + "\": \"" + watch.address + "\"";
                str = substitute(str, target, target + ", \"" + field + "Name\": \"" + watch.name + "\"");
            }
        } else {
            str = substitute(str, watch.address, watch.color + watch.displayName(false, true, true, 8) + cOff);
            str = substitute(str, "000000000000000000000000" + extract(watch.address, 2),
                             watch.color + "000000000000000000000000" + extract(watch.address, 2) + cWhite);
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

    blockStats.minWatchBlock = UINT32_MAX;
    blockStats.maxWatchBlock = 0;

    // Check the watches for validity
    for (uint32_t i = 0; i < watches.size(); i++) {
        C AccountWatch* watch = &watches.at(i);
        if (!isAddress(watch->address))
            return usage("Invalid watch address " + watch->address + "\n");

        if (watch->name.empty())
            return usage("Empty watch name " + watch->name + "\n");

        watch->curBalance = get NodeBal(watch->stateHistory, watch->address, watch->firstBlock - 1);

        blockStats.minWatchBlock = min(blockStats.minWatchBlock, watch->firstBlock);
        blockStats.maxWatchBlock = max(blockStats.maxWatchBlock, watch->lastBlock);
    }

    watches.push_back(C AccountWatch("Others", "Other Accts", 0, UINT32_MAX, bRed));
    return true;
}