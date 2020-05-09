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
#include <string>
#include "node.h"
#include "filenames.h"

namespace qblocks {

// TODO(tjayrush): global data
static QUITHANDLER theQuitHandler = NULL;
//-------------------------------------------------------------------------
void etherlib_init(QUITHANDLER qh) {
    {
        // Keep the frame
        mutex aMutex;
        lock_guard<mutex> lock(aMutex);
        // This global is okay to be program-wide (as opposed to thread wide)
        // since we should only ever init curl once for any number of threads.
        static bool been_here = false;
        if (!been_here) {
            curl_global_init(CURL_GLOBAL_ALL);
            been_here = true;
        }
    }

    extern void loadParseMap(void);
    loadParseMap();

    setDataSource("binary");

    // if curl has already been initialized, we want to clear it out
    getCurlContext()->releaseCurl();
    // initialize curl
    getCurlContext()->getCurl();

    checkNodeRequired();

    // If we create any lock files, we need to clean them up
    if (theQuitHandler == NULL || qh != defaultQuitHandler) {
        // Set this once, unless it's non-default
        theQuitHandler = qh;
        registerQuitHandler(qh);
    }

    CBlock::registerClass();
    CTransaction::registerClass();
    CReceipt::registerClass();
    CLogEntry::registerClass();
    CPriceQuote::registerClass();
    CTrace::registerClass();
    CTraceAction::registerClass();
    CTraceResult::registerClass();
    CAbi::registerClass();
    CFunction::registerClass();
    CParameter::registerClass();

    CAppearance::registerClass();
    CRPCResult::registerClass();
    CAccountName::registerClass();
    CCollection::registerClass();
    CCacheEntry::registerClass();

    establishFolder(configPath(""));
    establishFolder(getCachePath(""));
}

//-------------------------------------------------------------------------
void etherlib_cleanup(void) {
    {
        // Keep the frame
        mutex aMutex;
        lock_guard<mutex> lock(aMutex);
        // This global is okay to be program-wide (as opposed to thread wide)
        // since we should only ever init curl once for any number of threads.
        static bool been_here = false;
        if (!been_here) {
            curl_global_cleanup();
            been_here = true;
        }
    }

    getCurlContext()->releaseCurl();
    if (theQuitHandler)
        (*theQuitHandler)(-1);
    else
        cleanFileLocks();
}

//-------------------------------------------------------------------------
bool getObjectViaRPC(CBaseNode& node, const string_q& method, const string_q& params) {
    string_q str = callRPC(method, params, false);
    return node.parseJson3(str);
}

//--------------------------------------------------------------------------------
bool getBlock_light(CBlock& block, const string_q& val) {
    if (str_2_Uint(val) != 0 && fileSize(getBinaryCacheFilename(CT_BLOCKS, str_2_Uint(val))) > 0)
        return readBlockFromBinary(block, getBinaryCacheFilename(CT_BLOCKS, str_2_Uint(val)));
    block.light = true;
    getObjectViaRPC(block, "eth_getBlockByNumber", "[" + quote(val) + ",false]");
    return true;
}

//--------------------------------------------------------------------------------
bool getBlock_light(CBlock& block, blknum_t num) {
    if (fileSize(getBinaryCacheFilename(CT_BLOCKS, num)) > 0)
        return readBlockFromBinary(block, getBinaryCacheFilename(CT_BLOCKS, num));
    return getBlock_light(block, uint_2_Hex(num));
}

//--------------------------------------------------------------------------------
bool getBlock_header(CBlock& block, const string_q& val) {
    getObjectViaRPC(block, "parity_getBlockHeaderByNumber", "[" + quote(val) + "]");
    return true;
}

//--------------------------------------------------------------------------------
bool getBlock_header(CBlock& block, blknum_t bn) {
    if (fileSize(getBinaryCacheFilename(CT_BLOCKS, bn)) > 0)
        return readBlockFromBinary(block, getBinaryCacheFilename(CT_BLOCKS, bn));
    return getBlock_header(block, uint_2_Hex(bn));
}

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, blknum_t blockNum) {
    string_q prev = setDataSource(fileExists(getBinaryCacheFilename(CT_BLOCKS, blockNum)) ? "binary" : "local");
    bool ret = queryBlock(block, uint_2_Str(blockNum), true);
    setDataSource(prev);
    return ret;
}

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, const hash_t& blockHash) {
    return queryBlock(block, blockHash, true);
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, blknum_t blockNum, txnum_t txid) {
    if (fileExists(getBinaryCacheFilename(CT_TXS, blockNum, txid))) {
        readTransFromBinary(trans, getBinaryCacheFilename(CT_TXS, blockNum, txid));
        trans.pBlock = NULL;  // otherwise, it's pointing to an unintialized item
        trans.finishParse();  // set the pointer for the receipt
        return true;
    }

    getObjectViaRPC(trans, "eth_getTransactionByBlockNumberAndIndex",
                    "[\"" + uint_2_Hex(blockNum) + "\",\"" + uint_2_Hex(txid) + "\"]");
    trans.finishParse();  // set the pointer for the receipt
    return true;
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const hash_t& txHash) {
    getObjectViaRPC(trans, "eth_getTransactionByHash", "[\"" + str_2_Hash(txHash) + "\"]");
    trans.finishParse();
    return true;
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const hash_t& blockHash, txnum_t txid) {
    getObjectViaRPC(trans, "eth_getTransactionByBlockHashAndIndex",
                    "[\"" + str_2_Hash(blockHash) + "\",\"" + uint_2_Hex(txid) + "\"]");
    trans.finishParse();
    return true;
}

//-----------------------------------------------------------------------
bool writeNodeToBinary(const CBaseNode& node, const string_q& fileName) {
    string_q created;
    if (establishFolder(fileName, created)) {
        if (!created.empty() && !isTestMode())
            cerr << "mkdir(" << created << ")" << string_q(75, ' ') << "\n";
        CArchive nodeCache(WRITING_ARCHIVE);
        if (nodeCache.Lock(fileName, modeWriteCreate, LOCK_CREATE)) {
            node.SerializeC(nodeCache);
            nodeCache.Close();
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------
bool readNodeFromBinary(CBaseNode& item, const string_q& fileName) {
    // Assumes that the item is clear, so no Init
    CArchive nodeCache(READING_ARCHIVE);
    if (nodeCache.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        item.Serialize(nodeCache);
        nodeCache.Close();
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------
bool getReceipt(CReceipt& receipt, const hash_t& txHash) {
    receipt = CReceipt(receipt.pTrans);
    getObjectViaRPC(receipt, "eth_getTransactionReceipt", "[\"" + str_2_Hash(txHash) + "\"]");
    return true;
}

//--------------------------------------------------------------
void getTraces(CTraceArray& traces, const hash_t& hash) {
    traces.clear();

    string_q str;
    queryRawTrace(str, hash);

    CRPCResult generic;
    generic.parseJson3(str);  // pull out the result

    generic.result = cleanUpJson((char*)generic.result.c_str());  // NOLINT
    CTrace trace;
    while (trace.parseJson4(generic.result)) {
        traces.push_back(trace);
        trace = CTrace();  // reset
    }
}

//-----------------------------------------------------------------------
bool loadTraces(CTransaction& trans, blknum_t bn, blknum_t txid, bool useCache, bool skipDdos) {
    string_q trcFilename = getBinaryCacheFilename(CT_TRACES, bn, txid, "");
    if (useCache && fileExists(trcFilename)) {
        CArchive traceCache(READING_ARCHIVE);
        if (traceCache.Lock(trcFilename, modeReadOnly, LOCK_NOWAIT)) {
            traceCache >> trans.traces;
            traceCache.Release();
        }

    } else {
        if (skipDdos) {
            CTrace dDos;
            dDos.loadAsDdos(trans, bn, txid);
            trans.traces.push_back(dDos);

        } else if (txid == 99997 || txid == 99998 || txid == 99999) {
            CTrace blockReward;
            blockReward.loadAsBlockReward(trans, bn, txid);
            trans.traces.push_back(blockReward);

            if (txid == 99999) {
                CTrace txFee;
                txFee.loadAsTransactionFee(trans, bn, txid);
                trans.traces.push_back(txFee);
            }

        } else {
            getTraces(trans.traces, trans.getValueByName("hash"));
        }

        // Write traces if we're told to and there are traces. Remember: every transaction has at
        // least one trace, so if we've gotten here without traces, then the node is not a tracing node.
        // Don't write anything in that case.
        if (useCache && trans.traces.size() > 0) {
            establishFolder(trcFilename);
            CArchive traceCache(WRITING_ARCHIVE);
            if (traceCache.Lock(trcFilename, modeWriteCreate, LOCK_NOWAIT)) {
                traceCache << trans.traces;
                traceCache.Release();
            }
        }
    }
    return true;
}

//-------------------------------------------------------------------------
bool getFullReceipt(CTransaction* trans, bool needsTrace) {
    getReceipt(trans->receipt, trans->hash);
    if (trans->blockNumber >= byzantiumBlock) {
        trans->isError = (trans->receipt.status == 0);

    } else if (needsTrace && trans->gas == trans->receipt.gasUsed) {
        CURLCALLBACKFUNC prev = getCurlContext()->setCurlCallback(errorCallback);
        getCurlContext()->is_error = false;
        string_q unused;
        queryRawTrace(unused, trans->hash);
        trans->isError = getCurlContext()->is_error;
        getCurlContext()->setCurlCallback(prev);
    }
    return true;
}

//-------------------------------------------------------------------------
bool queryBlock(CBlock& block, const string_q& datIn, bool needTrace) {
    if (datIn == "latest")
        return queryBlock(block, uint_2_Str(getLatestBlock_client()), needTrace);

    if (isHash(datIn)) {
        getObjectViaRPC(block, "eth_getBlockByHash", "[" + quote(datIn) + ",true]");

    } else {
        uint64_t num = str_2_Uint(datIn);
        if (getDataSource() == "binary" && fileSize(getBinaryCacheFilename(CT_BLOCKS, num)) > 0) {
            block = CBlock();
            return readBlockFromBinary(block, getBinaryCacheFilename(CT_BLOCKS, num));
        }

        getObjectViaRPC(block, "eth_getBlockByNumber", "[" + quote(uint_2_Hex(num)) + ",true]");
    }

    // If there are no transactions, we do not have to trace and we want to tell the caller that
    if (!block.transactions.size())
        return false;

    // We have the transactions, but we also want the receipts, and we need an error indication
    for (size_t i = 0; i < block.transactions.size(); i++) {
        CTransaction* trans = &block.transactions.at(i);  // taking a non-const reference
        trans->pBlock = &block;
        getFullReceipt(trans, needTrace);
    }

    return true;
}

//-------------------------------------------------------------------------
bool queryRawBlock(string_q& blockStr, const string_q& datIn, bool needTrace, bool hashesOnly) {
    if (isHash(datIn)) {
        blockStr =
            callRPC("eth_getBlockByHash", "[" + quote(datIn) + "," + (hashesOnly ? "false" : "true") + "]", true);
    } else {
        blockStr = callRPC("eth_getBlockByNumber",
                           "[" + quote(str_2_Hex(datIn)) + "," + (hashesOnly ? "false" : "true") + "]", true);
    }
    return true;
}

//-------------------------------------------------------------------------
string_q getRawBlock(blknum_t bn) {
    string_q numStr = uint_2_Str(bn);

    string_q str;
    queryRawBlock(str, numStr, true, false);

    CRPCResult generic;
    generic.parseJson3(str);

    return generic.result;
}

//-------------------------------------------------------------------------
hash_t getRawBlockHash(blknum_t bn) {
    string_q blockStr;
    queryRawBlock(blockStr, uint_2_Str(bn), false, true);
    blockStr = substitute(extract(blockStr, blockStr.find("\"hash\":"), blockStr.length()), "\"hash\":\"", "");
    blockStr = nextTokenClear(blockStr, '\"');
    return blockStr;
}

//-------------------------------------------------------------------------
hash_t getRawTransactionHash(blknum_t bn, txnum_t tx) {
    return "Not implemented";
}

//-------------------------------------------------------------------------
bool queryRawTransaction(string_q& results, const hash_t& txHash) {
    string_q data = "[\"[HASH]\"]";
    replace(data, "[HASH]", txHash);
    results = callRPC("eth_getTransactionByHash", data, true);
    return true;
}

//-------------------------------------------------------------------------
bool queryRawReceipt(string_q& results, const hash_t& txHash) {
    string_q data = "[\"[HASH]\"]";
    replace(data, "[HASH]", txHash);
    results = callRPC("eth_getTransactionReceipt", data, true);
    return true;
}

//-------------------------------------------------------------------------
bool queryRawTrace(string_q& trace, const string_q& hashIn) {
    trace = "[" + callRPC("trace_transaction", "[\"" + str_2_Hash(hashIn) + "\"]", true) + "]";
    return true;
}

//-------------------------------------------------------------------------
bool queryRawLogs(string_q& results, hash_t hash, const address_t& addr, const CTopicArray& topics) {
    string_q data = "[{\"blockHash\":\"[HASH]\",\"address\":\"[ADDR]\"}]";
    //        string_q data = "[{\"blockHash\":\"[HASH]\",\"address\":\"[ADDR]\",\"topics\":[[TOPICS]]}]";
    replace(data, "[HASH]", hash);
    if (addr.empty())
        replace(data, ",\"address\":\"[ADDR]\"", "");
    else
        replace(data, "[ADDR]", addr_2_Str(addr));
    //        replace(data, "[TOPICS]", topic_2_Str(topics));
    results = callRPC("eth_getLogs", data, true);
    return true;
}

//-------------------------------------------------------------------------
bool queryRawLogs(string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr,
                  const CTopicArray& topics) {
    string_q data = "[{\"fromBlock\":\"[START]\",\"toBlock\":\"[STOP]\",\"address\":\"[ADDR]\"}]";
    //        string_q data =
    //        "[{\"fromBlock\":\"[START]\",\"toBlock\":\"[STOP]\",\"address\":\"[ADDR]\",\"topics\":[[TOPICS]]}]";
    replace(data, "[START]", uint_2_Hex(fromBlock));
    replace(data, "[STOP]", uint_2_Hex(toBlock));
    if (addr.empty())
        replace(data, ",\"address\":\"[ADDR]\"", "");
    else
        replace(data, "[ADDR]", addr_2_Str(addr));
    //        replace(data, "[TOPICS]", topic_2_Str(topics));
    results = callRPC("eth_getLogs", data, true);
    return true;
}

//-------------------------------------------------------------------------
string_q getVersionFromClient(void) {
    return callRPC("web3_clientVersion", "[]", false);
}

//-------------------------------------------------------------------------
bool getAccounts(CAddressArray& addrs) {
    string_q results = callRPC("eth_accounts", "[]", false);
    while (!results.empty())
        addrs.push_back(toLower(nextTokenClear(results, ',')));
    return true;
}

//--------------------------------------------------------------------------
bool getChainHead(void) {
    return false;  // callRPC("parity_chainStatus", "[]", false);
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_final(void) {
    string_q finLast = getLastFileInFolder(indexFolder_finalized, false);
    if (!finLast.empty()) {
        // Files in this folder are n-m.bin
        blknum_t last;
        timestamp_t unused;
        bnFromPath(finLast, last, unused);
        return last;
    }
    return 0;
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_staging(void) {
    string_q stageLast = getLastFileInFolder(indexFolder_staging, false);
    // Files in this folder are n.txt, if empty, we fall back on finalized folder
    if (!stageLast.empty())
        return bnFromPath(stageLast);
    return getLatestBlock_cache_final();
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_ripe(void) {
    string_q ripeLast = getLastFileInFolder(indexFolder_ripe, false);
    // Files in this folder are n.txt, if empty, we fall back on staging folder
    if (!ripeLast.empty())
        return bnFromPath(ripeLast);
    return getLatestBlock_cache_staging();
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_unripe(void) {
    string_q unripeLast = getLastFileInFolder(indexFolder_unripe, false);
    // Files in this folder are n.txt, if empty, we fall back on ripe folder
    if (!unripeLast.empty())
        return bnFromPath(unripeLast);
    return getLatestBlock_cache_ripe();
}

//-------------------------------------------------------------------------
blknum_t getLatestBlock_client(void) {
    string_q ret = callRPC("eth_blockNumber", "[]", false);
    uint64_t retN = str_2_Uint(ret);
    if (retN == 0) {
        // Try a different way just in case. Geth, for example, doesn't
        // return blockNumber until the chain is synced (Parity may--don't know
        // We fall back to this method just in case
        string_q str = callRPC("eth_syncing", "[]", false);
        replace(str, "currentBlock:", "|");
        nextTokenClear(str, '|');
        str = nextTokenClear(str, ',');
        retN = str_2_Uint(str);
    }
    return retN;
}

//--------------------------------------------------------------------------
bool getLatestBlocks(blknum_t& unripe, blknum_t& ripe, blknum_t& staging, blknum_t& finalized, blknum_t& client) {
    ripe = getLatestBlock_cache_ripe();
    unripe = getLatestBlock_cache_unripe();
    staging = getLatestBlock_cache_staging();
    finalized = getLatestBlock_cache_final();
    client = (isNodeRunning() ? getLatestBlock_client() : NOPOS);
    return true;
}

//-------------------------------------------------------------------------
uint64_t addFilter(address_t addr, const CTopicArray& topics, blknum_t num) {
    // Creates a filter object, based on filter options, to notify when the state changes (logs). To check if the state
    // has changed, call eth_getFilterChanges.
    //
    // A note on specifying topic filters:
    // Topics are order-dependent. A transaction with a log with topics [A, B] will be matched by the following topic
    // filters:
    //
    // [] "anything"
    // [A] "A in first position (and anything after)"
    // [null, B] "anything in first position AND B in second position (and anything after)"
    // [A, B] "A in first position AND B in second position (and anything after)"
    // [[A, B], [A, B]] "(A OR B) in first position AND (A OR B) in second position (and anything after)"
    //
    // Parameters
    //  Object - The filter options:
    //  fromBlock:  QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined
    //  block or "pending",
    //                  "earliest" for not yet mined transactions.
    //  toBlock:    QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined
    //  block or "pending",
    //                  "earliest" for not yet mined transactions.
    //  address:    DATA|Array, 20 Bytes - (optional) Contract address or a list of addresses from which logs should
    //  originate. topics:     Array of DATA, - (optional) Array of 32 Bytes DATA topics. Topics are order-dependent.
    //  Each topic can also be an
    //                  array of DATA with "or" options.
    //  params: [{
    //      "fromBlock": "0x1",
    //      "toBlock": "0x2",
    //      "address": "0x8888f1f195afa192cfee860698584c030f4c9db1",
    //      "topics": ["0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b",
    //                  null,
    //                  [ "0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b",
    //                          "0x0000000000000000000000000aff3454fce5edbc8cca8697c15331677e6ebccc"]]
    //      }]
    //  Returns QUANTITY - A filter id.
    return 0;
}

//-------------------------------------------------------------------------
bool nodeHasTraces(void) {
    // At block 50871 (firstTraceBlock) transaction 0, (hash:
    // 0x6df0b4a0d15ae3b925b9819646a0cff4d1bc0a53b294c0d84d884865302d13a5) we know there were exactly 23 traces as per
    // Parity. We check that here to see if the node is running with --tracing enabled. Not sure how this works with
    // Geth
    size_t count = getTraceCount("0x6df0b4a0d15ae3b925b9819646a0cff4d1bc0a53b294c0d84d884865302d13a5");
    return (count == 23);
}

//-------------------------------------------------------------------------
bool hasTraceAt(const string_q& hashIn, size_t where) {
    string_q cmd = "[\"" + str_2_Hash(hashIn) + "\",[\"" + uint_2_Hex(where) + "\"]]";
    string_q ret = callRPC("trace_get", cmd.c_str(), true);
    return ret.find("blockNumber") != string::npos;
}

//--------------------------------------------------------------
size_t getTraceCount_binarySearch(const hash_t& hashIn, size_t first, size_t last) {
    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        bool atMid = hasTraceAt(hashIn, mid);
        bool atMid1 = hasTraceAt(hashIn, mid + 1);
        if (atMid && !atMid1)
            return mid;  // found it
        if (!atMid) {
            // we're too high, so search below
            return getTraceCount_binarySearch(hashIn, first, mid - 1);
        }
        // we're too low, so search above
        return getTraceCount_binarySearch(hashIn, mid + 1, last);
    }
    return first;
}

// https://ethereum.stackexchange.com/questions/9883/why-is-my-node-synchronization-stuck
// -extremely-slow-at-block-2-306-843/10453
//--------------------------------------------------------------
size_t getTraceCount(const hash_t& hashIn) {
    // handle most likely cases linearly
    for (size_t n = 2; n < 8; n++) {
        if (!hasTraceAt(hashIn, n))
            return n - 1;
    }

    // binary search the rest
    if (!hasTraceAt(hashIn, (1 << 8))) {  // small?
        return getTraceCount_binarySearch(hashIn, 0, (1 << 8) - 1);
    } else if (!hasTraceAt(hashIn, (1 << 16))) {  // medium?
        return getTraceCount_binarySearch(hashIn, 0, (1 << 16) - 1);
    } else {
        return getTraceCount_binarySearch(hashIn, 0, (1 << 30));
    }
    return 0;
}

//-----------------------------------------------------------------------
void writeToJson(const CBaseNode& node, const string_q& fileName) {
    if (establishFolder(fileName)) {
        std::ofstream out(fileName.c_str());
        out << node.Format() << "\n";
        out.close();
    }
}

//-----------------------------------------------------------------------
bool readFromJson(CBaseNode& node, const string_q& fileName) {
    if (!fileExists(fileName)) {
        cerr << "File not found " << fileName << "\n";
        return false;
    }
    // assume the item is already clear
    string_q contents;
    asciiFileToString(fileName, contents);
    if (contains(contents, "null")) {
        replaceAll(contents, "null", "\"0x\"");
        stringToAsciiFile(fileName, contents);
    }
    if (!endsWith(contents, '\n')) {
        stringToAsciiFile(fileName, contents + "\n");
    }
    return node.parseJson3(contents);
}

//----------------------------------------------------------------------------------
bool readBloomFromBinary(CBloomArray& blooms, const string_q& fileName) {
    blooms.clear();
    CArchive bloomCache(READING_ARCHIVE);
    if (bloomCache.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        bloomCache >> blooms;
        bloomCache.Close();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
bool writeBloomArray(const CBloomArray& blooms, const string_q& fileName) {
    if (blooms.size() == 0 || (blooms.size() == 1 && blooms[0] == 0))
        return false;

    string_q created;
    if (establishFolder(fileName, created)) {
        if (!created.empty() && !isTestMode())
            cerr << "mkdir(" << created << ")" << string_q(75, ' ') << "\n";
        CArchive bloomCache(WRITING_ARCHIVE);
        if (bloomCache.Lock(fileName, modeWriteCreate, LOCK_CREATE)) {
            bloomCache << blooms;
            bloomCache.Close();
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------------
static string_q getFilename_local(cache_t type, const string_q& bn, const string_q& txid, const string_q& tcid,
                                  bool asPath) {
    ostringstream os;
    switch (type) {
        case CT_BLOCKS:
            os << "blocks/";
            break;
        case CT_BLOOMS:
            os << "blooms/";
            break;
        case CT_TXS:
            os << "txs/";
            break;
        case CT_TRACES:
            os << "traces/";
            break;
        case CT_ACCTS:
            os << "accts/";
            break;
        case CT_MONITORS:
            os << "monitors/";
            break;
        default:
            ASSERT(0);  // should not happen
    }
    if (type == CT_ACCTS || type == CT_MONITORS) {
        string_q addr = toLower(substitute(bn, "0x", ""));
        os << extract(addr, 0, 4) << "/" << extract(addr, 4, 4) << "/" << addr << ".bin";

    } else {
        os << extract(bn, 0, 2) << "/" << extract(bn, 2, 2) << "/" << extract(bn, 4, 2) << "/";
        if (!asPath) {
            os << bn;
            os << ((type == CT_TRACES || type == CT_TXS) ? "-" + txid : "");
            os << (type == CT_TRACES && !tcid.empty() ? "-" + tcid : "");
            os << ".bin";
        }
    }
    return getCachePath(os.str());
}

//-------------------------------------------------------------------------
string_q getBinaryCachePath(cache_t type, blknum_t bn, txnum_t txid, const string_q& trc_id) {
    return getFilename_local(type, padNum9(bn), padNum5(txid), trc_id, true);
}

//-------------------------------------------------------------------------
string_q getBinaryCacheFilename(cache_t type, blknum_t bn, txnum_t txid, const string_q& trc_id) {
    return getFilename_local(type, padNum9(bn), padNum5(txid), trc_id, false);
}

//-------------------------------------------------------------------------
string_q getBinaryCachePath(cache_t type, const address_t& addr) {
    return getFilename_local(type, addr, "", "", true);
}

//-------------------------------------------------------------------------
string_q getBinaryCacheFilename(cache_t type, const address_t& addr) {
    return getFilename_local(type, addr, "", "", false);
}

//-------------------------------------------------------------------------
bool forEveryBlock(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip) {
    // Here we simply scan the numbers and either read from disc or query the node
    if (!func)
        return false;

    for (uint64_t i = start; i < start + count - 1; i = i + skip) {
        string_q fileName = getBinaryCacheFilename(CT_BLOCKS, i);
        CBlock block;
        if (fileExists(fileName)) {
            block = CBlock();
            readBlockFromBinary(block, fileName);
        } else {
            getBlock(block, i);
        }

        bool ret = (*func)(block, data);
        if (!ret) {
            // Cleanup and return if user tells us to
            return false;
        }
    }
    return true;
}

//-------------------------------------------------------------------------
bool forEveryBlock(BLOCKVISITFUNC func, void* data, const string_q& block_list) {
    return true;
}

//-------------------------------------------------------------------------
bool forEveryBlockOnDisc(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip) {
    if (!func)
        return false;

    // Read every block from number start to start+count
    for (uint64_t i = start; i < start + count; i = i + skip) {
        CBlock block;
        getBlock(block, i);
        if (!(*func)(block, data))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool forEveryTraceInTransaction(TRACEVISITFUNC func, void* data, const CTransaction& trans) {
    if (!func)
        return false;

    CTraceArray traces;
    getTraces(traces, trans.hash);
    for (size_t i = 0; i < traces.size(); i++) {
        CTrace trace = traces[i];
        if (!(*func)(trace, data))
            return false;
    }

    return true;
}

//-------------------------------------------------------------------------
bool forEveryTraceInBlock(TRACEVISITFUNC func, void* data, const CBlock& block) {
    for (size_t i = 0; i < block.transactions.size(); i++) {
        if (!forEveryTraceInTransaction(func, data, block.transactions[i]))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool forEveryLogInTransaction(LOGVISITFUNC func, void* data, const CTransaction& trans) {
    if (!func)
        return false;

    for (size_t i = 0; i < trans.receipt.logs.size(); i++) {
        CLogEntry log = trans.receipt.logs[i];
        if (!(*func)(log, data))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool forEveryLogInBlock(LOGVISITFUNC func, void* data, const CBlock& block) {
    for (size_t i = 0; i < block.transactions.size(); i++) {
        if (!forEveryLogInTransaction(func, data, block.transactions[i]))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool forEveryTransactionInBlock(TRANSVISITFUNC func, void* data, const CBlock& block) {
    if (!func)
        return false;

    for (size_t i = 0; i < block.transactions.size(); i++) {
        CTransaction trans = block.transactions[i];
        if (!(*func)(trans, data))
            return false;
    }

    return true;
}

//-------------------------------------------------------------------------
bool forEveryTransactionInList(TRANSVISITFUNC func, void* data, const string_q& trans_list) {
    if (!func)
        return false;

    // trans_list is a list of tx_hash, blk_hash.tx_id, or blk_num.tx_id, or any combination
    string_q list = trans_list;
    while (!list.empty()) {
        string_q item = nextTokenClear(list, '|');
        string_q orig = item;
        bool hasDot = contains(item, ".");
        bool hasHex = startsWith(item, "0x");

        string_q hash = nextTokenClear(item, '.');
        uint64_t txid = str_2_Uint(item);

        bool fromCache = false;
        CTransaction trans;
        if (hasHex) {
            if (hasDot) {
                LOG4("blockHash.txid", hash, txid);
                getTransaction(trans, hash, txid);

            } else {
                LOG4("transHash", hash);
                getTransaction(trans, hash);
            }
        } else {
            blknum_t blockNum = str_2_Uint(hash);  // so the input is poorly named, sue me
            LOG4("blockNum.txid", blockNum, txid);
            getTransaction(trans, blockNum, txid);
            if (fileExists(getBinaryCacheFilename(CT_TXS, blockNum, txid)))
                fromCache = true;
        }

        if (!fromCache) {
            LOG4("not from cache");
            CBlock block;
            trans.pBlock = &block;
            if (isHash(trans.hash)) {
                // Note: at this point, we are not fully formed, we have to ask the node for the receipt
                getBlock(block, trans.blockNumber);
                if (block.transactions.size() > trans.transactionIndex)
                    trans.isError = block.transactions[trans.transactionIndex].isError;
                trans.receipt.pTrans = &trans;
                getReceipt(trans.receipt, trans.getValueByName("hash"));
                trans.finishParse();
            } else {
                // If the transaction has no hash here, there was a problem. Let the caller know
                trans.hash = orig + " invalid";
            }
        }
        if (!(*func)(trans, data))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool forEveryAbiInArray(ABIVISITFUNC func, void* data, const CAbiArray& abis) {
    if (!func)
        return false;
    for (auto abi : abis) {
        if (!(*func)(abi, data))
            return false;
    }
    return true;
}

//--------------------------------------------------------------------------
inline bool prevLastBlocks(blknum_t& u, blknum_t& r, blknum_t& s, blknum_t& f, blknum_t& c, bool write) {
    string_q fn = configPath("cache/tmp/scraper-status.txt");
    if (write) {
        ostringstream os;
        os << u << "\t" << r << "\t" << s << "\t" << f << "\t" << c << endl;
        stringToAsciiFile(fn, os.str());
    } else {
        string_q contents = asciiFileToString(fn);
        CUintArray parts;
        explode(parts, contents, '\t');
        if (parts.size()) {
            u = parts[0];
            r = parts[1];
            s = parts[2];
            f = parts[3];
            c = parts[4];
        } else {
            u = r = s = f = c = NOPOS;
        }
    }
    return true;
}

//-------------------------------------------------------------------------
inline string_q showLastBlocks(const blknum_t u, const blknum_t r, const blknum_t s, const blknum_t f,
                               const blknum_t c) {
    ostringstream os;
    if (isTestMode()) {
        os << "--final--, --staging--, --ripe--, --unripe--, --client--";
    } else {
        os << cYellow;
        os << padNum9T((int64_t)f) << ", ";
        os << padNum9T((int64_t)s) << ", ";
        os << padNum9T((int64_t)r) << ", ";
        os << padNum9T((int64_t)u) << ", ";
        os << padNum9T((int64_t)c);
        os << cOff;
    }
    return os.str();
}

//-------------------------------------------------------------------------
string_q scraperStatus(bool terse) {
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    char username[LOGIN_NAME_MAX];
    getlogin_r(username, LOGIN_NAME_MAX);
    string_q hostUser = string_q(hostname) + " (" + username + ")";

    uint64_t unripe, ripe, staging, finalized, client;
    uint64_t pUnripe, pRipe, pStaging, pFinalized, pClient;

    getLatestBlocks(unripe, ripe, staging, finalized, client);
    if (fileExists(configPath("cache/tmp/scraper-status.txt"))) {
        prevLastBlocks(pUnripe, pRipe, pStaging, pFinalized, pClient, false);
    } else {
        pUnripe = unripe;
        pRipe = ripe;
        pStaging = staging;
        pFinalized = finalized;
        pClient = client;
    }
    prevLastBlocks(unripe, ripe, staging, finalized, client, true);

    ostringstream heights;
    heights << showLastBlocks(unripe, ripe, staging, finalized, client);

    ostringstream pHeights;
    pHeights << showLastBlocks(pUnripe, pRipe, pStaging, pFinalized, pClient);

    ostringstream distances;
    distances << showLastBlocks(client - unripe, client - ripe, client - staging, client - finalized, client - client);

    ostringstream diffs;
    diffs << showLastBlocks(unripe - pUnripe, ripe - pRipe, staging - pStaging, finalized - pFinalized,
                            client - pClient);

    ostringstream pNeighbors;
    pNeighbors << showLastBlocks(pRipe - pUnripe, pStaging - pRipe, pFinalized - pStaging, 0, 0);

#define showOne(a, b) cYellow << (isTestMode() ? a : b) << cOff
#define showOne1(a) showOne(a, a)

    ostringstream os;
    if (!terse) {
        os << cGreen << "  Client version:     " << showOne("--version--", getVersionFromClient()) << endl;
        os << cGreen << "  Trueblocks version: " << showOne1(getVersionStr()) << endl;
        os << cGreen << "  RPC provider:       " << showOne("--rpc_provider--", getCurlContext()->baseURL) << endl;
        os << cGreen << "  Cache location:     " << showOne("--cache_dir--", getCachePath("")) << endl;
        os << cGreen << "  Host (user):        " << showOne("--host (user)--", hostUser) << endl;
        os << cGreen << "  Heights:            " << heights.str() << endl;
        os << cGreen << "  Previous Heights:   " << pHeights.str() << endl;
        os << cGreen << "  Distances:          " << distances.str() << endl;
        os << cGreen << "  Diffs:              " << diffs.str() << endl;
        os << cGreen << "  Neighbors:          " << pNeighbors.str() << endl;
    } else {
        os << "\t  heights:\t" << heights.str() << endl;
        os << "\t  prev heights:\t" << pHeights.str() << endl;
        os << "\t  distances:\t" << distances.str() << endl;
        os << "\t  diffs:\t" << diffs.str() << endl;
        os << "\t  neighbors:\t" << pNeighbors.str() << endl;
    }
    return os.str();
}

//-------------------------------------------------------------------------
string_q getIndexPath(const string_q& _part) {
    string_q indexPath = getGlobalConfig()->getConfigStr("settings", "indexPath", "<not-set>");
    if (indexPath == "<not-set>" || !folderExists(indexPath))
        return getCachePath("addr_index/" + _part);
    return indexPath + _part;
}

//--------------------------------------------------------------------------
biguint_t weiPerEther(void) {
    return (modexp(10, 9, uint64_t(10000000000)) * modexp(10, 9, uint64_t(10000000000)));
}

//-----------------------------------------------------------------------
string_q headerRow(const string_q& formatIn, const string_q& sep1, const string_q& sep2) {
    string_q format = substitute(substitute(formatIn, "{", "<field>"), "}", "</field>");
    string_q ret;
    while (contains(format, "<field>")) {
        string_q field = toLower(snagFieldClear(format, "field"));
        ret = ret + (sep2 + field + sep2 + sep1);
    }
    return trim(ret, sep1[0]);
}

//-----------------------------------------------------------------------
string_q exportPreamble(const string_q& format, const string_q& className) {
    format_t fmt = expContext().exportFmt;
    if (!className.empty()) {
        CBaseNode* obj = createObjectOfType(className);
        if (obj) {
            const CRuntimeClass* pClass = obj->getRuntimeClass();
            if (!pClass)
                cerr << "Unknown class '" << className << "'. Is it registered?" << endl;
            else if (pClass->fieldList.size() == 0)
                cerr << "Class '" << className << "' has no fields. Is it registered?" << endl;
            else
                expContext().types[className] = pClass;
            delete obj;
        }
    }

    ostringstream os;
    switch (fmt) {
        case NONE1:
            return "";
        case TXT1:
            if (format.empty())
                return "";
            os << headerRow(format, "\t", "");
            break;
        case CSV1:
            if (format.empty())
                return "";
            os << headerRow(format, ",", "\"");
            break;
        case JSON1:
            os << "{ \"data\": [";
            break;
        case API1:
            os << "{\"data\": [";
            break;
        default:
            ASSERT(0);  // shouldn't happen
            break;
    }

    return trim(trim(os.str(), ','), '\t') + "\n";
}

//-----------------------------------------------------------------------
inline string_q dispNumOrHex(uint64_t num) {
    if (num == NOPOS)
        return "\"n/a\"";
    if (!isTestMode())
        return uint_2_Str(num);
    ostringstream os;
    os << "\"0x" << std::hex << num << "\"";
    return os.str();
}

//-----------------------------------------------------------------------
string_q exportPostamble(const CStringArray& errorsIn, const string_q& extra) {
    format_t fmt = expContext().exportFmt;

    const char* STR_ERROR_MSG_TXT = "\"{[MSG]}\"";
    const char* STR_ERROR_MSG_JSON = "\"[MSG]\"";

    bool isText = (fmt == TXT1 || fmt == CSV1 || fmt == NONE1);

    CStringArray errors = errorsIn;
    for (auto curlError : getCurlContext()->curlErrors)
        errors.push_back(substitute(substitute(curlError, "\"", ""), "\n", ""));

    ostringstream errStrs;
    bool first = true;
    for (auto error : errors) {
        string_q msg = (isText ? STR_ERROR_MSG_TXT : STR_ERROR_MSG_JSON);
        if (!first) {
            if (isText)
                errStrs << endl;
            else
                errStrs << ", ";
        }
        errStrs << substitute(substitute(substitute(msg, "[MSG]", error), "{", cRed), "}", cOff);
        first = false;
    }

    if (isText)
        return errStrs.str();  // only errors are reported for text or csv
    ASSERT(fmt == JSON1 || fmt == API1);

    ostringstream os;
    os << "]";  // finish the data array (or the error array)...
    if (!errStrs.str().empty())
        os << ", \"errors\": [\n" << errStrs.str() << "\n]";

    if (!isText && expContext().types.size() > 0) {
        ostringstream typeStrs;
        first = true;
        for (auto type : expContext().types) {
            if (!first)
                typeStrs << ", ";
            typeStrs << "{ \"type\": \"" << type.first << "\", \"fields\": [";
            const CRuntimeClass* pClass = type.second;
            bool fy = true;
            while (pClass) {
                for (auto field : pClass->fieldList) {
                    if (!field.isHidden()) {
                        if (!fy)
                            typeStrs << ", ";
                        string_q t = toLower(fieldTypeName(field.getType()));
                        t = trim(substitute(substitute(substitute(t, "\t", " "), "t_", ""), "  ", " "));
                        typeStrs << "{ \"name\": \"" << field.getName() << "\", \"type\": \"" << t << "\" }";
                        fy = false;
                    }
                }
                string_q parent = pClass->m_BaseClass ? pClass->m_BaseClass->m_ClassName : "";
                pClass = (parent == "CBaseClass" ? NULL : pClass->m_BaseClass);
            }
            typeStrs << "] }";
            first = false;
        }
        os << ", \"types\": [\n" << typeStrs.str() << "\n]";
    }

    if (fmt == JSON1)
        return os.str() + " }";
    ASSERT(fmt == API1);

    uint64_t unripe, ripe, staging, finalized, client;
    getLatestBlocks(unripe, ripe, staging, finalized, client);
    if (isTestMode())
        unripe = ripe = staging = finalized = client = 0xdeadbeef;
    os << ", \"meta\": {";
    os << "\"unripe\": " << dispNumOrHex(unripe) << ",";
    os << "\"ripe\": " << dispNumOrHex(ripe) << ",";
    os << "\"staging\": " << dispNumOrHex(staging) << ",";
    os << "\"finalized\": " << dispNumOrHex(finalized) << ",";
    os << "\"client\": " << dispNumOrHex(client);
    if (!extra.empty())
        os << extra;
    os << " }";
    os << " }";

    return os.str();
}

//--------------------------------------------------------------
bool findTimestamp_binarySearch(CBlock& block, size_t first, size_t last, bool progress) {
    string_q t("|/-\\|/-\\");
    static int i = 0;
    if (progress && !isTestMode()) {
        cerr << "\r" << cGreen << t[(i++ % 8)] << " working" << cOff;
        cerr.flush();
    }

    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        CBlock b1, b2;
        getBlock_light(b1, mid);
        getBlock_light(b2, mid + 1);
        bool atMid = (b1.timestamp <= block.timestamp);
        bool atMid1 = (b2.timestamp <= block.timestamp);
        if (atMid && !atMid1) {
            block = b1;
            return true;
        } else if (!atMid) {
            // we're too high, so search below
            return findTimestamp_binarySearch(block, first, mid - 1);
        }
        // we're too low, so search above
        return findTimestamp_binarySearch(block, mid + 1, last);
    }
    getBlock_light(block, first);
    return true;
}

//-------------------------------------------------------------------------
wei_t blockReward(blknum_t bn, blknum_t txid, bool txFee) {
    if (txFee || txid == 99998)  // TODO(tjayrush): figure out uncle mining reward
        return str_2_Wei("0000000000000000000");

    if (bn < byzantiumBlock) {
        return str_2_Wei("5000000000000000000");
    } else if (bn < constantinopleBlock) {
        return str_2_Wei("3000000000000000000");
    } else {
        return str_2_Wei("2000000000000000000");
    }
}

//----------------------------------------------------------------
bool excludeTrace(const CTransaction* trans, size_t maxTraces) {
    if (!ddosRange(trans->blockNumber))
        return false;  // be careful, it's backwards

    static string_q exclusions;
    if (getGlobalConfig("acctExport")->getConfigBool("exclusions", "enabled", false)) {
        if (exclusions.empty())
            exclusions = getGlobalConfig("acctExport")->getConfigStr("exclusions", "list", "");
        if (contains(exclusions, trans->to))
            return true;
        if (contains(exclusions, trans->from))
            return true;
    }
    return (getTraceCount(trans->hash) > maxTraces);
}

//-----------------------------------------------------------------------
bool freshenTimestamps(blknum_t minBlock) {
    if (isTestMode())
        return true;

    string_q fn = configPath("ts.bin");
    if (!fileExists(fn)) {
        string_q zipFile = configPath("ts.bin.gz");
        if (fileExists(zipFile)) {  // first run since install? Let's try to get some timestamps
            string_q cmd = "cd " + configPath("") + " ; gunzip " + zipFile;
            cerr << doCommand(cmd) << endl;
            ASSERT(!fileExists(zipFile));
            ASSERT(fileExists(fn));
        }
    }

    size_t nRecords = ((fileSize(fn) / sizeof(uint32_t)) / 2);
    if (nRecords >= minBlock)
        return true;

    if (fileExists(fn + ".lck"))  // it's being updated elsewhere
        return true;

    CArchive file(WRITING_ARCHIVE);
    if (!file.Lock(fn, modeWriteAppend, LOCK_NOWAIT)) {
        cerr << "Failed to open ts.bin" << endl;
        return false;
    }

    if (nRecords == 0) {
        file << (uint32_t)0 << (uint32_t)blockZeroTs;
        file.flush();
        cerr << "\t" << padNum9(0) << "\t";
        cerr << blockZeroTs << "\t";
        cerr << ts_2_Date(blockZeroTs).Format(FMT_EXPORT);
        cerr << "          \r";
        cerr.flush();
    }

    CBlock block;
    for (blknum_t bn = nRecords; bn <= minBlock; bn++) {
        block = CBlock();  // reset
        getBlock_header(block, bn);
        file << ((uint32_t)block.blockNumber) << ((uint32_t)block.timestamp);
        file.flush();
        cerr << "\t" << padNum9(block.blockNumber) << "\t";
        cerr << block.timestamp << "\t";
        cerr << ts_2_Date(block.timestamp).Format(FMT_EXPORT);
        cerr << "          \r";
        cerr.flush();
    }
    file.Release();

    cerr << cTeal << "\t  updated timestamps to " << block.blockNumber << " (";
    cerr << block.timestamp << " - ";
    cerr << ts_2_Date(block.timestamp).Format(FMT_EXPORT) << ")" << cOff << endl;
    return true;
}

//-----------------------------------------------------------------------
bool loadTimestampFile(uint32_t** theArray, size_t& cnt) {
    static CMemMapFile file;
    if (file.is_open())
        file.close();

    string_q fn = configPath("ts.bin");
    cnt = ((fileSize(fn) / sizeof(uint32_t)) / 2);
    if (theArray == NULL)
        return false;

    file.open(configPath("ts.bin"));
    if (file.isValid())
        *theArray = (uint32_t*)file.getData();  // NOLINT

    return true;
}

//-------------------------------------------------------------------------
string_q doEthCall(const address_t& to, const string_q& encoding, const string_q& bytes, blknum_t blockNum,
                   const CAbi& abi) {
    ostringstream cmd;
    cmd << "[{";
    cmd << "\"to\": \"" << to << "\", ";
    cmd << "\"data\": \"" << encoding << bytes << "\"";
    cmd << "}, \"" << uint_2_Hex(blockNum) << "\"]";

    CFunction ret;
    string_q rpcRet = callRPC("eth_call", cmd.str(), false);
    if (startsWith(rpcRet, "0x"))
        abi.articulateOutputs(encoding, rpcRet, ret);
    return ret.outputs.size() ? ret.outputs[0].value : "";
}

//-----------------------------------------------------------------------
const string_q defHide =
    "CTransaction: price, nonce, input"
    "|CLogEntry: data, topics"
    "|CTrace: blockHash, blockNumber, transactionHash, transactionIndex, traceAddress, subtraces"
    "|CTraceAction: init"
    "|CTraceResult: code"
    "|CFunction: constant, payable, signature, encoding, type, articulate_str"
    "|CParameter: type, indexed, is_pointer, is_array, is_object, is_builtin, no_write, is_minimal, is_enabled, "
    "is_flags";

const string_q defShow =
    "CTransaction: gasCost, articulatedTx, traces, isError, date, ether"
    "|CLogEntry: articulatedLog"
    "|CTrace: articulatedTrace"
    "|CTraceAction: "
    "|CTraceResult: "
    "|CFunction: "
    "|CParameter: ";

}  // namespace qblocks
