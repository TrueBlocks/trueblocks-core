/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

const string_q uncleBlockFile = "data/uncle_blocks.csv";
const string_q resultsFile = "data/results.csv";
//--------------------------------------------------------------
bool COptions::model_issuance(void) {
    if (!fileExists(uncleBlockFile))
        return usage("Cannot open list of uncle blocks. Run with --uncles option first.");

    CUintArray uncleBlocks;
    asciiFileToLines(uncleBlockFile, uncleBlocks);

    blknum_t prevBn = 0;
    for (auto uncleBn : uncleBlocks) {
        for (blknum_t bn = prevBn; bn < uncleBn; bn++) {
            if (true) {
                CReconciliation rec;
                rec.blockNum = bn;
                rec.timestamp = getBlockTimestamp(bn);
                if (bn == 0) {
                    cout << STR_HEADER_EXPORT << endl;
                    for (auto item : prefundWeiMap)
                        rec.minerBaseRewardIn = (rec.minerBaseRewardIn + item.second);
                } else {
                    rec.minerBaseRewardIn = getBlockReward2(bn);
                }
                if (bn != 0 && bn == prevBn) {
                    // This block has an uncle
                    rec.minerNephewRewardIn = getNephewReward(bn);
                    uint64_t count = getUncleCount(bn);
                    for (size_t i = 0; i < count; i++) {
                        CBlock uncle;
                        getUncle(uncle, bn, i);
                        rec.minerUncleRewardIn = (rec.minerUncleRewardIn + getUncleReward(bn, uncle.blockNumber));
                    }
                }
                cout << rec.Format(STR_DISPLAY_EXPORT) << endl;
            }
        }
        prevBn = uncleBn;
    }

    return true;
}

CStringArray header;
////--------------------------------------------------------------
//bool auditLine(const char* str, void* data) {
//    if (header.empty()) {
//        string_q headers = "blockNum,timestamp,month,day,minerBaseRewardIn,minerNephewRewardIn,minerIssuance,minerUncleRewardIn,issuance";
//        explode(header, headers, ',');
//        return true;
//    }
//
//    string_q line = str;
//    CReconciliation rec;
//    rec.parseCSV(header, line);
//    if (rec.blockNum == 0)
//        return true;
//    // END LAST TIME: 459690
//    if (rec.blockNum < 1145042) //(firstTransactionBlock - 10))
//        return true;
//
//    CTransaction trans;
//    trans.l oadTransAsBlockReward(rec.blockNum, 99999, block.miner);
//    expContext().account edFor = block.miner;
//    if (rec.reconcileIssuance(rec.blockNum, &trans)) {
//        cout << "Block " << rec.blockNum << " balances" << endl;
//    } else {
//        cerr << "Block " << rec.blockNum << " does not balances. Hit enter to coninue..." << endl;
//        cerr << rec << endl;
//        getchar();
//    }
//    return true;
//}

//--------------------------------------------------------------
int sortByFirstAppearance(const void *v1, const void *v2) {
    CAddressRecord_base *vv1 = (CAddressRecord_base*)v1;
    CAddressRecord_base *vv2 = (CAddressRecord_base*)v2;
    if (vv1->offset > vv2->offset)
        return 1;
    else if (vv1->offset < vv2->offset)
        return -1;
    return 0;
}

//--------------------------------------------------------------
int sortReverseByTxid(const void *v1, const void *v2) {
    CAppearance_base *vv1 = (CAppearance_base*)v1;
    CAppearance_base *vv2 = (CAppearance_base*)v2;
    if (vv2->txid > vv1->txid)
        return 1;
    else if (vv2->txid < vv1->txid)
        return -1;
    return 0;
}

//--------------------------------------------------------------
bool reconcileIssuance(const CAppearance& app) {
    if (app.bn == 0)
        return true;
    if (app.addr == "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead")
        return true;

    bigint_t minerReward, nephewReward, minerTxFee, uncleReward1, uncleReward2, netTx;
    uint64_t d1=0, d2=0;
    uint64_t uncleCount = getUncleCount(app.bn);
    if (app.tx != 99998) {
        minerReward = getBlockReward2(app.bn);
        nephewReward = getNephewReward(app.bn);
        minerTxFee = getTransFees(app.bn);
    }

    for (size_t i = 0 ; i < uncleCount ; i++) {
        CBlock uncle;
        getUncle(uncle, app.bn, i);
        if (uncle.miner == app.addr) {
            if (uncleReward1 == 0) {
                uncleReward1 = getUncleReward(app.bn, uncle.blockNumber);
                d1 = app.bn - uncle.blockNumber;
            } else {
                uncleReward2 = getUncleReward(app.bn, uncle.blockNumber);
                d2 = app.bn - uncle.blockNumber;
            }
        }
    }

    bigint_t begBal = getBalanceAt(app.addr, app.bn - 1);
    bigint_t endBal = getBalanceAt(app.addr, app.bn);
    bigint_t expected = begBal + minerReward + nephewReward + minerTxFee + uncleReward1 + uncleReward2;

    if (expected == endBal) {
        return true;
    }

    CBlock block;
    getBlock(block, app.bn);
    for (auto trans : block.transactions) {
        if (trans.from == app.addr) {
            netTx -= trans.isError ? 0 : trans.value;
            netTx -= str_2_BigInt(trans.getValueByName("gasCost"));
        }
        if (trans.to == app.addr) {
            netTx += trans.isError ? 0 : trans.value;
        }
        expected = begBal + minerReward + nephewReward + minerTxFee + uncleReward1 + uncleReward2 + netTx;
        cerr << expected << " : " << endBal << " : " << (expected - endBal) << string_q(90, ' ') << "\r"; cerr.flush();
        if (endBal == expected) return true;
    }

    if (endBal != expected) {
        cout << endl;
        cout << app << endl;
        cout << "hash:         " << block.blockNumber << endl;
        cout << "begBal:       " << padLeft(bni_2_Str(begBal), 20) << endl;
        cout << "endBal:       " << padLeft(bni_2_Str(endBal), 20) << endl;
        cout << "minerReward:  " << padLeft(bni_2_Str(minerReward), 20) << endl;
        cout << "nephewReward: " << padLeft(bni_2_Str(nephewReward), 20) << endl;
        cout << "minerTxFee:   " << padLeft(bni_2_Str(minerTxFee), 20) << endl;
        cout << "d1:           " << padLeft(uint_2_Str(d1), 20) << endl;
        cout << "uncleReward1: " << padLeft(bni_2_Str(uncleReward1), 20) << endl;
        cout << "d2:           " << padLeft(uint_2_Str(d2), 20) << endl;
        cout << "uncleReward2: " << padLeft(bni_2_Str(uncleReward2), 20) << endl;
        cout << "netTx:        " << padLeft(bni_2_Str(netTx), 20) << endl;
        cout << "expected:     " << padLeft(bni_2_Str(expected), 20) << endl;
        cout << "diff:         " << padLeft(bni_2_Str((expected - endBal)), 20) << endl;
    }

    return (endBal == expected);
}

//--------------------------------------------------------------
bool visitIndexChunk(CIndexArchive& chunk, void* data) {
    static bool skip = true;
    string_q fn = substitute(chunk.getFilename(), indexFolder_finalized, "");
    if (contains(fn, "010401728-010403680"))
        skip = false;
    if (skip) {
        cerr << "Skipping " << fn << endl;
        return true;
    }

    size_t nMiners = 0;
    size_t nUncles = 0;
    CAppearanceArray checkList;
    for (size_t i = 0 ; i < chunk.nAddrs ; i++) {
        CAddressRecord_base *rec = &chunk.addresses[i];
        CAppearance_base *start = &chunk.appearances[rec->offset];
        qsort(start, rec->cnt, sizeof(CAppearance_base), sortReverseByTxid);
        if (start[0].blk != 0 && (start[0].txid == 99997 || start[0].txid == 99998 || start[0].txid == 99999)) {
            CAppearance_base *appPtr = &chunk.appearances[rec->offset];
            CAppearance app;
            app.bn = appPtr->blk;
            app.tx = appPtr->txid;
            app.addr = bytes_2_Addr(rec->bytes);
            nMiners += (appPtr->txid == 99997 || appPtr->txid == 99999);
            nUncles += (appPtr->txid == 99998);
            checkList.push_back(app);
        }
    }

    cout << fn << " nMiners: " << nMiners << " nUncles: " << nUncles << string_q(120, ' ') << endl;
    for (auto app : checkList) {
        if (reconcileIssuance(app)) {
            cerr << fn << ": Block reward for  " << app.addr << " at block " << app.bn << " balances" << "    \r"; cerr.flush();
        } else {
            cout << fn << ": Block reward for  " << app.addr << " at block " << app.bn << bRed << " does not balance" << cOff << endl;
        }
    }
    return true;
}

//--------------------------------------------------------------
bool COptions::audit_issuance(void) {
    qblocks::forEveryIndexChunk(visitIndexChunk, NULL);
    return true;
}

//--------------------------------------------------------------
int sortByBlockNumber(const void *v1, const void *v2) {
    CAppearance_base *vv1 = (CAppearance_base*)v1;
    CAppearance_base *vv2 = (CAppearance_base*)v2;
    if (vv1->blk > vv2->blk)
        return 1;
    else if (vv1->blk < vv2->blk)
        return -1;
    return 0;
}

//--------------------------------------------------------------
bool listUncleBlocks(CIndexArchive& chunk, void *data) {
    qsort(chunk.appearances, chunk.nApps, sizeof(CAppearance_base), sortByBlockNumber);
    for (size_t i = 0 ; i < chunk.nApps ; i++) {
        CAppearance_base *start = &chunk.appearances[i];
        if (start->txid == 99998)
            cout << start[0].blk << endl;
    }
    return true;
}

//--------------------------------------------------------------
bool COptions::show_uncle_blocks(void) {
    qblocks::forEveryIndexChunk(listUncleBlocks, NULL);
    return true;
}

//--------------------------------------------------------------------------------
bool COptions::check_uncles(void) {
    if (!fileExists(uncleBlockFile))
        return usage("Cannot open list of uncle blocks. Run with --uncles option first.");

    CUintArray uncleBlocks;
    asciiFileToLines(uncleBlockFile, uncleBlocks);

    for (auto uncleBn : uncleBlocks) {
        uint64_t nUncles = getUncleCount(uncleBn);
        for (size_t index = 0 ; index < nUncles ; index++) {
            string_q uncleStr = getRawUncle(uncleBn, index);
            if (contains(uncleStr, "uncles:[]")) {
                cerr << "No uncles in uncle block " << uncleBn << "." << index << "\r"; cerr.flush();
            } else {
                cout << "Uncle block " << uncleBn << "." << index << " has children" << endl;
            }
            printf("");
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
string_q STR_DISPLAY_EXPORT =
"[{BLOCKNUM}],"\
"[{TIMESTAMP}],"\
"[{MONTH}],"\
"[{DAY}],"\
"[{MINERBASEREWARDIN}],"\
"[{MINERNEPHEWREWARDIN}],"\
"[{MINERISSUANCE}],"\
"[{MINERUNCLEREWARDIN}],"\
"[{ISSUANCE}]";


string_q STR_HEADER_EXPORT =
"blockNum,timestamp,month,day,minerBaseRewardIn,minerNephewRewardIn,minerIssuance,minerUncleRewardIn,issuance";

//--------------------------------------------------------------
class CAccumulator {
public:
    CReconciliation sum;
    period_t period;
    string_q fmt;
    blknum_t start, end;
    bool discrete;
    CAccumulator(void) {
        period = BY_NOTHING;
        discrete = false;
        start = 0;
        end = getBlockProgress(BP_CLIENT).client;
    }
    bool isSamePeriod(const CReconciliation& r) {
        if (isBlockSummary())
            return qblocks::isSamePeriod(period, sum.blockNum, r.blockNum);
        return qblocks::isSamePeriod(period, ts_2_Date(sum.timestamp), ts_2_Date(r.timestamp));
    }
    bool isDateSummary(void) const {
        return !isBlockSummary();
    }
    bool isBlockSummary(void) const {
        return (period == BY_1 || period == BY_10 || period == BY_100 || period == BY_1000 || period == BY_10000 || period == BY_100000 || period == BY_1000000);
    }
};

//--------------------------------------------------------------
bool visitLine(const char* str, void* data) {
    CAccumulator *acc = (CAccumulator*)data;

    if (header.empty()) {
        string_q headers = "blockNum,timestamp,month,day,minerBaseRewardIn,minerNephewRewardIn,minerIssuance,minerUncleRewardIn,issuance";
        explode(header, headers, ',');
        return true;
    }

    string_q line = str;
    CReconciliation rec;
    rec.parseCSV(header, line);
    if (!inRange(rec.blockNum, acc->start, acc->end))
        return rec.blockNum < acc->start;  // if it's not in range and not less than start, then we're done

    bool same = acc->isSamePeriod(rec);
    if (!same) {
        CReconciliation out(acc->sum);
        cout << out.Format(acc->fmt) << endl;
        if (acc->discrete) {
            CReconciliation reset;
            acc->sum = reset;
        }
    } else {
        if (!(rec.timestamp % 23)) { // skip
            cerr << "Processing " << rec.blockNum << ": " << ts_2_Date(rec.timestamp).Format(FMT_JSON) << "\r";
            cerr.flush();
        }
    }
    // accumulate...
    acc->sum = (acc->sum + rec);
    return true;
}

//--------------------------------------------------------------
bool COptions::summary_by_period(void) {

    CAccumulator accumulator;
    accumulator.period = by_period;
    accumulator.fmt = substitute(STR_DISPLAY_EXPORT, "[{MONTH}],[{DAY}]", per_2_Str(by_period));
    accumulator.discrete = discrete;
    accumulator.start = startBlock;
    accumulator.end = endBlock;
    forEveryLineInAsciiFile(resultsFile, visitLine, &accumulator);
    CReconciliation out(accumulator.sum);
    cout << out.Format(accumulator.fmt) << endl;

    return true;
}
