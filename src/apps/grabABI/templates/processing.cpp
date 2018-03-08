/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
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
bool displayFromCache(const SFString& cacheFileName, uint64_t& blockNum, void *dataPtr) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(dataPtr);
    CBlock block;

    // If there is no cache, there's nothing to display
    if (!fileExists(cacheFileName))
        return true; // return true if we want to continue

    uint64_t orig = blockNum, lastBlock = 0;

    if (visitor->cache.Lock(cacheFileName, binaryReadOnly, LOCK_NOWAIT)) {

        while (!visitor->cache.Eof()) {

            uint64_t transID;
            uint32_t whichWatch;
            visitor->cache >> whichWatch;
            if (visitor->cache.Eof()) {
                visitor->closeIncomeStatement(block);
                visitor->cache.Release();
                blockNum++; // the next block to process
                return true;
            }
            visitor->cache >> blockNum;
            if (visitor->cache.Eof()) {
                visitor->closeIncomeStatement(block);
                visitor->cache.Release();
                blockNum++; // the next block to process
                return true;
            }
            visitor->cache >> transID;
            if (blockNum >= orig) {
                if (blockNum > lastBlock) {  // only re-read if it's a new block

                    if (!visitor->closeIncomeStatement(block)) {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        blockNum++; // the next block to process
                        return false; // return false since user hit 'quit' on debugger
                    }

                    // If we switched blocks, read the next block
                    if (!readOneBlock_fromBinary(block, getBinaryFilename(blockNum))) {
                        getBlock(block, blockNum);
                        writeToBinary(block, getBinaryFilename(blockNum));
                        if (!fileExists(getBinaryFilename(blockNum))) {
                            cerr << "Read of block " << blockNum << " failed. Quitting cache read\r\n";
                            visitor->cache.Release();
                            blockNum++; // the next block to process
                            return false;
                        }
                    }

                    if (!visitor->openIncomeStatement(block))  {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        blockNum++; // the next block to process
                        return false; // return false since user hit 'quit' on debugger
                    }

                    lastBlock = blockNum;
                    if (verbose) {
                        cerr << "fromCache: " << block.blockNumber << " " << dateFromTimeStamp(block.timestamp) << "\r";
                        cerr.flush();
                    }
                }

                // this will always be true, but we just protect ourselves here
                if (transID < block.transactions.getCount()) {
                    CTransaction *trans = &block.transactions[(uint32_t)transID];
                    trans->pBlock = &block;
                    if (visitor->watches[whichWatch].status != "disabled") {
                        if (visitor->trace_on || visitor->logs_on)
                            cout << bGreen << SFString('-',180) << "\r\n";
                        if (visitor->accounting_on || visitor->trace_on)
                            getTraces(trans->traces, trans->hash);
                        visitor->accountForExtTransaction(block, trans);
                        visitor->displayTrans(whichWatch,trans);
                        visitor->transStats.nDisplayed++;
                        if (visitor->debugger_on && !visitor->esc_hit) {
                            nodelay(stdscr, true);
                            int ch = getch();
                            visitor->esc_hit = (ch == 27 || ch == 'q');
                            if (ch == 27) // esc comes with an extra key
                                getch();
                            nodelay(stdscr, false);
                        }
                    }
                }
            }
        }

        // ignore return since we're done anway
        visitor->closeIncomeStatement(block);
        visitor->cache.Release();
        blockNum++; // the next block to process
    }
    return true;
}

//-----------------------------------------------------------------------
void loadWatches(const CToml& toml, CAccountWatchArray& array, const SFString& key, blknum_t& minny, blknum_t& maxxy) {

    minny = UINT32_MAX;
    maxxy = 0;

    SFString watchStr = toml.getConfigStr("watches", key, "");
    if (key == "list" && watchStr.empty()) {
        cout << "Empty list of watches. Quitting.\r\n";
        exit(0);
    }

    uint32_t cnt = 0;
    char *p = cleanUpJson((char *)watchStr.c_str());
    while (p && *p) {
        CAccountWatch watch;
        uint32_t nFields = 0;
        p = watch.parseJson(p, nFields);

        if (nFields) {
            // cleanup and report on errors
            bool okay = true;
            SFString msg;
            watch.id = cnt++;
            watch.color = convertColor(watch.color);
            watch.address = fixAddress(toLower(watch.address));
            watch.nodeBal = watch.getNodeBal(watch.firstBlock-1);
            if (!isAddress(watch.address)) {
                okay = false;
                msg = "invalid address " + watch.address;
            }
            if (watch.name.empty()) {
                if (!msg.empty())
                    msg += ", ";
                msg += "no name " + watch.name;
                okay = false;
            }
            if (okay) {
                minny = min(minny, watch.firstBlock);
                maxxy = max(maxxy, watch.lastBlock);
                array[array.getCount()] = watch;
            } else {
                cerr << msg << "\n";
            }
        }
    }
    return;
}

