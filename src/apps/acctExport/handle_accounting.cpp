//(write_opt & CACHE_TRACES)

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportAccounting(void) {
    ENTER("exportAccounting");

    ASSERT(isApiMode());
    ASSERT(!freshen && !appearances && !logs && !receipts && !traces && !balances && articulate);
    ASSERT(expContext().exportFmt == API1);
    ASSERT(monitors.size() == 1);

    if (items.size() == 0)
        EXIT_FAIL("Nothing to export. Quitting...");

    CReconciliationNumeric lastStatement;
    lastStatement.blockNum = NOPOS;

    bool first = true;
    cout << "[";

    nCacheItemsRead = 0;
    nExported = 0;

    string_q fn = getMonitorCach(accountForAddr);
    bool exists = fileExists(fn);
    if (exists && fileSize(fn) > 0) {
        // If the cache already exists, we read it into memory...
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(fn, modeReadOnly, LOCK_NOWAIT)) {
            archive >> nCacheItemsRead;
            archive >> lastStatement.blockNum;
            for (blknum_t i = 0; i < nCacheItemsRead ; i++) {
                // Read in the data...
                CTransaction trans;
                archive >> trans;
                archive >> trans.statements;
                archive >> trans.traces;

                // We've read in block 'n', so we can start at 'n + 1'...
                scanRange.first = trans.blockNumber + 1;
                // Remember which addresses we've seen...
                fromNameExistsMap[trans.from]++;
                toNameExistsMap[trans.to]++;

                // If we have the file, ignore this range test and just deliver the entire file
                if (true) { //inRange(i, first_record, (first_record + max_records - 1))) {

                    // Display the transaction...
                    abis.articulateTransaction(&trans);
                    if (!first)
                        cout << ", ";
                    cout << trans.Format() << endl;
                    first = false;
                    nExported++;
                    LOG_INFO("Reading ", (i + 1), " of ", nTransactions12, " cache records (max ", nProcessing, ").          \r");
                    //LOG4("trans ", trans.hash);

                }
            }
            archive.Release();

        } else if (exists) {
            EXIT_FAIL("Could not open file " + fn + " or file size is zero. Quitting...");

        }
    }

    // At this point, either the file was empty or we've displayed all transactions in the file. Remember this...
    nCacheItemsWritten = nCacheItemsRead;

    // We open the file in preparation for writing any new transactions...
    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(fn, exists ? modeReadWrite : modeWriteCreate, LOCK_NOWAIT))
        EXIT_FAIL("Could not open file " + fn + ". Quitting...");

    if (!exists) {
        // If the file did not yet exist, we need to save some space of the counts, so write those and flush...
        archive.Seek(0, SEEK_SET);
        archive << nCacheItemsWritten;  // may be zero
        archive << lastStatement.blockNum;
        archive.flush();

    } else {
        // Otherwise go to the end of the file.
        archive.Seek(0, SEEK_END);
        lastStatement.endBal = lastStatement.endBalCalc = getBalanceAt(accountForAddr, lastStatement.blockNum);
    }

    for (size_t i = 0; i < items.size() && !shouldQuit() && items[i].blk <= scanRange.second; i++) {

        const CAppearance_base* item = &items[i];
        bool include = inRange((blknum_t)item->blk, scanRange.first, scanRange.second);
        bool dClude = nCacheItemsWritten < max_records;
        bool tClude = items[i].blk < ts_cnt;

        if (include && dClude && tClude) {

            CBlock block;  // do not move this from this scope
            block.blockNumber = item->blk;
            CTransaction trans;
            trans.pBlock = &block;
            if (item->blk == 0) {
                address_t addr = prefundAddrMap[item->txid];
                trans.transactionIndex = item->txid;
                trans.loadAsPrefund(addr, prefundWeiMap[addr]);

            } else if (item->txid == 99997 || item->txid == 99998 || item->txid == 99999) {
                trans.loadAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

            } else {
                getTransaction(trans, item->blk, item->txid);
                getFullReceipt(&trans, true);
            }

            trans.pBlock = &block;
            trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

            blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;

            CReconciliationNumeric nums;
            nums.blockNum = trans.blockNumber;
            CStringArray corrections;
            nums.reconcile(corrections, lastStatement, accountForAddr, next, &trans);
            lastStatement = nums;
            CIncomeStatement st(nums);
            trans.statements.push_back(st);

            fromNameExistsMap[trans.from]++;
            toNameExistsMap[trans.to]++;

            abis.articulateTransaction(&trans);

            archive << trans;
            archive << trans.statements;
            archive << trans.traces;
            archive.flush();
            nCacheItemsWritten++;

            if (!first)
                cout << ", ";
            cout << trans.Format() << endl;
            first = false;
            nExported++;
            LOG_INFO("Exporting ", nCacheItemsWritten, " of ", nTransactions12, " records (max ", nProcessing, ").          \r");
            //LOG4("trans ", trans.hash);
        }
    }

    if (nCacheItemsWritten != nCacheItemsRead) {
        monitors[0].writeLastExport(lastStatement.blockNum);
        archive.Seek(0, SEEK_SET);
        archive << nCacheItemsWritten;  // save some space
        archive << lastStatement.blockNum;
        archive.Seek(0, SEEK_END);
        archive.flush();
    }
    archive.Release();

    cout << "]" << endl;

    reportOnNeighbors();

    LOG_INFO(string_q(120, ' '), "\r");
    EXIT_NOMSG(true);
}
