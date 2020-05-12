/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportData(void) {
    ENTER("exportData");

    bool shouldDisplay = !freshen;
    bool isJson =
        (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);

    if (isJson && shouldDisplay)
        cout << "[";

    bool first = true;
    for (size_t i = 0; i < items.size() && !shouldQuit() && items[i].blk < ts_cnt; i++) {
        const CAppearance_base* item = &items[i];
        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
            if (appearances) {
                if (isJson && shouldDisplay && !first)
                    cout << ", ";
                nExported++;
                if (shouldDisplay) {
                    CDisplayApp d(hackAppAddr, item->blk, item->txid);
                    cout << d.Format() << endl;
                }
                first = false;

            } else {
                CBlock block;  // do not move this from this scope
                block.blockNumber = item->blk;
                CTransaction trans;
                trans.pBlock = &block;

                string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
                if (item->blk != 0 && fileExists(txFilename)) {
                    readTransFromBinary(trans, txFilename);
                    trans.finishParse();
                    trans.pBlock = &block;
                    block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

                } else {
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
                    if ((write_opt & CACHE_TXS) && !fileExists(txFilename))
                        writeTransToBinary(trans, txFilename);
                }

                if (traces) {
                    // acctExport --traces
                    loadTraces(trans, item->blk, item->txid, (write_opt & CACHE_TRACES),
                               (skip_ddos && excludeTrace(&trans, max_traces)));
                    for (auto trace : trans.traces) {
                        bool isSuicide = trace.action.address != "";
                        bool isCreation = trace.result.address != "";

                        if (!isSuicide) {
                            if (grab_abis) {
                                abiMap[trace.action.to] = true;
                            } else {
                                if (!isTestMode() && isApiMode()) {
                                    qblocks::eLogger->setEndline('\r');
                                    LOG_INFO("\t\t\t\t\t\tGetting trace ", trans.blockNumber, ".",
                                             trans.transactionIndex, "-", trace.getValueByName("traceAddress"),
                                             string_q(50, ' '));
                                    qblocks::eLogger->setEndline('\n');
                                }
                                if (articulate)
                                    abis.articulateTrace(&trace);
                                if (isJson && shouldDisplay && !first)
                                    cout << ", ";
                                nExported++;
                                if (shouldDisplay)
                                    cout << trace.Format() << endl;
                                first = false;
                            }
                        }

                        if (isSuicide) {  // suicide
                            CTrace copy = trace;
                            copy.action.from = trace.action.address;
                            copy.action.to = trace.action.refundAddress;
                            copy.action.callType = "suicide";
                            copy.action.value = trace.action.balance;
                            copy.traceAddress.push_back("s");
                            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                            copy.action.input = "0x";
                            if (grab_abis) {
                                abiMap[trace.action.to] = true;
                            } else {
                                if (isJson && shouldDisplay && !first)
                                    cout << ", ";
                                nExported++;
                                if (shouldDisplay)
                                    cout << copy.Format() << endl;
                            }
                        }

                        if (isCreation) {  // contract creation
                            CTrace copy = trace;
                            copy.action.from = "0x0";
                            copy.action.to = trace.result.address;
                            copy.action.callType = "creation";
                            copy.action.value = trace.action.value;
                            if (copy.traceAddress.size() == 0)
                                copy.traceAddress.push_back("null");
                            copy.traceAddress.push_back("s");
                            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                            copy.action.input = trace.action.input;
                            if (grab_abis) {
                                abiMap[trace.action.to] = true;

                            } else {
                                if (isJson && shouldDisplay && !first)
                                    cout << ", ";
                                nExported++;
                                if (shouldDisplay)
                                    cout << copy.Format() << endl;
                            }
                        }
                    }

                } else {
                    if (receipts) {
                        // acctExport --receipts
                        if (articulate)
                            abis.articulateTransaction(&trans);
                        if (isJson && shouldDisplay && !first)
                            cout << ", ";
                        nExported++;
                        if (shouldDisplay)
                            cout << trans.receipt.Format() << endl;
                        first = false;

                    } else if (logs) {
                        // acctExport --logs
                        for (auto log : trans.receipt.logs) {
                            if (!emitter || log.address == monitors[0].address) {
                                if (isJson && shouldDisplay && !first)
                                    cout << ", ";
                                if (articulate)
                                    abis.articulateLog(&log);
                                nExported++;
                                if (shouldDisplay)
                                    cout << log.Format() << endl;
                                first = false;
                            }
                        }

                    } else {
                        toNameExistsMap[trans.to]++;
                        fromNameExistsMap[trans.from]++;
                        // we only articulate the transaction if we're JSON
                        if (isJson && articulate)
                            abis.articulateTransaction(&trans);
                        if (isJson && shouldDisplay && !first)
                            cout << ", ";
                        nExported++;
                        if (shouldDisplay)
                            cout << trans.Format() << endl;
                        first = false;
                    }
                }

                HIDE_FIELD(CFunction, "message");
                static size_t cnt = 0;
                if (!(++cnt % 11) || isRedirected() || (freshen && !(cnt % 3)))
                    LOG_INFO(className, ": ", i, " of ", items.size(), " (", trans.blockNumber, ".",
                             trans.transactionIndex, ")      ", "\r");
            }
        }
    }

    if (!isTestMode() && shouldDisplay)
        LOG_INFO(string_q(120, ' '), "\r");

    if (grab_abis) {
        // acctExport --grab_abis (downloads and writes the ABIs for all the traces to disc)
        for (pair<address_t, bool> item : abiMap) {
            if (isContractAt(item.first, latestBlock)) {
                CAbi unused;
                loadAbiAndCache(unused, item.first, false, errors);
                cerr << "ABI for " << item.first << " ";
                cerr << (fileExists(getCachePath("abis/" + item.first + ".json")) ? "cached" : "not cached") << endl;
                if (isApiMode())
                    errors.clear();
            }
        }
    }

    if (isJson && shouldDisplay)
        cout << "]";
    if (isTestMode())
        cout << endl;

    if (occurrence != NOPOS) {
        ostringstream os;
        os << ", \"addresses\": []\n";
        expContext().fmtMap["meta"] = expContext().fmtMap["meta"] + os.str();

    } else {
        addr_name_map_t fromAndToMap;
        for (auto addr : fromNameExistsMap)
            fromAndToMap[addr.first] = 1L;
        for (auto addr : toNameExistsMap)
            if (fromAndToMap[addr.first] == 1L)
                fromAndToMap[addr.first] = 2L;

        CNameStatsArray fromAndToUnnamed;
        CNameStatsArray fromAndToNamed;
        for (auto addr : fromAndToMap) {
            CAccountName acct;
            acct.address = addr.first;
            getNamedAccount(acct, addr.first);
            if (acct.name.empty()) {
                CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
                fromAndToUnnamed.push_back(stats);
            } else {
                CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
                fromAndToNamed.push_back(stats);
            }
        }

        {
            sort(fromAndToNamed.begin(), fromAndToNamed.end());
            ostringstream os;
            bool frst = true;
            os << ", \"namedFromAndTo\": {";
            for (auto stats : fromAndToNamed) {
                if (fromAndToMap[stats.address] == 2) {
                    if (!frst)
                        os << ",";
                    os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \""
                       << stats.name << "\", \"count\": " << stats.count << " }";
                    frst = false;
                }
            }
            os << "}\n";
            expContext().fmtMap["meta"] += os.str();
        }

        {
            sort(fromAndToUnnamed.begin(), fromAndToUnnamed.end());
            ostringstream os;
            os << ", \"unNamedFromAndTo\": {";
            bool frst = true;
            for (auto stats : fromAndToUnnamed) {
                if (fromAndToMap[stats.address] == 2) {
                    if (!frst)
                        os << ",";
                    os << "\"" << stats.address << "\": " << stats.count;
                    frst = false;
                }
            }
            os << "}";
            expContext().fmtMap["meta"] += os.str();
        }

        CNameStatsArray fromUnnamed;
        CNameStatsArray fromNamed;
        for (auto addr : fromNameExistsMap) {
            CAccountName acct;
            acct.address = addr.first;
            getNamedAccount(acct, addr.first);
            if (acct.name.empty()) {
                CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
                fromUnnamed.push_back(stats);
            } else {
                CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
                fromNamed.push_back(stats);
            }
        }

        {
            sort(fromNamed.begin(), fromNamed.end());
            ostringstream os;
            bool frst = true;
            os << ", \"namedFrom\": {";
            for (auto stats : fromNamed) {
                if (fromAndToMap[stats.address] != 2) {
                    if (!frst)
                        os << ",";
                    os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \""
                       << stats.name << "\", \"count\": " << stats.count << " }";
                    frst = false;
                }
            }
            os << "}\n";
            expContext().fmtMap["meta"] += os.str();
        }

        {
            sort(fromUnnamed.begin(), fromUnnamed.end());
            ostringstream os;
            os << ", \"unNamedFrom\": {";
            bool frst = true;
            for (auto stats : fromUnnamed) {
                if (fromAndToMap[stats.address] != 2) {
                    if (!frst)
                        os << ",";
                    os << "\"" << stats.address << "\": " << stats.count;
                    frst = false;
                }
            }
            os << "}";
            expContext().fmtMap["meta"] += os.str();
        }

        CNameStatsArray toUnnamed;
        CNameStatsArray toNamed;
        for (auto addr : toNameExistsMap) {
            CAccountName acct;
            acct.address = addr.first;
            getNamedAccount(acct, addr.first);
            if (isZeroAddr(acct.address)) {
                acct.tags = "Contract Creation";
                acct.name = "Contract Creation";
            }
            if (acct.name.empty()) {
                CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
                toUnnamed.push_back(stats);
            } else {
                CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
                toNamed.push_back(stats);
            }
        }

        {
            sort(toNamed.begin(), toNamed.end());
            ostringstream os;
            bool frst = true;
            os << ", \"namedTo\": {";
            for (auto stats : toNamed) {
                if (fromAndToMap[stats.address] != 2) {
                    if (!frst)
                        os << ",";
                    os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \""
                       << stats.name << "\", \"count\": " << stats.count << " }";
                    frst = false;
                }
            }
            os << "}\n";
            expContext().fmtMap["meta"] += os.str();
        }

        {
            sort(toUnnamed.begin(), toUnnamed.end());
            ostringstream os;
            os << ", \"unNamedTo\": {";
            bool frst = true;
            for (auto stats : toUnnamed) {
                if (fromAndToMap[stats.address] != 2) {
                    if (!frst)
                        os << ",";
                    os << "\"" << stats.address << "\": " << stats.count;
                    frst = false;
                }
            }
            os << "}";
            expContext().fmtMap["meta"] += os.str();
        }
    }

    for (auto watch : monitors)
        if (items.size() > 0)
            watch.writeLastExport(items[items.size() - 1].blk);

    EXIT_NOMSG(true);
}
