/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        blknum_t lastInCache = getLatestBlockFromCache();
        blknum_t lastVisited = str_2_Uint(asciiFileToString(getTransCachePath("lastBlock.txt")));

        if (options.minWatchBlock > 0)
            lastVisited = max(options.minWatchBlock - 1, lastVisited);

        // Only scrape if the transaction cache is behind the index
        if (lastVisited <= lastInCache) {

            options.lastBlock  = min(lastInCache, options.maxWatchBlock);
            options.firstBlock = min(lastVisited, options.lastBlock);
            options.nBlocks    = min(options.lastBlock - options.firstBlock, options.maxBlocks);
            if (options.useIndex)
                options.nBlocks = 10000000;  // TODO(tjayrush): Not right

            if (options.txCache.Lock(options.cacheFilename, "a+", LOCK_WAIT)) {

                if (options.useIndex) {
                    forEveryFileInFolder(indexFolder_prod, visitIndexFiles, &options);

                } else {
                    forEveryBloomFile(visitBloomFilters, &options, options.firstBlock, options.nBlocks);

                }
                options.txCache.Release();

            } else {
                return options.usage("Cannot open transaction cache '" + options.cacheFilename + "'. Quitting...");

            }
        }
    }

    if (options.isList) {
        CArchive cache(READING_ARCHIVE);
        if (!cache.Lock(options.cacheFilename, binaryReadOnly, LOCK_NOWAIT))
            return options.usage("Could not open file: " + options.cacheFilename + ". Quitting.");
        while (!cache.Eof() && !shouldQuit()) {
            CAcctCacheItem item;
            cache >> item.blockNum >> item.transIndex;
            if (item.blockNum > 0)
                cout << item.blockNum << "\t" << item.transIndex << endl;
        }
        cache.Release();

    } else {

        cerr << options.name << " bn: " << options.firstBlock + options.nBlocks << options << "\r";
        cerr.flush();

        if (options.blkStats.nSeen && options.logLevel > 0) {
            string_q logFile = blockCachePath("logs/acct-scrape.log");
            double startTime = qbNow();
            if (!fileExists(logFile))
                appendToAsciiFile(logFile, options.finalReport(startTime, true));
            if (options.logLevel >= 2 || options.blkStats.nSeen > 1000)
                appendToAsciiFile(logFile, options.finalReport(startTime, false));
        }
    }

    acctlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool visitBloomFilters(const string_q& path, void *data) {

    COptions *options = reinterpret_cast<COptions*>(data);
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", visitBloomFilters, data);

    } else {

        if (endsWith(path, ".bin")) {

            blknum_t bn = bnFromPath(path);
            if (bn < options->firstBlock) {
                // We may be too early since the forEveryBloom function starts with entire
                // folders, so we must check and possibly break out early...
                return true;

            } else if (bn >= options->firstBlock + options->nBlocks) {
                // If we're not too late....don't contiue...
                writeLastBlock(bn);
                options->blkStats.nSeen++;
                return false;
            }

            options->blkStats.nSeen++;
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
                if (!bloomCache.Lock(path, binaryReadOnly, LOCK_NOWAIT)) {
                    // If we can't read the bloom, we want to skip over it next time, so write last block
                    writeLastBlock(bn);
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
                cerr << options->name << " bn: " << bn << *options << "\r";
                cerr.flush();
            }

            // May be redunant, but it's okay since we're writing a single value
            writeLastBlock(bn);

        } else {
            // silently skip case where file name does not end with .bin
        }
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
                writeLastBlock(bn);
                return false;
            }
        }

    } else {
        // TODO(tjayrush): This is a bug. We should be writing the miner's record
        writeLastBlock(bn);
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
const string_q fmt = "[{BLOCKNUMBER} ][{w:5:TRANSACTIONINDEX} ][{w:10:DATE} ][{ETHER}]";
//-----------------------------------------------------------------------
bool processTrans(const CBlock& block, const CTransaction *trans, COptions *options) {

    options->transStats.nSeen++;
    bool hit = false;

    // As soon as we have a hit on any account, we're done since we only want to write a transaction once...
    for (size_t ac = 0 ; ac < options->monitors.size() && !hit ; ac++) {

        string_q ex_data = toLower("chifra/v0.0.1: " + options->name + "_" + options->monitors[0].address);

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

                if (!options->blockCounted) {
                    options->blockCounted = true;
                    options->blkStats.nHit++; // only count the block once per block
                }
                options->transStats.nHit++;

                if (!isTestMode()) {
                    // We found something...write it to the cache...
                    lockSection(true);
                    options->txCache << block.blockNumber << trans->transactionIndex;
                    options->txCache.flush();
                    writeLastBlock(block.blockNumber);

                    CBlock *pBlock = (CBlock*)&block;
                    pBlock->finalized = isBlockFinal(block.timestamp, options->lastTimestamp, (60 * 4));

                    // Send the data to an api if we have one
                    if (!acct->api_spec.uri.empty()) {
                        if (trans->traces.size() == 0)
                            getTraces(((CTransaction*)trans)->traces, trans->hash);
                        acct->abi_spec.articulateTransaction((CTransaction*)trans);
                        if (!trans->articulatedTx.message.empty())
                            SHOW_FIELD(CFunction, "message");
                        SHOW_FIELD(CTransaction, "extra_data");
                        SHOW_FIELD(CTransaction, "finalized");
                        ((CTransaction*)trans)->extra_data = ex_data;
                        ((CTransaction*)trans)->finalized = pBlock->finalized;
                        if (!fileExists("./debug")) {
                            string_q data = trans->Format();
                            ((CAccountWatch*)acct)->api_spec.sendData(data);
                        } else {
                            SHOW_FIELD(CLogEntry, "data");
                            SHOW_FIELD(CLogEntry, "topics");
                            SHOW_FIELD(CParameter, "type");
                            cout << trans->Format() << endl;
                        }
                        HIDE_FIELD(CTransaction, "extra_data");
                        HIDE_FIELD(CTransaction, "finalized");
                        HIDE_FIELD(CFunction, "message");
                    }

                    // Also, we optionally write blocks if we're told to do so
                    if (options->writeBlocks) {
                        // pBlock->finalized is used here too
                        string_q fn = getBinaryFilename(block.blockNumber);
                        if (!fileExists(fn))
                            writeBlockToBinary(block, fn);
                    }
                    lockSection(false);
                }

                // Report to the screen
                cerr << "        " << options->monitors[ac].displayName(false,true,true,8)
                        << ": " << trans->Format(fmt)
                        << " (" << options->blkStats.nSeen << " of " << options->nBlocks << ") "
                        << "                    " << endl;

            } else {
                cerr << options->name << " bn: " << block.blockNumber << *options << "\r";
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
bool COptions::loadMonitors(void) {

    minWatchBlock = UINT32_MAX;
    maxWatchBlock = 0;

    string_q watchStr = toml->getConfigJson("watches", "list", "");
    if (watchStr.empty())
        return usage("Empty list of watches. Quitting.");

    CAccountWatch watch;
    while (watch.parseJson3(watchStr)) {
        // cleanup and report on errors
        watch.color   = convertColor(watch.color);
        watch.address = str_2_Addr(toLower(watch.address));
        watch.nodeBal = getBalanceAt(watch.address, watch.firstBlock-1);
        watch.api_spec.method = toml->getConfigStr("api_spec", "method", "");
        watch.api_spec.uri = toml->getConfigStr("api_spec", "uri", "");
        watch.api_spec.headers = toml->getConfigStr("api_spec", "headers", "");
        if (!watch.api_spec.uri.empty()) {
            watch.abi_spec.loadAbiByAddress(watch.address);
            watch.abi_spec.loadAbiKnown("all");
        }

        string_q msg;
        if (!isAddress(watch.address)) {
            msg = "invalid address " + watch.address;
        }
        if (watch.name.empty()) {
            if (!msg.empty())
                msg += ", ";
            msg += "no name " + watch.name;
        }

        // add to array or return error
        if (msg.empty()) {
            minWatchBlock = min(minWatchBlock, watch.firstBlock);
            maxWatchBlock = max(maxWatchBlock, watch.lastBlock);
            monitors.push_back(watch);

        } else {
            return usage(msg);

        }
        watch = CAccountWatch();  // reset
    }
    name = toml->getConfigStr("settings", "name", monitors[0].name);

    return true;
}

//-------------------------------------------------------------------------
void writeLastBlock(blknum_t bn) {
    if (!isTestMode())
        stringToAsciiFile(getTransCachePath("lastBlock.txt"), uint_2_Str(bn) + "\n");
    else
        cerr << "Would have written lastBlock.txt file: " << bn << endl;
}
