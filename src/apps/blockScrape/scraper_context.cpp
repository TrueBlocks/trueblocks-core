/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
CScraper::CScraper(COptions *o, blknum_t num) :
    options(o),
    status(""),
    addrList(NULL, NULL, true),
    pTrans(NULL),
    traceCount(0),
    curSize(0),
    maxTraceDepth(0),
    nAddrsInBlock(0)
{
    ASSERT(addrList.addrTxMap);
    block.blockNumber = num;
    resetClock();
}

//-------------------------------------------------------------------------
bool CScraper::scrapeBlock(void) {

    // This queries the node for fresh data on the block
    getObjectViaRPC(block, "eth_getBlockByNumber", "[\"" + uint_2_Hex(block.blockNumber) + "\",true]");

    // Note the miner address
    noteAddress(block.miner, true);

    // We do not use forEveryTransaction here because we need the actual transaction so we can mark
    // pre-byzantium errors by querying traces. forEvery... makes a copy which won't mark the error
    for (uint32_t i = 0 ; i < block.transactions.size() ; i++) {
        if (!scrapeTransaction(&block.transactions[i]))
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool CScraper::scrapeTransaction(CTransaction *txPtr) {

    pTrans = txPtr;
    pTrans->pBlock = &block;
    getReceipt(pTrans->receipt, pTrans->hash);

    noteAddress(pTrans->from);
    noteAddress(pTrans->to);
    noteAddress(pTrans->receipt.contractAddress);
    foundPotential(notePotential, this, pTrans->blockNumber, pTrans->transactionIndex, 0, extract(pTrans->input, 10));

    for (uint32_t lg = 0 ; lg < pTrans->receipt.logs.size() ; lg++) {
        const CLogEntry *log = &pTrans->receipt.logs[lg];
        noteAddress(log->address);
        string_q addr;
        for (uint32_t tp = 0 ; tp < log->topics.size() ; tp++) {
            if (isPotentialAddr(log->topics[tp], addr))
                noteAddress(addr);
        }
        foundPotential(notePotential, this, pTrans->blockNumber, pTrans->transactionIndex, 0, extract(log->data, 2));
    }

    uint64_t nTraces = getTraceCount(pTrans->hash);
    bool ddos        = ddosRange(block.blockNumber);
    bool isDDos      = ddos && nTraces > 250;
    bool isExcl      = ddos && (options->isExcluded(pTrans->from) || options->isExcluded(pTrans->to));
    if (!isDDos && !isExcl) {
        // We can use forEveryTrace... here because we don't need to save any data on the trace. It's okay to use a copy
        potList = "";
        forEveryTraceInTransaction(visitTrace, this, *pTrans);
        // We handle all the traces potentials here since we don't use trace id anyway
        foundPotential(notePotential, this, pTrans->blockNumber, pTrans->transactionIndex, 0, potList);
    }

    if (pTrans->pBlock->blockNumber >= byzantiumBlock)
        pTrans->isError = (pTrans->receipt.status == 0); // contains zero on fail

    return true;
}

//-------------------------------------------------------------------------
bool visitTrace(CTrace& trace, void *data) {

    CScraper *sCtx = (CScraper*)data;

    // Note, this is redundant. It is also done in queryBlock. We do it in both places becuase it is
    // possible to create (and write) a new block directly from other programs
    if (sCtx->pTrans->pBlock->blockNumber < byzantiumBlock && // if we are before the byzantium hard fork...
        sCtx->pTrans->gas == sCtx->pTrans->receipt.gasUsed) { // ...and gasUsed == gasLimit...
        if (!sCtx->pTrans->isError)                           // ...and we're not yet an error...
            ((CTransaction*)sCtx->pTrans)->isError = trace.isError();
    }

    sCtx->noteAddress(trace.action.from);
    sCtx->noteAddress(trace.action.to);
    sCtx->noteAddress(trace.action.refundAddress);
    sCtx->noteAddress(trace.action.address);
    sCtx->noteAddress(trace.result.address);
    sCtx->potList += extract(trace.action.input, 10);
    sCtx->potList += extract(trace.result.output, 2);
    sCtx->traceCount++;
    sCtx->maxTraceDepth = max(sCtx->maxTraceDepth, (uint64_t)trace.traceAddress.size());

    return true;
}

//----------------------------------------------------------------------------------
void CScraper::noteAddress(const address_t& addr, bool isMiner) {

    if (isZeroAddr(addr))
        return;

    nAddrsInBlock++;

    ASSERT(options && sCtx.addrList.addrTxMap);
    CAddressAppearance app;
    app.addr = addr;
    app.bn = block.blockNumber;
    if (isMiner)
        app.tx = 99999;
    else if (pTrans)
        app.tx = pTrans->transactionIndex;
    addrList.insertUnique(app);
}

//--------------------------------------------------------------------------
class CCounter {
public:
    blknum_t bn;
    blknum_t lines;
    blknum_t size;
    CCounter(string_q& line) {
        CStringArray fields;
        explode(fields, line, '\t');
        bn = str_2_Uint(fields[0]);
        lines = str_2_Uint(fields[1]);
        size = str_2_Uint(fields[2]);
    }
};

//--------------------------------------------------------------------------
void CScraper::updateAddrIndex(void) {

    ASSERT(pBlock);
    string_q indexFilename = indexFolder_staging_v2 + padNum9(block.blockNumber) + ".txt";
    string_q countFile     = indexFolder_staging_v2 + "counts.txt";

    // Note: we are inside the lockSection

    // We've scanned the block. We have the list of all addresses. We write it here even if it's
    // not final because the next time we re-visit this block we may not re-scan in which case we
    // won't have the list. If we do rescan, this list will get re-rewritten.
    ostringstream os1;
    ASSERT(addrList.addrTxMap);
    CAddressTxAppearanceMap::iterator it;
    for (it=addrList.addrTxMap->begin(); it!=addrList.addrTxMap->end(); it++ ) {
        os1 << it->first.addr << "\t";
        os1 << padNum9(it->first.bn) << "\t";
        os1 << padNum5(it->first.tx) << "\n";;
    }
    stringToAsciiFile(indexFilename, os1.str());

    ostringstream os2;
    os2 << block.blockNumber << "\t" << addrList.addrTxMap->size() << "\t" << fileSize(indexFilename) << "\n";
    appendToAsciiFile(countFile, os2.str());

    if (true) { //options->consolidate) {
        string_q contents;
        asciiFileToString(countFile, contents);
        CStringArray lines;
        explode(lines, contents, '\n');
        vector<CCounter> counters;
        curSize = 0;
        for (auto line : lines) {
            CCounter counter(line);
            counters.push_back(counter);
            curSize += counter.size;
        }
        if (curSize > options->maxIndexBytes) {
            CStringArray apps;
            apps.reserve(620000);
            for (auto counter : counters) {
                string_q theStuff;
                string_q fn = substitute(countFile, "counts", padNum9(counter.bn));
                asciiFileToString(fn, theStuff);
                CStringArray lns;
                explode(lns, theStuff, '\n');
                for (auto ln : lns) {
                    apps.push_back(ln);
                }
            }
            sort(apps.begin(), apps.end());
            blknum_t first = counters[0].bn;
            blknum_t last = counters[counters.size()-1].bn;
            string_q resFile = substitute(countFile, "counts", padNum9(first)+"-"+padNum9(last));
            resFile = substitute(resFile, "/staging/", "/tmp/");
            for (auto app : apps)
                appendToAsciiFile(resFile, app + "\n");
            ::remove(countFile.c_str());
            for (auto counter : counters)
                ::remove(substitute(countFile, "counts", padNum9(counter.bn)).c_str());
            putc(7,stdout);

cout << resFile << endl;
cout << "Press enter to continue or 'q' to quit" << endl;

        }
    }
}

//--------------------------------------------------------------------------
void CScraper::updateIndexes(void) {

    updateAddrIndex();

    CBlockIndexItem item(block.blockNumber, block.timestamp, block.transactions.size());
    options->finalBlockCache2 << item.bn << item.ts << item.cnt;
    options->finalBlockCache2.flush();
}

//-------------------------------------------------------------------------
bool notePotential(const CAddressAppearance& item, void *data) {
    CScraper *sCtx = (CScraper*)data;
    sCtx->noteAddress(item.addr);
    return true;
}

//---------------------------------------------------------------------------
void foundPotential(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList) {
    CAddressAppearance item(bn, tx, tc, "", "");
    potentialAddr(func, data, item, potList);
}

//--------------------------------------------------------------------------
string_q CScraper::report(uint64_t last) {

    time_q blkDate = ts_2_Date(block.timestamp);

    cerr << ((block.finalized ? greenCheck : whiteStar) + " ");
    cerr << padRight(uint_2_Str(last - block.blockNumber), 4) << ": ";

    ostringstream os; os.precision(4);
    os << bBlack;
    os << blkDate.Format(FMT_EXPORT)  << "\t";
    os << Now().Format(FMT_EXPORT)    << "\t";
    os << TIC()                       << "\t" << cYellow;
    os << block.blockNumber           << "\t";
    os << block.transactions.size()   << "\t";
    os << traceCount                  << "\t";
    os << maxTraceDepth               << "\t";
    os << nAddrsInBlock               << "\t";
    os << addrList.addrTxMap->size()  << "\t";
    os << fixed << setprecision(3);
    os << (curSize/1024./1024.)       << "\t" << cTeal;
    os << status                      << cOff;

    return os.str();
}
