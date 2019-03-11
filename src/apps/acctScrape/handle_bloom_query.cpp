/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
bool visitBloomFilters(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        // Descend into subfolders if there are any
        return forEveryFileInFolder(path + "*", visitBloomFilters, data);

    } else {

        // The context that we're working with...
        COptions *options = reinterpret_cast<COptions*>(data);

        // Silently skip unknown files (such as shell scripts for example).
        if (!endsWith(path, ".bin"))
            return !shouldQuit();

        // Which block are we working with?
        blknum_t bn = bnFromPath(path);

        // Are we too early? (`forEveryBloom` starts with entire folders, so we have to check...
        if (bn < options->startScrape)
            return !shouldQuit();

        // If we're past the end of our search, quit searching. The user tells us to start at x and scan for y
        // files (as an example, start at '0' scan for '1') therfore, we use >= so we don't overscan
        options->blkStats.nSeen++;
        if (bn >= options->startScrape + options->scrapeCnt) {
            // We're done searching all the monitors...
            for (auto monitor : options->monitors)
                monitor.writeLastBlock(bn);
            return false;
        }

        CBloomArray blooms;
        bool hasPotential = false;
        if (options->ignoreBlooms) {
            hasPotential = true;
            for (size_t ac = 0 ; ac < options->monitors.size() ; ac++)
                options->monitors.at(ac).inBlock = true;

        } else {
            // For each bloom filter, we identify which accounts are 'possibly in' the block. Once we decide this,
            // we can thereafter ignore any accounts that don't hit one of the bloom filters.
            CArchive bloomCache(READING_ARCHIVE);
            if (!bloomCache.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                // If we can't read the bloom, we want to skip over it next time, so write last block
                options->monitors[0].writeLastBlock(bn);
                cerr << "Could not open file " << path << ". Quitting search." << endl;
                return false; // end the search
            }

            // First, we need to read the bloom into memory...
            bloomCache >> blooms;
            bloomCache.Release();

            // For each account we look at each bloom to see if that account is 'of possible interest'. Note
            // accounts may hit the bloom filter, but not be 'of interest' because blooms sometimes report
            // false positives. So as to not produce duplicates we look at every transaction

            // Visit every account...
            for (size_t ac = 0 ; ac < options->monitors.size() ; ac++) {

                // Visit each bloom until either there is a hit or we've checked all blooms
                options->monitors.at(ac).inBlock = false;
                for (size_t bl = 0 ; bl < blooms.size() && !options->monitors[ac].inBlock ; bl++) {
                    bool hit = isBloomHit(options->monitors[ac].bloom, blooms[bl]);
                    if (hit) {
                        options->monitors.at(ac).inBlock = true;
                        hasPotential = true;
                    }
                }
            }
        }

        if (hasPotential) {
            processBlock(bn, options);

        } else {
            cerr << " bn: " << bn << *options << "\r";
            cerr.flush();
        }

        // May be redunant, but it's okay since we're writing a single value
        options->monitors[0].writeLastBlock(bn);
    }

    return !shouldQuit(); // continue if we should not quit
}

//-----------------------------------------------------------------------
bool processBlock(blknum_t bn, COptions *options) {

    options->blkStats.nQueried++;
    options->blockCounted = false;
    CBlock block;
    if (queryBlock(block, uint_2_Str(bn), false, false)) {

        // Check each transaction for actual involvement.
        for (size_t tr = 0; tr < block.transactions.size() ; tr++) {
            // ignore the return, so we can cleanup and write the last block
            bool ret = processTrans(block, &block.transactions[tr], options);
            if (!ret) {
                // May be redunant, but it's okay since we're writing a single value
                options->monitors[0].writeLastBlock(bn);
                return false;
            }
        }

    } else {
        // TODO(tjayrush): This is a bug. We should be writing the miner's record
        options->monitors[0].writeLastBlock(bn);
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool processTrans(const CBlock& block, const CTransaction *trans, COptions *options) {

    options->transStats.nSeen++;
    bool hit = false;

    // As soon as we have a hit on any account, we're done since we only want to write a transaction once...
    for (size_t ac = 0 ; ac < options->monitors.size() && !hit ; ac++) {

        // For each account we're monitoring...
        const CAccountWatch *acct = &options->monitors[ac];
        if (acct->inBlock) {

            // 'extended' pads the address to 32 bytes so it matches logs topics, logs data or input. Addresses
            // found in these locations may not mean an account is of interest, but we take the bet and
            // note it here. We do this to avoid traces if possible. There may be false positives here, since
            // the smart contact may be handling addresses as data (for example, transferFrom is sent by one
            // address, but delivers tokens to another). We choose to include these transactions in the data
            address_t watched = acct->address;
            string_q extended = substitute(padLeft(substitute(watched, "0x", ""), 64), " ", "0");

            // First, we check the simple, top-level data... (following variable is for clarity only)
            address_t contrAddr = trans->receipt.contractAddress;

            if (block.miner % watched) { hit = true; }
            else if (trans->to   % watched) { hit = true; }
            else if (trans->from % watched) { hit = true; }
            else if (contrAddr   % watched) { hit = true; }

            if (!hit) {
                // If we did not find a hit, dig deeper
                if (containsI(trans->input, extended)) {
                    hit = true;

                } else {

                    // Technically, we don't have to check the logs (events can be found in the traces). But, we
                    // want to avoid having to get traces at all costs. The `blockScraper` app must look at every
                    // trace, therefor, it does not check logs. As soon as we hit, we can stop
                    for (size_t lg = 0 ; lg < trans->receipt.logs.size() && !hit ; lg++) {

                        const CLogEntry *l = &trans->receipt.logs[lg];
                        if (l->address % watched        ) { hit = true; }
                        else if (containsI(l->data, extended)) { hit = true; }

                        // Or if we find it as one of the topics. Note, won't spin if we've already hit.
                        for (size_t tp = 1 ; tp < l->topics.size() && !hit ; tp++) {
                            string_q topic = topic_2_Str(l->topics[tp]);
                            if (topic % extended) { hit = true; }
                        }
                    }
                }

                // If we are still not hit, we must decend into the traces. Sorry...but we must be thourough...
                if (!hit) {
                    hit = processTraces(block, trans, acct, options);
                }
            }

            if (hit) {
                options->foundAHit(acct, block, trans);

            } else {
                cerr << " bn: " << block.blockNumber << *options << "\r";
                cerr.flush();
            }
        }
    }
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool processTraces(const CBlock& block, const CTransaction *trans, const CAccountWatch *acct, COptions *options) {

    // TODO: If we wanted to add an option called 'deep' to decend into traces even if they are dDos we would to it here.
    size_t nTraces = getTraceCount(trans->hash);
    bool ddos      = ddosRange(block.blockNumber);
    bool isDDos    = ddos && nTraces > 250;
    bool isExcl    = ddos && options->isExcluded(trans->to);

    if (isDDos || isExcl) {
        options->traceStats.nSeen    += nTraces;
        options->traceStats.nSkipped += nTraces;
        return false;
    }

    CTraceArray traces;
    getTraces(traces, trans->hash); // This call right here is very slow

    address_t watched = acct->address;
    for (size_t tc = 0 ; tc < traces.size() ; tc++) {
        options->traceStats.nSeen++;

        const CTraceAction *action = &traces[tc].action;
        const CTraceResult *result = &traces[tc].result;

        bool hit = false;
        if (action->to            % watched) { hit = true; }
        else if (action->from          % watched) { hit = true; }
        else if (action->address       % watched) { hit = true; }
        else if (action->refundAddress % watched) { hit = true; }
        else if (result->address       % watched) { hit = true; }

        if (hit) {
            options->traceStats.nHit++;
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------
const string_q fmt = "[{BLOCKNUMBER} ][{w:5:TRANSACTIONINDEX} ][{w:10:DATE} ][{ETHER}]";
//-------------------------------------------------------------------------
bool COptions::foundAHit(const CAccountWatch *acct, const CBlock& block, const CTransaction *trans) {
    if (!blockCounted) {
        blockCounted = true;
        blkStats.nHit++; // only count the block once per block
    }
    transStats.nHit++;

    CBlock *pBlock = (CBlock*)&block;
    pBlock->finalized = isBlockFinal(block.timestamp, lastTimestamp, (60 * 4));

    lockSection(true);
    if (!isTestMode()) {
        // We found something...write it to the cache...
        *monitors[0].txCache << block.blockNumber << trans->transactionIndex;
        monitors[0].txCache->flush();
        monitors[0].writeLastBlock(block.blockNumber);
        if (writeBlocks) {
            // pBlock->finalized is implicit here don't remove it above
            string_q fn = getBinaryFilename(block.blockNumber);
            if (!fileExists(fn))
                writeBlockToBinary(block, fn);
        }
    }

    // Send the data to an api if we have one
    if (!acct->api_spec.uri.empty()) {
        ((CTransaction*)trans)->extra_data = acct->extra_data;
        ((CTransaction*)trans)->finalized = pBlock->finalized;

        if (trans->traces.size() == 0)
            getTraces(((CTransaction*)trans)->traces, trans->hash);
        acct->abi_spec.articulateTransaction((CTransaction*)trans);

        SHOW_FIELD(CFunction,    "message");
        SHOW_FIELD(CTransaction, "extra_data");
        SHOW_FIELD(CTransaction, "finalized");
        if (isTestMode() || fileExists("./debug")) {
            SHOW_FIELD(CLogEntry,  "data");
            SHOW_FIELD(CLogEntry,  "topics");
            SHOW_FIELD(CParameter, "type");
            cout << trans->Format() << endl;
        } else {
            ((CAccountWatch*)acct)->api_spec.sendData(trans->Format());
        }
        HIDE_FIELD(CTransaction, "extra_data");
        HIDE_FIELD(CTransaction, "finalized");
        HIDE_FIELD(CFunction,    "message");
    }
    lockSection(false);

    cerr << string_q(4,' ') << acct->displayName(false,true,true,8) << ": ";
    cerr << trans->Format(fmt) << " (" << blkStats.nSeen << " of " << scrapeCnt << ")" << string_q(15,' ') << endl;

    return true;
}
