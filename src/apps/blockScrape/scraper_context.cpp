/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
CScraper::CScraper(COptions *o, blknum_t num) :
    options(o), status(""), addrList(NULL, NULL, true), traceCount(0), curLines(0),
    maxTraceDepth(0), nAddrsInBlock(0), pTrans(NULL) {
    ASSERT(addrList.addrTxMap);
    block.blockNumber = num;
    resetClock();
}

//-------------------------------------------------------------------------
bool CScraper::scrapeBlock(void) {

    // Query the node for fresh block data
    getObjectViaRPC(block, "eth_getBlockByNumber", "[\"" + uint_2_Hex(block.blockNumber) + "\",true]");

    // Note the miner address
    noteAddress(block.miner, true);

    // Visit each transaction in this block. Note -- do not use forEveryTransaction since
    // that makes a copy and we may have to write the block to disc. For instance, we
    // need to mark pre-byzantium errors.
    for (uint32_t tr = 0 ; tr < block.transactions.size() ; tr++) {
        pTrans = &block.transactions[tr];
        pTrans->pBlock = &block;
        if (!scrapeTransaction())
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool CScraper::scrapeTransaction(void) {

    ASSERT(pTrans != NULL);
    getReceipt(pTrans->receipt, pTrans->hash);

    noteAddress(pTrans->from);
    noteAddress(pTrans->to);
    noteAddress(pTrans->receipt.contractAddress);
    // We heuristically try to catch addresses in the function input data
    foundPotential(notePotential, this, pTrans->blockNumber, pTrans->transactionIndex, 0, extract(pTrans->input, 10));

    for (uint32_t lg = 0 ; lg < pTrans->receipt.logs.size() ; lg++) {
        const CLogEntry *log = &pTrans->receipt.logs[lg];
        noteAddress(log->address);
        string_q addr;
        for (uint32_t tp = 0 ; tp < log->topics.size() ; tp++) {
            // Heuristacally try to pick up any addresses in the topics
            if (isPotentialAddr(log->topics[tp], addr))
                noteAddress(addr);
        }
        // Heuristacally try to pick up any addresses in the log data
        foundPotential(notePotential, this, pTrans->blockNumber, pTrans->transactionIndex, 0, extract(log->data, 2));
    }

    traceCount += getTraceCount(pTrans->hash);

    string_q traces;
    queryRawTrace(traces, pTrans->hash);

    //
    // The following code is very hacky, but it's way faster than parseing every trace, especially for
    // for the dDos transactions from late-2106.
    //

    //
    // queryRawTrace returns all the traces for the transaction directly from the node as JSON. We don't want
    // to parse all that data since we're not going to store it anyway, so we hackily do string searches for
    // what we're looking for. We start with the traceAddress arrays since we need to count them for trace depth.
    // We convert commas into pipes so they don't get tokenized below...
    //
    string_q search = "traceAddress";
    size_t pos = traces.find(search);
    while (pos != string::npos) {
        pos += search.size();
        while (traces[pos] != ']') {
            if (traces[pos] == ',')
                traces[pos] = '|';
            pos++;
        }
        pos = traces.find(search, pos + search.size());
    }

    // This code prepares the string for tokenizing the fields in the JSON
    for (char& ch : traces) {
        if (ch == '\"') ch = ' ';
        if (ch == ',') ch = '+';
        if (ch == '{') ch = '+';
        if (ch == '}') ch = ' ';
    }

    CStringArray fields;
    fields.reserve(countOf(traces, '+') + 10); // extra room

    explode(fields, traces, '+');
    bool hasError = false;
    for (auto v : fields) {
        v = trim(v);

#define VV(s) { \
    string_q ss(s); \
    ss += " : "; \
    if (startsWith(v, ss)) \
        noteAddress(v.substr(ss.size())); \
}

#define VV2(s,n) { \
    string_q ss(s); \
    ss += " : "; \
    if (startsWith(v, ss)) { \
        size_t sz = min(v.size(), ss.size() + n); \
        ss = v.substr(sz); \
        if (!isZeroAddr(ss)) \
            potList += ss; \
    } \
}

        VV("from");
        VV("to");
        VV("address"); // both from trace.action and trace.result
        VV("refundAddress");

        potList = "";
        VV2("input", 10);
        VV2("output", 2);
        foundPotential(notePotential, this, pTrans->blockNumber, pTrans->transactionIndex, 0, potList);

        if (startsWith(v, "traceAddress"))
            maxTraceDepth = max(maxTraceDepth, (uint64_t)(countOf(v, '|') + 1));

        if (startsWith(v, "error"))
            hasError = true;

        // We ignore 'code' and 'init'
        // if (startsWith(v, "code") cout << v << endl;
        // if (startsWith(v, "init") cout << v << endl;
    }

    // If we're writing blocks, we need an error code on the transaction
    if (options->writeBlocks) {
        if (block.blockNumber < byzantiumBlock) {
            pTrans->isError = hasError;
        } else {
            pTrans->isError = (pTrans->receipt.status == 0); // status contains zero on fail
        }
    }

    return true;
}

#define lastPrecompile address_t("0x0000000000000000000000000000000000000008")
//----------------------------------------------------------------------------------
bool isPrecompile(const address_t& addr) {
    return (addr <= lastPrecompile);
}

//----------------------------------------------------------------------------------
void CScraper::noteAddress(const address_t& addr, bool isMiner) {

    // We don't store zero address since it has special meaning and it appears many, many, many times
    // all over the place. It would blow up the size of the index and have almost no meaning to query
    if (isZeroAddr(addr))
        return;

    // We don't store pre-compiles for similar reasons, but more importantly because Parity has a bug that
    // misreports 'calldata' and 'calldelegate' to pre-compile 0x000....004. This eliminates that bug and
    // save a ton of space in the index. It disallows querying for pre-comile addresses.
    if (isPrecompile(addr))
        return;

    nAddrsInBlock++;

    CAddressAppearance app;
    app.addr = addr;
    app.bn = block.blockNumber;
    if (isMiner) {
        app.tx = 99999;
    } else {
        ASSERT(pTrans);
        app.tx = pTrans->transactionIndex;
    }
    ASSERT(addrList.addrTxMap);
    addrList.insertUnique(app);
}

//--------------------------------------------------------------------------
bool CScraper::writeList(const string_q& toFile, const string_q& removeFile) {

    ASSERT(addrList.addrTxMap);

    ostringstream os;
    CAddressTxAppearanceMap::iterator it;
    for (it = addrList.addrTxMap->begin(); it != addrList.addrTxMap->end(); it++ ) {
        os << it->first.addr << "\t";
        os << padNum9(it->first.bn) << "\t";
        os << padNum5(it->first.tx) << "\n";
    }
    stringToAsciiFile(toFile, os.str());
    if (!removeFile.empty() && fileExists(removeFile))
        ::remove(removeFile.c_str());

    return true;
}

//--------------------------------------------------------------------------
bool CScraper::stageList(void) {
    string_q stagingName = indexFolder_staging_v2 + padNum9(block.blockNumber) + ".txt";
    return writeList(stagingName, "");
}

//--------------------------------------------------------------------------
bool CScraper::finalizeList(void) {

    //
    // Write the per-block address list to the finalized folder and keep track of how
    // many items we've stored so far.
    //
    string_q stagingName = indexFolder_staging_v2 + padNum9(block.blockNumber) + ".txt";
    string_q finalName = indexFolder_finalized_v2 + padNum9(block.blockNumber) + ".txt";
    if (!writeList(finalName, stagingName))
        return false;

    //string_q countFile = configPath("cache/tmp/scrape_count.tmp");
    string_q countFile = indexFolder_v2 + "counts.txt";
    curLines = str_2_Uint(asciiFileToString(countFile));
    curLines += addrList.addrTxMap->size();
    stringToAsciiFile(countFile, uint_2_Str(curLines));

    bool overLimit = curLines >= options->maxIndexRows;
    bool is50 = !(block.blockNumber % 50);
    if ((overLimit && is50)) {
        consolidateIndex();
//        return false;
    }

    return true;
}

//--------------------------------------------------------------------------
void CScraper::consolidateIndex(void) {

    blknum_t first = str_2_Uint(substitute(getFirstFileInFolder(indexFolder_finalized_v2, false), indexFolder_finalized_v2, ""));
    blknum_t last  = str_2_Uint(substitute(getLastFileInFolder (indexFolder_finalized_v2, false), indexFolder_finalized_v2, ""));

    string_q resFile = indexFolder_sorted_v2 + padNum9(first)+"-"+padNum9(last) + ".txt";

    CStringArray apps;
    apps.reserve(options->maxIndexRows + 100);

    cerr << "Processing index.";
    cerr.flush();

    for (blknum_t i = first ; i <= last ; i++) {
        string_q theStuff;
        string_q fn = indexFolder_finalized_v2 + padNum9(i) + ".txt";
        asciiFileToString(fn, theStuff);
        CStringArray lns;
        explode(lns, theStuff, '\n');
        for (auto ln : lns) {
            apps.push_back(ln);
            if (!(apps.size() % (options->maxIndexRows / 10))) {
                cerr << "."; cerr.flush();
            }
        }
    }

    sort(apps.begin(), apps.end());

    size_t cnt = 0;
    ostringstream os;
    for (auto app : apps) {
        os << app << "\n";
        if (!(++cnt % (options->maxIndexRows / 10))) {
            cerr << "."; cerr.flush();
        }
    }
    appendToAsciiFile(resFile, os.str());

    string_q countFile = indexFolder_v2 + "counts.txt";
    ::remove(countFile.c_str());
    cnt = 0;
    cerr << "\ncleaning up"; cerr.flush();
    for (blknum_t i = first ; i <= last ; i++) {
        string_q fn = indexFolder_finalized_v2 + padNum9(i) + ".txt";
        ::remove(fn.c_str());
        if (!(++cnt % (options->maxIndexRows / 10))) {
            cerr << "."; cerr.flush();
        }
    }
    putc(7,stdout);
}

//-------------------------------------------------------------------------
bool notePotential(const CAddressAppearance& item, void *data) {
    ((CScraper*)data)->noteAddress(item.addr);
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
    double age = double(options->latestBlockTs - block.timestamp) / 60.;

    cerr << ((block.finalized ? greenCheck : whiteStar) + " ");
    cerr << padRight(uint_2_Str(last - block.blockNumber), 4) << ": ";

    ostringstream os; os.precision(4);
    os << fixed << setprecision(3);
    os << bBlack;
    os << blkDate.Format(FMT_EXPORT)  << "\t";
    os << age                         << "\t";
    os << TIC()                       << "\t" << cYellow;
    os << block.blockNumber           << "\t";
    os << block.transactions.size()   << "\t";
    os << traceCount                  << "\t";
    os << maxTraceDepth               << "\t";
    os << nAddrsInBlock               << "\t";
    os << addrList.addrTxMap->size()  << "\t";
    os << padNum6(curLines)           << "\t" << cTeal;
    os << status                      << cOff;

    return os.str();
}
