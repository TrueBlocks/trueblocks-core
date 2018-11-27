/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern const string_q fmt;
extern bool processBlock       (blknum_t bn, COptions *options);
extern bool processTransaction (const CBlock& block, const CTransaction *trans, COptions *options);
extern bool processTraces      (const CBlock& block, const CTransaction *trans, const CAccountWatch *acct, COptions *options);
extern void getTraces2         (blknum_t bn, CTraceArray& traces, const hash_t& hashIn);
extern void writeLastBlock     (blknum_t bn);
extern string_q report         (const COptions& options, double start, double stop);
//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    acctlib_init();

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 1;

    cerr << bBlack << Now().Format(FMT_JSON) << cOff << ": Monitoring " << cYellow << getCWD() << cOff << "             \n";

    CStringArray commands;
    explode(commands, options.commandList, '\n');
    if (commands.empty()) commands.push_back("--noop");
    string_q command = commands[0];
    if (!options.parseArguments(command))
        return 1;

    if (!isParity() || !nodeHasTraces()) {
        cerr << "This tool only runs against Parity and only if --tracing is enabled. Quitting..." << endl;
        return 1;
    }

    CToml config("./config.toml");
    if (!fileExists(config.getFilename())) {
        cerr << "Configuration file '" << config.getFilename() << " not found. Quitting." << endl;
        return 1;
    }

    if (!options.loadMonitors(config))
        return 1;

    if (!folderExists("./cache")) {
        cerr << "The cache folder (./cache/) does not exist. Quitting" << endl;
        return 1;
    }

    string_q cacheFileName = "./cache/" + options.monitors[0].address + ".acct.bin";
    if (fileExists(cacheFileName+".lck") || fileExists("./cache/lastBlock.txt.lck")) {
        cerr << "The cache is locked. acctScrape cannot run. Quitting..." << endl;
        return 1;
    }

    string_q bloomPath = blockCachePath("/blooms/");
    if (!folderExists(bloomPath)) {
        cerr << "The bloom file cache '" << bloomPath << " was not found. acctScrape cannot run. Quitting..." << endl;
        return 1;
    }

    double startTime = qbNow();
    if (options.oneBlock) {
        options.firstBlock = options.oneBlock;
        options.nBlocks    = 1;
        string_q fileName = substitute(getBinaryFilename(options.oneBlock), "/blocks/", "/blooms/");
        setenv("TEST_MODE", "true", true);
        options.debugging = (verbose ? 2 : 1);
        visitBloomFilters(fileName, &options);

    } else {
        string_q results;
        asciiFileToString("./cache/lastBlock.txt", results);
        uint64_t blockNum = max(options.minWatchBlock - 1, str_2_Uint(results));
        if (blockNum <= getLatestBlockFromCache()) {  // the cache may be behind the acct db, so don't scrape
            options.lastBlock = min(getLatestBlockFromCache(), options.maxWatchBlock);
            options.firstBlock = min(blockNum, options.lastBlock);
            options.nBlocks = min(options.lastBlock - options.firstBlock, options.maxBlocks);
            if (verbose) {
                cerr << "Visiting blooms between " << options.firstBlock << " and ";
                cerr << options.firstBlock + options.nBlocks << endl;
            }

            //if (options.useAddressIndex) {
            // THIS IS WHERE WE CAN READ THE ADDRESS INDEX WHICH GIVES US A LIST OF BLOCKS
            // AND CALL processBlock(bn, options) DIRECTLY
            //  blocks = for each watch, get the list of blocks
            //  for (auto bn : blocks) {
            //      options->blkStats.nSeen++;
            //      processBlock(bn, &options);
            //  }
            //} else
            {
                if (options.txCache.Lock(cacheFileName, "a+", LOCK_WAIT)) {
                    forEveryBloomFile(visitBloomFilters, &options, options.firstBlock, options.nBlocks);
                    options.txCache.Release();
                }
            }
        }
    }

    cerr << options.name << " bn: " << options.firstBlock + options.nBlocks << options << "\r";
    cerr.flush();

    if (options.blkStats.nSeen > 1000 && options.logLevel > 0) {
        appendToAsciiFile(blockCachePath("blooms/blooms_stats.txt"), options.finalReport(qbNow()-startTime));
    }

    etherlib_cleanup();

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
            if (options->debugging)
                cerr << "Opening bloom file " << path << "\n";

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
                    if (options->debugging) { cerr << "Account " << ac << " ("; }

                    // Visit each bloom until either there is a hit or we've checked all blooms
                    options->monitors.at(ac).inBlock = false;
                    for (size_t bl = 0 ; bl < blooms.size() && !options->monitors[ac].inBlock ; bl++) {
                        bool hit = isBloomHit(options->monitors[ac].bloom, blooms[bl]);
                        if (hit) {
                            options->monitors.at(ac).inBlock = true;
                            hasPotential = true;
                        }

                        // do a little reporting if told to
                        if (options->debugging) {
                            cerr << "bl: " << bl << "-" << (hit ? greenCheck : redX);
                            if (bl < blooms.size()-1)
                                cerr << ",";
                            cerr.flush();
                        }
                    }
                    if (options->debugging) { cerr << ")\n"; }
                }
            }

            // If we hit one or more blooms...we first want to scan the per-block accounts cache
            // to see if we need to descend into the actual block
            if (options->checkAddrs && hasPotential) {
                string_q acctFile = substitute(path, "/blooms/", "/accounts/");
                options->addrStats.nSeen++;
                if (fileExists(acctFile)) {
                    hasPotential = false;
                    CAddressArray addrs;
                    CArchive acctCache(READING_ARCHIVE);
                    if (acctCache.Lock(acctFile, binaryReadOnly, LOCK_NOWAIT)) {
                        acctCache >> addrs;
                        acctCache.Release();

                        if (verbose > 1) {
                            cerr << "Addresses in block:\n";
                            for (size_t a = 0 ; a < addrs.size() ; a++) {
                                cerr << "\t" << addrs[a] << "\n";
                            }
                        }

                        for (size_t ac = 0 ; ac < options->monitors.size() && !hasPotential ; ac++) {
                            address_t addr = options->monitors[ac].address;
                            vector<address_t>::iterator it = find(addrs.begin(), addrs.end(), addr);
                            if (it != addrs.end()) {
                                if (verbose)
                                    cerr << "\tSearching...found " << addr << endl;
                                hasPotential = true;
                            } else {
                                if (verbose)
                                    cerr << "\tSearching...not found " << addr << endl;
                                hasPotential = false;
                            }
                        }
                        if (hasPotential)
                            options->addrStats.nHit++;
                        else
                            options->addrStats.nSkipped++;
                    } else {
                        if (verbose)
                            cerr << "Could not open file " << acctFile << endl;
                        options->addrStats.nSkipped++;
                    }
                } else {
                    if (verbose)
                        cerr << "File " << acctFile << " does not exist." << endl;
                    options->addrStats.nSkipped++;
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

    if (options->debugging)
        cerr << "At least one bloom hit on block " << bn << ". Fetching block." << endl;

    options->blkStats.nQueried++;
    options->blockCounted = false;
    CBlock block;
    if (queryBlock(block, uint_2_Str(bn), false, false)) {

        // Check each transaction for actual involvement.
        if (options->oneTrans && options->oneTrans < block.transactions.size()) {
            // ignore the return, so we can cleanup and write the last block
            processTransaction(block, &block.transactions[options->oneTrans], options);

        } else {
            for (size_t tr = 0; tr < block.transactions.size() ; tr++) {
                // ignore the return, so we can cleanup and write the last block
                bool ret = processTransaction(block, &block.transactions[tr], options);
                if (!ret) {
                    // May be redunant, but it's okay since we're writing a single value
                    writeLastBlock(bn);
                    return false;
                }
            }
        }

    } else {
        // Could not read block, or it had zero transactions. This is a bug. Skip over it next time
        writeLastBlock(bn);
        cerr << "Block " << bn << " had no transactions but a non-zero bloom. This is a bug. Quitting..." << endl;
        return false; // end the search
    }

    return !shouldQuit();
}

#define REP(n, v) if (options->debugging) { cerr << "\t" << padRight(n,8) << ": " << (v) << endl; }
#define DEBUG_PRINT1(a)   if (options->debugging > 1) { cerr << (a) << endl; }
#define DEBUG_PRINT2(a,b) if (options->debugging > 1) { cerr << (a) << (b) << endl; }
//-----------------------------------------------------------------------
bool processTransaction(const CBlock& block, const CTransaction *trans, COptions *options) {

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

                 if (block.miner % watched) { hit = true; REP("miner",    block.miner); }
            else if (trans->to   % watched) { hit = true; REP("to",       trans->to  ); }
            else if (trans->from % watched) { hit = true; REP("to",       trans->from); }
            else if (contrAddr   % watched) { hit = true; REP("contract", contrAddr);   }

            if (!hit) {
                // If we did not find a hit, dig deeper
                if (containsI(trans->input, extended)) {
                    hit = true;
                    REP("input", "..." + extract(trans->input, toLower(trans->input).find(toLower(extended))));

                } else {

                    // Technically, we don't have to check the logs (events can be found in the traces). But, we
                    // want to avoid having to get traces at all costs. The `blockScraper` app must look at every
                    // trace, therefor, it does not check logs. As soon as we hit, we can stop
                    for (size_t lg = 0 ; lg < trans->receipt.logs.size() && !hit ; lg++) {

                        const CLogEntry *l = &trans->receipt.logs[lg];
                             if (l->address % watched       ) { hit = true; REP("l->addr", l->address); }
                        else if (containsI(l->data, extended)) { hit = true; REP("l->data", "..." + extract(l->data, toLower(l->data).find(toLower(extended))))
                        }

                        // Or if we find it as one of the topics. Note, won't spin if we've already hit.
                        for (size_t tp = 1 ; tp < l->topics.size() && !hit ; tp++) {
                            string_q topic = topic_2_Str(l->topics[tp]);
                            if (topic % extended)             { hit = true; REP("l->topic", "..." + extract(topic, toLower(topic).find(toLower(extended)))); }
                        }
                    }
                }

                // If we are still not hit, we must decend into the traces. Sorry...but we must be thourough...
                if (!hit) {
                    DEBUG_PRINT1("Decending into traces");
                    hit = processTraces(block, trans, acct, options);
                }
            }

            if (hit) {

                if (!options->blockCounted) {
                    options->blockCounted = true;
                    options->blkStats.nHit++; // only count the block once per block
                }
                options->transStats.nHit++;

                if (options->debugging)
                    cerr << "Would have written record at " << block.blockNumber << " " << trans->transactionIndex << endl;

                if (!isTestMode()) {
                    // We found something...write it to the cache...
                    lockSection(true);
                    options->txCache << block.blockNumber << trans->transactionIndex;
                    options->txCache.flush();
                    writeLastBlock(block.blockNumber);
                    // Send the data to an api if we have one
                    if (!acct->api_spec.uri.empty()) {
                        if (trans->traces.size() == 0)
                            getTraces(((CTransaction*)trans)->traces, trans->hash);
                        acct->abi_spec.articulateTransaction((CTransaction*)trans);
                        ((CAccountWatch*)acct)->api_spec.sendData(trans->Format());
                        ((CAccountWatch*)acct)->api_spec.sendData("cleanup");
//                        cout << *trans << "\n";
                        cout << "\n";
                        cout.flush();
//                        getchar();
                    }
                    // Also, we optionally write blocks if we're told to do so
                    if (options->writeBlocks) {
                        string_q fn = getBinaryFilename(block.blockNumber);
                        if (!fileExists(fn)) {
                            CBlock *pBlock = (CBlock*)&block;
                            pBlock->finalized = isBlockFinal(block.timestamp, options->lastTimestamp, (60 * 4));
                            writeBlockToBinary(block, fn);
                        }
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
                if (options->debugging)
                    cerr << "\n";
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
        DEBUG_PRINT1("Early exist for dDos");
        options->traceStats.nSeen    += nTraces;
        options->traceStats.nSkipped += nTraces;
        return false;
    }

    CTraceArray traces;
    getTraces2(block.blockNumber, traces, trans->hash); // This call right here is very slow

    DEBUG_PRINT2("nTraces: ", traces.size());
    address_t watched = acct->address;
    for (size_t tc = 0 ; tc < traces.size() ; tc++) {
        options->traceStats.nSeen++;

        const CTraceAction *action = &traces[tc].action;
        const CTraceResult *result = &traces[tc].result;

        bool hit = false;
             if (action->to            % watched) { REP("action->to",   action->to           ); hit = true; }
        else if (action->from          % watched) { REP("action->from", action->from         ); hit = true; }
        else if (action->address       % watched) { REP("action->addr", action->address      ); hit = true; }
        else if (action->refundAddress % watched) { REP("action->ref",  action->refundAddress); hit = true; }
        else if (result->address       % watched) { REP("result->addr", result->address      ); hit = true; }

        if (hit) {
            DEBUG_PRINT1("Trace hit");
            options->traceStats.nHit++;
            return true;
        }
    }

    DEBUG_PRINT1("Trace not hit");
    return false;
}

//-----------------------------------------------------------------------
bool COptions::loadMonitors(const CToml& config) {

    minWatchBlock = UINT32_MAX;
    maxWatchBlock = 0;

    string_q watchStr = config.getConfigStr("watches", "list", "");
    if (watchStr.empty()) {
        cerr << "Empty list of watches. Quitting." << endl;
        return false;
    }

    CAccountWatch watch;
    while (watch.parseJson3(watchStr)) {
        // cleanup and report on errors
        watch.color   = convertColor(watch.color);
        watch.address = str_2_Addr(toLower(watch.address));
        watch.nodeBal = getBalance(watch.address, watch.firstBlock-1, false);
        watch.api_spec.method = config.getConfigStr("api_spec", "method", "");
        watch.api_spec.uri = config.getConfigStr("api_spec", "uri", "");
        watch.api_spec.headers = config.getConfigStr("api_spec", "headers", "");
        if (!watch.api_spec.uri.empty()) {
            watch.abi_spec.loadByAddress(watch.address);
            watch.abi_spec.loadKnownABIs("all");
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

        if (msg.empty()) {
            minWatchBlock = min(minWatchBlock, watch.firstBlock);
            maxWatchBlock = max(maxWatchBlock, watch.lastBlock);
            monitors.push_back(watch);

        } else {
            cerr << msg << endl;
            return false;
        }
        watch = CAccountWatch();  // reset
    }
    name = config.getConfigStr("settings", "name", monitors[0].name);

    return true;
}

//-------------------------------------------------------------------------
void writeLastBlock(blknum_t bn) {
    if (!isTestMode())
        stringToAsciiFile("./cache/lastBlock.txt", uint_2_Str(bn) + "\n");
    else
        cerr << "Would have written lastBlock.txt file: " << bn << endl;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COptions& item) {
    os << bBlack
        << " (bh.bq.bs.nb: "  << item.blkStats.nHit << "/" << item.blkStats.nQueried << "/" << item.blkStats.nSeen << "/" << item.nBlocks
        << ", ah.anh.as: "    << item.addrStats.nHit  << "/" << item.addrStats.nSkipped   << "/" << item.addrStats.nSeen
        << ", th.ts: "        << item.transStats.nHit  << "/" << item.transStats.nSeen
        << ", trh.trsk.trs: " << item.traceStats.nHit << "/" << item.traceStats.nSkipped << "/" << item.traceStats.nSeen << ")" << cOff;
    return os;
}

//-----------------------------------------------------------------------
const string_q fmt = "[{BLOCKNUMBER} ][{w:5:TRANSACTIONINDEX} ][{w:10:DATE} ][{ETHER}]";

//-----------------------------------------------------------------------
string_q COptions::finalReport(double timing) const {
    colorsOff();
    ostringstream os;
    os << monitors[0].name << "\t";
    os << monitors[0].address << "\t";
    os << timing << "\t";
    os << blkStats.nHit << "\t";
    os << blkStats.nQueried << "\t";
    os << blkStats.nSeen << "\t";
    os << nBlocks << "\t";
    os << transStats.nHit << "\t";
    os << transStats.nSeen << "\t";
    os << traceStats.nHit << "\t";
    os << traceStats.nSkipped << "\t";
    os << traceStats.nSeen << "\n";
    return os.str();
}

