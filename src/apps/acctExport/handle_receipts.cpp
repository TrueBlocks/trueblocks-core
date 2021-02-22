/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::handle_receipts(void) {
    ENTER("handle_receipts");

    ASSERT(receipts);
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    SHOW_FIELD(CReceipt, "blockNumber");
    SHOW_FIELD(CReceipt, "transactionIndex");
    SHOW_FIELD(CReceipt, "isError");

    bool first = true;
    blknum_t lastExported = exportRange.second;
    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < freshen_max)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= ts_cnt) {
            lastExported = app->blk - 1;
            break;
        }

        // LOG_TEST("passes", inRange((blknum_t)app->blk, exportRange.first, exportRange.second) ? "true" : "false");
        if (inRange((blknum_t)app->blk, exportRange.first, exportRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = app->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, app->blk, app->txid);
            if (app->blk != 0 && fileExists(txFilename)) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(app->blk * 2) + 1];

            } else {
                if (app->blk == 0) {
                    address_t addr = prefundAddrMap[app->txid];
                    trans.loadTransAsPrefund(app->blk, app->txid, addr, prefundWeiMap[addr]);

                } else if (app->txid == 99997 || app->txid == 99999) {
                    trans.loadTransAsBlockReward(app->blk, app->txid, blkRewardMap[app->blk]);

                } else if (app->txid == 99998) {
                    uint64_t nUncles = getUncleCount(app->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, app->blk, u);
                        if (uncle.miner == blkRewardMap[app->blk]) {
                            trans.loadTransAsUncleReward(app->blk, uncle.blockNumber, uncle.miner);
                        }
                    }

                } else {
                    getTransaction(trans, app->blk, app->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(app->blk * 2) + 1];

                // ... we don't write the data here since it will not be complete.
                // if (false) // (cache_txs && !fileExists(txFilename))
                //    writeTransToBinary(trans, txFilename);
            }

            if (articulate)
                abi_spec.articulateTransaction(&trans);
            nProcessed++;
            if (shouldDisplay) {
                cout << ((isJson() && !first) ? ", " : "");
                cout << trans.receipt.Format() << endl;
                first = false;
            }

            HIDE_FIELD(CFunction, "message");
            if (!isTestMode() && (isApiMode() || !(i % 3))) {
                ostringstream os;
                os << "Exporting " << nProcessed << " ";
                os << plural(className) << " of ";
                os << nTransactions << " (max " << nProcessing << ") txs for address " << allMonitors[0].address;
                LOG_INFO(os.str() + "\r");
            }
        }
    }

    if (!isTestMode())
        LOG_PROGRESS1((freshen ? "Updated" : "Reported"), (first_record + nProcessed), nTransactions,
                      " receipts for address " + allMonitors[0].address + "\r");

    for (auto monitor : allMonitors)
        monitor.updateLastExport(lastExported);

    reportNeighbors();

    EXIT_NOMSG(true);
}
