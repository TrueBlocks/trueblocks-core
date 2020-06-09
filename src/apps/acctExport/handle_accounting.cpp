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

    string_q readFilename = getMonitorCach(accountForAddr);
    bool readFileExists = fileExists(readFilename);

    LOG8(string_q(120,'+'));
    LOG8("readFile: ", readFilename, " (", fileSize(readFilename), ") readFileExists: ", readFileExists);

    if (readFileExists && fileSize(readFilename) > 0) {
        // If the cache already exists, we read it into memory...
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(readFilename, modeReadOnly, LOCK_WAIT)) {
            archive >> nCacheItemsRead;
            archive >> lastStatement.blockNum;
            for (blknum_t i = 0; i < nCacheItemsRead; i++) {
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
                if (true) {  // inRange(i, first_record, (first_record + max_records - 1))) {

                    // Display the transaction...
                    abis.articulateTransaction(&trans);
                    if (!first)
                        cout << ", ";
                    cout << trans.Format() << endl;
                    first = false;
                    nExported++;
                    if (!isTestMode())
                        LOG_INFO("Reading ", (i + 1), " of ", nTransactions, " cache records (max ", nProcessing,
                                 ").          \r");
                }
            }
            archive.Release();

        } else if (readFileExists) {
            EXIT_FAIL("Could not open file " + readFilename + " or file size is zero. Quitting...");
        }
    }

    // At this point, either the file was empty or we've displayed all transactions in the file. Remember this...
    nCacheItemsWritten = nCacheItemsRead;

    // We open the file in staging to protect it from interruption...
    LOG8(string_q(120,'-'));

    string_q stagingFilename = getMonitorCach(accountForAddr);
    bool stagingFileExists = false;
    if (readFileExists) {
        copyFile(readFilename, stagingFilename);
        stagingFileExists = fileExists(stagingFilename);
    }

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(stagingFilename, stagingFileExists ? modeReadWrite : modeWriteCreate, LOCK_WAIT))
        EXIT_FAIL("Could not open file " + stagingFilename + ". Quitting...");

    if (!stagingFileExists) {
        // If the file did not yet exist, we need to save some space of the counts, so write those and flush...
        lockSection(true);
        archive.Seek(0, SEEK_SET);
        archive << 0;  // set it to zero until we can get it right
        archive << lastStatement.blockNum;
        archive.flush();
        lockSection(false);

    } else {
        // Otherwise go to the end of the file.
        archive.Seek(0, SEEK_END);
        lastStatement.endBal = lastStatement.endBalCalc = getBalanceAt(accountForAddr, lastStatement.blockNum);
    }

    LOG8(string_q(120,'='));

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

            nCacheItemsWritten++;

            lockSection(true);
            archive << trans;
            archive << trans.statements;
            archive << trans.traces;
            archive.Seek(0, SEEK_SET);
            archive << nCacheItemsWritten;
            archive << lastStatement.blockNum;
            archive.Seek(0, SEEK_END);
            archive.flush();
            monitors[0].writeLastExport(lastStatement.blockNum);
            lockSection(false);
            LOG8("wrote: ", trans.blockNumber, ".", trans.transactionIndex, ": ", nCacheItemsWritten);

            if (!first)
                cout << ", ";
            cout << trans.Format() << endl;
            first = false;
            nExported++;
            //LOG_INFO("Exporting ", nCacheItemsWritten, " of ", nTransactions, " records (max ", nProcessing,
            //         ").          \r");
        }
    }
    archive.Release();
    if (nCacheItemsWritten > nCacheItemsRead) {
        // If we wrote anything, copy the file to production
        lockSection(true);
        string_q prodFilename = getMonitorCach(accountForAddr);
        if (fileExists(stagingFilename))
            moveFile(stagingFilename, prodFilename);
        lockSection(false);
    }

    LOG8(string_q(120,'@'));

    cout << "]" << endl;

    reportOnNeighbors();

    LOG_INFO(string_q(120, ' '), "\r");
    EXIT_NOMSG(true);
}
