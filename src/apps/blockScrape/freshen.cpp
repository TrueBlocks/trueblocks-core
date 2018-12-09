/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool updateIndex(CArchive& fullBlockCache, blknum_t bn);
extern string_q timerStr(double start);
//--------------------------------------------------------------------------
bool freshenLocalCache(COptions& options) {

    // Note: It's okay to open this once and keep it open since we protect the write with locksection.
    // If the program quits unexpectedly, the file will get proplery closed anyway.
    CArchive fullBlockCache(WRITING_ARCHIVE);
    if (!fullBlockCache.Lock(fullBlockIndex, "a+", LOCK_WAIT)) {
        cerr << "updateIndex failed: " << fullBlockCache.LockFailure() << "\n";
        return false;
    }

    CBlock latest;
    getBlock(latest, "latest");

    double lastRun = qbNow();
    for (blknum_t num = options.start ; num < options.end && !shouldQuit() ; num++) {

        if (!isParity() || !nodeHasTraces()) {
            cerr << "This tool can only run against a Parity node that has tracing enabled. Quitting..." << "\n";
            cerr.flush();
            return false;
        }

        CBlock block;
        block.blockNumber = num;
        CScraperCtx sCtx(&options);
        sCtx.pBlock = &block;

        // We need to handle the case of a re-org, so we only write the fullBlock index if we are sure
        // the block is final. In this way, each time we re-start, we will restart from the place
        // where the last index was written. We also optionally write the block data to disc (this
        // defaults to true). By making this optional, we can delay writing the larger block data
        // until we've identified blocks of interest if so desired.

        string_q blockFilename = getBinaryFilename(num);
        string_q bloomFilename = substitute(blockFilename, "/blocks/", "/blooms/");
        bool blockOkay = false, bloomOkay = false;

        string_q action;
        bool goodHash = false;
        if (fileExists(blockFilename)) {
            // If the file exists, we may be able to skip re-scanning it
            bloomOkay = blockOkay = true;
            readBlockFromBinary(block, blockFilename);
            readBloomArray(sCtx.blooms, bloomFilename);

            // If it had been finalized, we wouldn't be re-reading it
            ASSERT(!block.finalized);

            // Check to see if the hash has changed
            goodHash = (block.hash == getRawBlockHash(num));

            // Check to see if its finalized
            block.finalized = isBlockFinal(block.timestamp, latest.timestamp, (60 * 4));

            if (goodHash) {
                // If it's the same hash, assume it's not final...
                action = "not final";
                if (block.finalized) {
                    // ...unless it is...
                    action = "final-a";
                    lockSection(true);
                    blockOkay = (!options.writeBlocks || writeBlockToBinary(block, blockFilename));
                    updateIndex(fullBlockCache, num);
                    lockSection(false);
                }
            } else {
                // This is a block re-org, so we will re-scan
                action = "reorg-re";
            }
        }

        // If we already have a good hash, we're re-written it if it's now final, or we don't have to if it isn't.
        // If we don't have a good hash (either because we've never seen this block before or it's changed since
        // last read), we need to rescan.
        if (!goodHash && sCtx.scrape(block)) {
            action += "scanned";
            block.finalized = isBlockFinal(block.timestamp, latest.timestamp, (60 * 4));
            lockSection(true);
            blockOkay = (options.writeBlocks ? writeBlockToBinary(block, blockFilename) : true);
            bloomOkay = writeBloomArray(sCtx.blooms, bloomFilename);
            if (block.finalized) {
                updateIndex(fullBlockCache, num);
                action = "final-b";
            }
            lockSection(false);
        }

        string_q timeStr = timerStr(lastRun);

        ASSERT((block.transactions.size() && (blockOkay && bloomOkay)) || (!block.transactions.size() && (!blockOkay && !bloomOkay)));
        string_q result = "\r  @DATE-DIS-SEC} {NUM} ({LEFT}): ({TXS} /{TRC}-{DPT} /{ADDRS}) WRITE+PATH}: BL\n";
        replaceAll (result, "{",      cYellow);
        replaceAll (result, "+",      (blockOkay && bloomOkay) ? bTeal : bRed);
        replaceAll (result, "@",      bBlack);
        replaceAll (result, "}",      cOff);
        replace    (result,  "WRITE", ((block.finalized) ? greenCheck : (bWhite + "✽" + cOff)) + " " + padRight(((blockOkay && bloomOkay) ? action : "skipped"), 10));
        replace    (result,  "NUM",   uint_2_Str(num));
        replace    (result,  "LEFT",  padNum4T(options.end - num));
        replace    (result,  "DATE",  extract(substitute(ts_2_Date(block.timestamp).Format(FMT_JSON), " UTC", ""), 2));
        replace    (result,  "TXS",   padNum3T((uint64_t)block.transactions.size()));
        replace    (result,  "TRC",   padNum4T(sCtx.traceCount));
        replace    (result,  "DPT",   padNum3T(sCtx.maxTraceDepth));
        replace    (result,  "ADDRS", padNum4T(sCtx.totAccounts));
        replace    (result,  "BL",    reportBloom(sCtx.blooms));
        replace    (result,  "PATH",  substitute(bloomFilename, blockCachePath(""), "./"));
        replace    (result,  "DIS",   padLeft(int_2_Str(latest.timestamp - block.timestamp),3));
        replace    (result,  "SEC",   timeStr);
        cout << result;
        cout.flush();
//        if ((Now().m_nSeconds - lastRun.m_nSeconds) > 3) {
//            ostringstream os;
//            os << result;
//            appendToAsciiFile("./scraper_log.txt", os.str().c_str());
//        }
        lastRun = qbNow();
    }

    if (fullBlockCache.isOpen())
        fullBlockCache.Release();

    return true;
}

//-------------------------------------------------------------------------
bool CScraperCtx::scrape(CBlock& block) {

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
    CScraperCtx *sCtx = (CScraperCtx*)data;
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

    CScraperCtx *sCtx = (CScraperCtx*)data;
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

    CScraperCtx *sCtx = (CScraperCtx*)data;

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
void CScraperCtx::addToBloom(const address_t& addr) {
    if (isZeroAddr(addr))
        return;
    nAccounts++;
    totAccounts++;
//    if (opts && opts->keepAddrIdx)
//        appendToAsciiFile("addr_index.txt", (addr + "\t" + uint_2_Str(pBlock->blockNumber) + "\n"));
// SEARCH FOR 'BIT_TWIDDLE_AMT'
    if (addAddrToBloom(addr, blooms, 200U))
        nAccounts = 0;
}

//----------------------------------------------------------------------------------
string_q timerStr(double start) {
    string_q perfStr = double_2_Str(max(0.0, qbNow() - start), 4);
    if (!contains(perfStr, "."))
        perfStr += ".0000";
    string_q end = perfStr;
    nextTokenClear(end, '.');
    if (end.length() < 1) perfStr += ".0000";
    if (end.length() < 2) perfStr += "000";
    if (end.length() < 3) perfStr += "00";
    if (end.length() < 4) perfStr += "0";
    return perfStr;
}
