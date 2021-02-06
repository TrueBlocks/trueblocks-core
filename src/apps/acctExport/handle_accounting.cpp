/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::handle_accounting(void) {
    ENTER("handle_accounting");

    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    CReconciliation lastStatement;
    if (apps.size() > 0 && first_record != 0)
        lastStatement.endBal = getBalanceAt(expContext().accountedFor, apps[0].blk - 1);

    bool first = true;
    blknum_t lastExported = scanRange.second;
    //    LOG_INFO(cRed, "lastExported: ", lastExported, " scan.start: ", scanRange.first, " scan.end: ",
    //    scanRange.second,
    //             " nApps: ", nApps, string_q(20, ' '), cOff);
    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < freshen_max)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= ts_cnt) {
            lastExported = app->blk - 1;
            break;
        }
        if (inRange((blknum_t)app->blk, scanRange.first, scanRange.second)) {
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

                // This data isn't stored, so we need to recreate it
                if (accounting || statements) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    CReconciliation nums;
                    nums.blockNumber = trans.blockNumber;
                    nums.transactionIndex = trans.transactionIndex;
                    nums.timestamp = trans.timestamp;
                    CStringArray corrections;
                    nums.reconcile(corrections, lastStatement, next, &trans);
                    // trans.reconciliations.clear();
                    trans.reconciliations.push_back(nums);
                    trans.statements.clear();
                    CReconciliationOutput st(nums);
                    trans.statements.push_back(st);
                    lastStatement = nums;
                }

                markNeighbors(trans);
                articulateAll(trans);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(nProcessed % FREQ)) {
                    blknum_t current = first_record + i;
                    // blknum_t goal = min(first_record + max_records, nApps);
                    ostringstream post;
                    post << " txs for address " << monitors[0].address;
                    post << " " << first_record << " " << nProcessed << " " << i << " " << nTransactions << "\r";
                    LOG_PROGRESS1("Reading ", current, nTransactions, post.str());
                }

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

                if (accounting || statements) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    CReconciliation nums;
                    nums.blockNumber = trans.blockNumber;
                    nums.transactionIndex = trans.transactionIndex;
                    nums.timestamp = trans.timestamp;
                    CStringArray corrections;
                    nums.reconcile(corrections, lastStatement, next, &trans);
                    trans.reconciliations.push_back(nums);
                    trans.statements.clear();
                    CReconciliationOutput st(nums);
                    trans.statements.push_back(st);
                    lastStatement = nums;
                }

                markNeighbors(trans);
                articulateAll(trans);

                if ((write_opt & CACHE_TXS))
                    writeTransToBinary(trans, txFilename);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(nProcessed % FREQ)) {
                    blknum_t current = first_record + i;
                    // blknum_t goal = min(first_record + max_records, nApp);
                    ostringstream post;
                    post << " txs for address " << monitors[0].address;
                    post << " " << first_record << " " << nProcessed << " " << i << " " << nTransactions << "\r";
                    LOG_PROGRESS1("Extract ", current, nTransactions, post.str());
                }
            }

            nProcessed++;
            if (shouldDisplay) {
                cout << ((isJson() && !first) ? ", " : "");
                cout << trans.Format() << endl;
                first = false;
            }
        }
    }

    if (!isTestMode()) {
        LOG_PROGRESS1((freshen ? "Updated" : "Reported"), (first_record + nProcessed), nTransactions,
                      " txs for address " + monitors[0].address);
    }

    // LOG_INFO("n: ", monitors.size(), " lastExported: ", lastExported);
    // getchar();
    for (auto monitor : monitors)
        monitor.updateLastExport(lastExported);

    reportNeighbors();

    EXIT_NOMSG(true);
}

bool isTokenFunc(const string_q& input) {
    CStringArray sigs = {
        "0x095ea7b3",
        "0xa9059cbb",
        "0x23b872dd",
    };
    string_q sig = input.substr(0, 10);
    return sig == sigs[0] || sig == sigs[1] || sig == sigs[2];
}
bool isTokenTopic(const topic_t& topic) {
    static const topic_t transferTopic =
        str_2_Topic("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
    static const topic_t approvalTopic =
        str_2_Topic("0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925");
    static const topic_t ensTransferTopic =
        str_2_Topic("0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266");
    return (topic == transferTopic || topic == ensTransferTopic || topic == approvalTopic);
}

void COptions::articulateAll(CTransaction& trans) {
    if (articulate) {
        abiMap[trans.to]++;
        if (abiMap[trans.to] == 1 || fileExists(getCachePath("abis/" + trans.to + ".json"))) {
            CStringArray unused;
            abi_spec.loadAbiFromEtherscan(trans.to, false, unused);
        }
        abi_spec.articulateTransaction(&trans);
        trans.hasToken |= isTokenFunc(trans.input);

        for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
            CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];
            trans.hasToken |= isTokenTopic(log->topics[0]);
            string_q str = log->Format();
            if (contains(str, bytesOnly)) {
                abiMap[log->address]++;
                if (abiMap[log->address] == 1 || fileExists(getCachePath("abis/" + log->address + ".json"))) {
                    CStringArray unused;
                    abi_spec.loadAbiFromEtherscan(log->address, false, unused);
                }
                abi_spec.articulateLog(log);
            }
        }

        for (size_t j = 0; j < trans.traces.size(); j++) {
            CTrace* trace = (CTrace*)&trans.traces[j];
            trans.hasToken |= isTokenFunc(trace->action.input);
            abiMap[trace->action.to]++;
            if (abiMap[trace->action.to] == 1 || fileExists(getCachePath("abis/" + trace->action.to + ".json"))) {
                CStringArray unused;
                abi_spec.loadAbiFromEtherscan(trace->action.to, false, unused);
            }
            abi_spec.articulateTrace(trace);
        }
    }
}
