#define MAX_TXS NOPOS
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportData(void) {

    ENTER("exportData");
    if (exportFmt == JSON1)
        cout << "[";

    bool first = true;
    size_t nExported = 0;
    for (size_t i = 0 ; i < items.size() && !shouldQuit() && i < MAX_TXS && items[i].blk < ts_cnt ; i++) {
        const CAppearance_base *item = &items[i];
        if (inRange(item->blk, scanRange.first, scanRange.second)) {

            CBlock block; // do not move this from this scope
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
            if (fileExists(txFilename)) {
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[item->blk];

            } else {
                if (item->blk == 0) {
                    trans.transactionIndex = item->txid;
                    trans.loadAsPrefund(prefunds, prefundMap[item->txid]);

                } else if (item->txid == 99999) {
                    trans.loadAsBlockReward(item->blk, blkRewardMap[item->blk]);
                    trans.hash = uint_2_Hex(item->blk * 100000 + item->txid);
                    block.blockNumber = item->blk;

                } else {
                    getTransaction(trans, item->blk, item->txid);
                    getFullReceipt(&trans, true);
                }
                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[item->blk];
                if (writeTrxs && !fileExists(txFilename))
                    writeTransToBinary(trans, txFilename);
            }

            if (exportFmt == JSON1 && !first)
                cout << ", ";
            if (doLogs) {
                for (auto log : trans.receipt.logs)
                    cout << log.Format() << endl;
            } else {
                cout << trans.Format() << endl;
            }
            nExported++;
            first = false;
            HIDE_FIELD(CFunction, "message");
            if (isRedirected()) {  // we are not in --output mode
                cerr << "   " << i << " of " << items.size() << ": " << trans.hash << "\r";
                cerr.flush();
            } else {
                static size_t cnt = 0;
                if (!(++cnt % 71)) { // not reporting every tx is way faster
                    cerr << "   " << i << " of " << items.size() << ": " << trans.hash << "\r";
                    cerr.flush();
                }
            }
        }
    }
#if 0
/*
Do we really need to store both trace 0 and the transaction? I think not, so we store only trace 1 and on
writeNodeToBinary but do so as an array and start at item 1 not item 0
we need to articulate
We need to handle the non-txt case where we're exporting the fully articulated transaction, all it's traces, all its data
We want to name everything we can name
What about the header in trace mode -- different for each trace, trans, receipt, log
*/
#endif

    cerr << "Exported " << nExported << " of " << items.size() << " records." << string_q(45,' ') << "\n";
    cerr.flush();
    if (exportFmt == JSON1)
        cout << "]";

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (options.loadData())
            options.exportData();
    }

    acctlib_cleanup();
    return 0;
}

//----------------------------------------------------------------
bool isInTrace(CTrace& trace, const address_t& addr) {
    string_q justBytes = toLower(extract(addr, 2));
    if (contains(trace.action.from, justBytes)) return true;
    if (contains(trace.action.to, justBytes)) return true;
    if (contains(trace.action.refundAddress, justBytes)) return true;
    if (contains(trace.action.address, justBytes)) return true;
    if (contains(trace.result.address, justBytes)) return true;
    if (contains(trace.action.input, justBytes)) return true;
    if (contains(trace.result.output, justBytes)) return true;
    return false;
}

//----------------------------------------------------------------
bool excludeTrace(const CTransaction *trans, size_t maxTraces) {
    if (!ddosRange(trans->blockNumber))
        return false; // be careful, it's backwards
    static string_q exclusions;
    if (getGlobalConfig("blockScrape")->getConfigBool("exclusions", "enabled", false)) {
        if (exclusions.empty())
            exclusions = getGlobalConfig("blockScrape")->getConfigStr("exclusions", "list", "");
        if (contains(exclusions, trans->to))
            return true;
        if (contains(exclusions, trans->from))
            return true;
    }
    return (getTraceCount(trans->hash) > maxTraces);
}
