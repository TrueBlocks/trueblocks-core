/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
CScraperContext::CScraperContext(COptions *o, CBlock *b)
    : options(o), pBlock(b), pTrans(NULL), addrList(NULL, NULL, true), traceCount(0), maxTraceDepth(0),
      reported(false), nAddrsInBloom(0), nAddrsInBlock(0),
      blockOkay(false), bloomOkay(false)
{
    ASSERT(addrList.addrTxMap);
    bloomList.resize(1);
    bloomList.at(0) = 0;
    resetClock();
//    addrList.reserve(35000); // largest # of addrs during dDos
}

//-------------------------------------------------------------------------
bool CScraperContext::scrape(CBlock& block) {

    getObjectViaRPC(block, "eth_getBlockByNumber", "[\"" + uint_2_Hex(block.blockNumber) + "\",true]");

    // Add the miner to the bloom filter
    addToBloom(block.miner, true);

    if (!block.transactions.size()) {
        if (block.blockNumber < 50000) { // otherwise it appears to be hung
            cerr << "skipping empty block " << block.blockNumber << "\r";
            cout << report(options->endBlock) << endl;
            cerr.flush();
        }
        // TODO: we do not account for miners of zero transaction blocks, we can
        // do it here by writing the miner's address to a file
        return false;
    }

    // We can't use forEveryTrans... because it uses copies. We need to visit the actual transaction so we can write it
    for (uint32_t i = 0 ; i < block.transactions.size() ; i++) {
        if (!visitTransaction(block.transactions.at(i), this)) {
            cout << report(options->endBlock) << endl;
            return false;
        }
    }

    cout << report(options->endBlock) << endl;
    return true;
}

//-------------------------------------------------------------------------
bool addPotentialAddr(const CAddressAppearance& item, void *data) {
    CScraperContext *sCtx = (CScraperContext*)data;
    sCtx->addToBloom(item.addr);
    return true;
}

//---------------------------------------------------------------------------
void foundPot1(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList) {
    CAddressAppearance item(bn, tx, tc, "", "");
    potentialAddr(func, data, item, potList);
}

//-------------------------------------------------------------------------
// Note: See CBlock::forEveryAddress for a similar algorithm. We can't use that
// because we are processing transaction error marks here as well.
bool visitTransaction(CTransaction& trans, void *data) {

    CScraperContext *sCtx = (CScraperContext*)data;
    sCtx->pTrans = &trans;
    trans.pBlock  = sCtx->pBlock;

    getReceipt(trans.receipt, trans.hash);
    sCtx->addToBloom(trans.from);
    sCtx->addToBloom(trans.to);
    sCtx->addToBloom(trans.receipt.contractAddress);
    foundPot1(addPotentialAddr, sCtx, trans.blockNumber, trans.transactionIndex, 0, extract(trans.input, 10));

    for (uint32_t lg = 0 ; lg < trans.receipt.logs.size() ; lg++) {
        const CLogEntry *log = &trans.receipt.logs[lg];
        sCtx->addToBloom(log->address);
        // Note: Add topics that look like addresses even though they may be redundant.
        string_q addr;
        for (uint32_t tp = 0 ; tp < log->topics.size() ; tp++) {
            if (isPotentialAddr(log->topics[tp], addr)) {
                sCtx->addToBloom(addr);
            }
        }
        foundPot1(addPotentialAddr, sCtx, trans.blockNumber, trans.transactionIndex, 0, extract(log->data, 2));
    }

    uint64_t nTraces = getTraceCount(trans.hash);
    bool ddos        = ddosRange(sCtx->pBlock->blockNumber);
    bool isDDos      = ddos && nTraces > 250;
    bool isExcl      = ddos && (sCtx->options->isExcluded(trans.from) || sCtx->options->isExcluded(trans.to));
    if (!isDDos && !isExcl) {
        // We can use forEveryTrace... here because we don't need to save any data on the trace. It's okay to use a copy
        sCtx->potList = "";
        forEveryTraceInTransaction(visitTrace, sCtx, trans);
        // We handle all the traces potentials here since we don't use trace id anyway
        foundPot1(addPotentialAddr, sCtx, trans.blockNumber, trans.transactionIndex, 0, sCtx->potList);
    }

    if (trans.pBlock->blockNumber >= byzantiumBlock)
        trans.isError = (trans.receipt.status == 0); // contains zero on fail

    return true;
}

//-------------------------------------------------------------------------
bool visitTrace(CTrace& trace, void *data) {

    CScraperContext *sCtx = (CScraperContext*)data;

    // Note, this is redundant. It is also done in queryBlock. We do it in both places becuase it is
    // possible to create (and write) a new block directly from other programs
    if (sCtx->pTrans->pBlock->blockNumber < byzantiumBlock && // if we are before the byzantium hard fork...
        sCtx->pTrans->gas == sCtx->pTrans->receipt.gasUsed) { // ...and gasUsed == gasLimit...
        if (!sCtx->pTrans->isError)                           // ...and we're not yet an error...
            ((CTransaction*)sCtx->pTrans)->isError = trace.isError();
    }

    sCtx->addToBloom(trace.action.from);
    sCtx->addToBloom(trace.action.to);
    sCtx->addToBloom(trace.action.refundAddress);
    sCtx->addToBloom(trace.action.address);
    sCtx->addToBloom(trace.result.address);
    sCtx->potList += extract(trace.action.input, 10);
    sCtx->traceCount++;
    sCtx->maxTraceDepth = max(sCtx->maxTraceDepth, (uint64_t)trace.traceAddress.size());

    return true;
}

//----------------------------------------------------------------------------------
void CScraperContext::addToBloom(const address_t& addr, bool isMiner) {

    if (isZeroAddr(addr))
        return;

    nAddrsInBloom++;
    nAddrsInBlock++;
    addToIndex(addr, isMiner);

    // SEARCH FOR 'BIT_TWIDDLE_AMT 200'
    if (addAddrToBloom(addr, bloomList, options->bitBound))
        nAddrsInBloom = 0;
}

//----------------------------------------------------------------------------------
void CScraperContext::addToIndex(const address_t& addr, bool isMiner) {

    if (!options->addrIndex)
        return;

    ASSERT(options && sCtx.addrList.addrTxMap);
    CAddressAppearance app;
    app.addr = addr;
    app.bn = pBlock->blockNumber;
    if (isMiner) {
        app.tx = 99999;
    } else {
        if (pTrans)
            app.tx = pTrans->transactionIndex;
    }
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
void CScraperContext::updateAddrIndex(void) {

    if (!options->addrIndex)
        return;

    ASSERT(pBlock);
    string_q stageFilename = indexFolder_stage + "staging.txt";

    // First we write the current block's data to the staging file...
    ostringstream os1;
    ASSERT(addrList.addrTxMap);
    ASSERT(addrList.addrTxMap.size() > 0);
    blknum_t maxBlock = 0;
    CAddressTxAppearanceMap::iterator it;
    for (it=addrList.addrTxMap->begin(); it!=addrList.addrTxMap->end(); it++ ) {
        os1 << it->first.addr << "\t";
        os1 << padNum9(it->first.bn) << "\t";
        os1 << padNum5(it->first.tx) << endl;
        maxBlock = max(maxBlock, it->first.bn);
    }
    appendToAsciiFile(stageFilename, os1.str());

    // ...now we consolidate if we're told to
    if (!options->consolidate)
        return;

    if (maxBlock == 0) // weird case should not happen
        return;

    size_t size = fileSize(stageFilename);
    if (size < options->maxIndexBytes)
        return;

    // It's okay to write the sorted index, so let's do it
    string_q contents;
    asciiFileToString(stageFilename, contents);

    CStringArray apps;
    apps.reserve(900000);
    explode(apps, contents, '\n');

    CStringArray parts;

    // Find first block
    string_q tmp = apps[0];
    explode(parts, tmp, '\t');
    uint64_t blockOne = str_2_Uint(parts[1]);
    parts.clear();

    // Find last block (plus one)
    tmp = apps[apps.size() - 1];
    explode(parts, tmp, '\t');
    uint64_t blockN = str_2_Uint(parts[1]) + 1;

    // sort the array by tx within block within address
    sort(apps.begin(), apps.end());

    string_q outFilename = substitute(stageFilename, "staging", padNum9(blockOne) + "-" + padNum9(blockN));
    ofstream output;
    output.open(outFilename.c_str());
    if (!output.is_open()) {
        cerr << "Could not open file '" << outFilename << endl;
        return;
    }

    for (auto app : apps)
        output << app << endl;
    output.close();

    ::remove(stageFilename.c_str());
    putc(7,stdout);
}

//--------------------------------------------------------------------------
string_q CScraperContext::report(uint64_t last) {

    ASSERT(sCtx.addrList.addrTxMap);

    cerr << ((pBlock->finalized ? greenCheck : whiteStar) + " ");
    cerr << padRight(uint_2_Str(last - pBlock->blockNumber), 4) << ": ";

    ostringstream os; os.precision(4);

    string_q stageFilename = indexFolder_stage + "staging.txt";
    time_q   blkDate = ts_2_Date(pBlock->timestamp);

    os << bBlack;
    os << blkDate.Format(FMT_EXPORT)  << "\t";
    os << Now().Format(FMT_EXPORT)    << "\t";
    os << TIC()                       << "\t" << cYellow;
#if 1
    os << fileSize(stageFilename)     << "\t";
#endif
    os << pBlock->blockNumber         << "\t";
    os << pBlock->transactions.size() << "\t";
    os << traceCount                  << "\t";
    os << maxTraceDepth               << "\t";
    os << nAddrsInBlock               << "\t";
    os << addrList.addrTxMap->size()  << "\t" << cTeal;
    os << padRight(status, 9)         << "\t" << cYellow;
    os << reportBloom(bloomList)      << cOff;

    return os.str();
}
