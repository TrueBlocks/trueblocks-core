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

    CReconciliationNumeric lastStatement;
    lastStatement.blockNum = NOPOS;

    bool first = true;
    cout << "[";

    uint64_t readCount = 0;
    string_q fn = getMonitorCach(relativeTo);
    if (isApiMode() && accounting && fileExists(fn)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(fn, modeReadOnly, LOCK_NOWAIT)) {
            archive >> readCount;
            archive >> lastStatement.blockNum;
            for (uint32_t i = 0; i < readCount; i++) {
                CTransaction trans;
                archive >> trans;
                archive >> trans.statements;
                archive >> trans.traces;
                scanRange.first = trans.blockNumber + 1;
                toNameExistsMap[trans.to]++;
                fromNameExistsMap[trans.from]++;
                // we only articulate the transaction if we're JSON
                if (articulate)
                    abis.articulateTransaction(&trans);
                if (!first)
                    cout << ",";
                cout << trans << endl;
                first = false;
            }
            archive.Release();
        }
    }

    bool exists = fileExists(fn);
    CArchive archive(WRITING_ARCHIVE);
    archive.Lock(fn, exists ? modeWriteAppend : modeWriteCreate, LOCK_WAIT);
    uint64_t nWritten = readCount;
    if (!exists) {
        archive.Seek(0, SEEK_SET);
        archive << nWritten;  // save some space
        archive << lastStatement.blockNum;
        archive.flush();
    } else {
        lastStatement.endBal = lastStatement.endBalCalc = getBalanceAt(relativeTo, lastStatement.blockNum);
    }

    for (size_t i = 0;
         i < items.size() && !shouldQuit() && items[i].blk < ts_cnt && (!freshen || (nExported < freshen_max)); i++) {
        const CAppearance_base* item = &items[i];
        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
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
            nums.reconcile(corrections, lastStatement, relativeTo, next, &trans);
            CIncomeStatement st(nums);
            trans.statements.push_back(st);
            if (archive.isOpen()) {
                archive << trans;
                archive << trans.statements;
                archive << trans.traces;
                archive.flush();
                nWritten++;
            }
            lastStatement = nums;
            toNameExistsMap[trans.to]++;
            fromNameExistsMap[trans.from]++;
            abis.articulateTransaction(&trans);
            if (!first)
                cout << ", ";
            nExported++;
            cout << trans.Format() << endl;
            first = false;
        }
    }

    archive.Release();
    if (nWritten != readCount) {
        bool exists2 = fileExists(fn);
        if (archive.Lock(fn, exists2 ? modeReadWrite : modeWriteCreate, LOCK_WAIT)) {
            archive.Seek(0, SEEK_SET);
            archive << nWritten;  // save some space
            archive << lastStatement.blockNum;
            archive.flush();
            archive.Release();
        }
    }

    if (!isTestMode())
        LOG_INFO(string_q(120, ' '), "\r");

    cout << "]" << endl;

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
                os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \"" << stats.name
                   << "\", \"count\": " << stats.count << " }";
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
                os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \"" << stats.name
                   << "\", \"count\": " << stats.count << " }";
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
                os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \"" << stats.name
                   << "\", \"count\": " << stats.count << " }";
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

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    EXIT_NOMSG(true);
}
