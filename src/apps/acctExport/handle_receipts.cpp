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
    for (size_t i = 0; i < items.size() && (!freshen || (nExported < freshen_max)); i++) {
        const CAppearance_base* item = &items[i];
        if (shouldQuit() || item->blk >= ts_cnt)
            break;
        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = item->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
            if (item->blk != 0 && fileExists(txFilename)) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

            } else {
                if (item->blk == 0) {
                    address_t addr = prefundAddrMap[item->txid];
                    trans.loadTransAsPrefund(item->blk, item->txid, addr, prefundWeiMap[addr]);

                } else if (item->txid == 99997 || item->txid == 99999) {
                    trans.loadTransAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

                } else if (item->txid == 99998) {
                    uint64_t nUncles = getUncleCount(item->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, item->blk, u);
                        if (uncle.miner == blkRewardMap[item->blk]) {
                            trans.loadTransAsUncleReward(item->blk, uncle.blockNumber, uncle.miner);
                        }
                    }

                } else {
                    getTransaction(trans, item->blk, item->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

                // ... we don't write the data here since it will not be complete.
                // if (false) // (write_opt & CACHE_TXS) && !fileExists(txFilename))
                //    writeTransToBinary(trans, txFilename);
            }

            if (articulate)
                abis.articulateTransaction(&trans);
            nExported++;
            if (shouldDisplay) {
                cout << ((isJson() && !first) ? ", " : "");
                cout << trans.receipt.Format() << endl;
                first = false;
            }

            HIDE_FIELD(CFunction, "message");
            if (!isTestMode() && (isApiMode() || !(i % 3))) {
                ostringstream os;
                os << "Exporting " << nExported << " ";
                os << plural(className) << "of ";
                os << nTransactions << " (max " << nProcessing << ") txs for address " << monitors[0].address;
                LOG_INFO(os.str() + "\r");
            }
        }
    }

    if (!isTestMode())
        LOG_PROGRESS1("Reported", (first_record + nExported), nTransactions,
                      " receipts for address " + monitors[0].address + "\r");

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    reportNeighbors();

    EXIT_NOMSG(true);
}
