/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportReceipts(void) {
    ENTER("exportData");

    ASSERT(receipts);
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;
    bool isJson =
        (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);

    bool first = true;
    for (size_t i = 0;
         i < items.size() && !shouldQuit() && items[i].blk < ts_cnt && (!freshen || (nExported < freshen_max)); i++) {
        const CAppearance_base* item = &items[i];
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
                if (item->blk == 0 || (item->txid == 99997 || item->txid == 99999 || item->txid == 99998)) {
                    // Not a thing

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
            if (isJson && shouldDisplay && !first)
                cout << ", ";
            nExported++;
            if (shouldDisplay)
                cout << trans.receipt.Format() << endl;
            first = false;

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

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    reportNeighbors();
    EXIT_NOMSG(true);
}
