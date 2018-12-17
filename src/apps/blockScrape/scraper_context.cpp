/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
CScraperContext::CScraperContext(COptions *o, CBlock *b)
    : opts(o), pBlock(b), pTrans(NULL), addrList(NULL, NULL, true), traceCount(0), maxTraceDepth(0),
      reported(false), nAddrsInBloom(0), nAddrsInBlock(0),
      blockOkay(false), bloomOkay(false)
{
    bloomList.resize(1);
    bloomList.at(0) = 0;
    resetClock();
//    addrList.reserve(35000); // largest # of addrs during dDos
}

//-------------------------------------------------------------------------
bool CScraperContext::scrape(CBlock& block) {

    getObjectViaRPC(block, "eth_getBlockByNumber", "[\"" + uint_2_Hex(block.blockNumber) + "\",true]");

    if (!block.transactions.size()) {
        if (block.blockNumber < 50000) { // otherwise it appears to be hung
            cerr << "skipping empty block " << block.blockNumber << "\r";
            cerr.flush();
        }
        // TODO: we do not account for miners of zero transaction blocks, we can
        // do it here by writing the miner's address to a file
        return false;
    }

    // Add the miner to the bloom filter (TODO: we do not account for miners of zero transaction blocks)
    addToBloom(block.miner);

    // We can't use forEveryTrans... because it uses copies. We need to visit the actual transaction so we can write it
    for (uint32_t i = 0 ; i < block.transactions.size() ; i++) {
        if (!visitTransaction(block.transactions.at(i), this))
            return false;
    }

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
    bool isExcl      = ddos && (sCtx->opts->isExcluded(trans.from) || sCtx->opts->isExcluded(trans.to));
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
void CScraperContext::addToBloom(const address_t& addr) {
    if (isZeroAddr(addr))
        return;
    nAddrsInBloom++;
    nAddrsInBlock++;

    ASSERT(opts);
    if (opts->addrIndex && addrList.addrTxMap) {
        CAddressAppearance app;
        app.bn = pBlock->blockNumber;
        if (pTrans)
            app.tx = pTrans->transactionIndex;
        app.addr = addr;
        addrList.insertUnique(app);
    }

    // SEARCH FOR 'BIT_TWIDDLE_AMT 200'
    if (addAddrToBloom(addr, bloomList, opts->bitBound))
        nAddrsInBloom = 0;
}

#define cleanDate(dt)  substitute(substitute((dt).Format(FMT_JSON), " UTC", ""), " ", "T")
#define cleanDate1(ts) cleanDate(ts_2_Date((ts)))
//--------------------------------------------------------------------------
string_q CScraperContext::report(uint64_t last) {

    cerr << ((pBlock->finalized ? greenCheck : whiteStar) + " ");
    cerr << padRight(uint_2_Str(last - pBlock->blockNumber), 4) << ": ";

    string_q statusStr;
    statusStr += (cTeal + ((blockOkay && bloomOkay) ? status : "skipped"));

    ostringstream os;
    os.precision(4);

    os << bBlack;
    os << cleanDate1(pBlock->timestamp) << "\t";
    os << cleanDate (Now())             << "\t";
    os << TIC()                         << "\t" << cYellow;
    os << pBlock->blockNumber           << "\t";
    os << pBlock->transactions.size()   << "\t";
    os << traceCount                    << "\t";
    os << maxTraceDepth                 << "\t";
    os << nAddrsInBlock                 << "\t" << cOff;
    os << statusStr                     << "\t" << cYellow;
    os << reportBloom(bloomList)        << cOff;

    return os.str();
}
