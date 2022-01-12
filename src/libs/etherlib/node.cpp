/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
#include "logfilter.h"

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
    CTraceFilter::registerClass();
    CAbi::registerClass();
    CFunction::registerClass();
    CParameter::registerClass();

    CReconciliation::registerClass();
    CEthState::registerClass();
    CEthCall::registerClass();
    CAppearance::registerClass();
    CRPCResult::registerClass();
    CAccountName::registerClass();
    CCollection::registerClass();
    CCacheEntry::registerClass();

    establishFolder(getPathToRootConfig(""));
    establishFolder(getPathToCache(""));
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
bool getNodeIds(uint64_t& clientId, uint64_t& networkId) {
    clientId = str_2_Uint(callRPC("eth_chainId", "[]", false));
    networkId = str_2_Uint(callRPC("net_version", "[]", false));
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
    if (isParity())
        getObjectViaRPC(block, "parity_getBlockHeaderByNumber", "[" + quote(val) + "]");
    else
        getBlock_light(block, str_2_Uint(val));
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
bool getUncle(CBlock& block, blknum_t blockNum, size_t index) {
    return queryUncle(block, uint_2_Hex(blockNum), index);
}

//-------------------------------------------------------------------------
bool getUncle(CBlock& block, const hash_t& blockHash, size_t index) {
    return queryUncle(block, blockHash, index);
}

//-------------------------------------------------------------------------
bool queryUncle(CBlock& block, const string_q& datIn, size_t index) {
    if (datIn == "latest")
        return queryUncle(block, uint_2_Hex(getBlockProgress(BP_CLIENT).client), index);
    string_q func = isHash(datIn) ? "eth_getUncleByBlockHashAndIndex" : "eth_getUncleByBlockNumberAndIndex";
    string_q params = "[" + quote(datIn) + "," + quote(uint_2_Hex(index)) + "]";
    return getObjectViaRPC(block, func, params);
}

//-------------------------------------------------------------------------
size_t getUncleCount(blknum_t blockNum) {
    return queryUncleCount(uint_2_Hex(blockNum));
}

//-------------------------------------------------------------------------
size_t getUncleCount(const hash_t& blockHash) {
    return queryUncleCount(blockHash);
}

//-------------------------------------------------------------------------
size_t queryUncleCount(const string_q& datIn) {
    if (datIn == "latest")
        return queryUncleCount(uint_2_Hex(getBlockProgress(BP_CLIENT).client));
    string_q func = isHash(datIn) ? "eth_getUncleCountByBlockHash" : "eth_getUncleCountByBlockNumber";
    return str_2_Uint(callRPC(func, "[" + quote(datIn) + "]", false));
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, blknum_t blockNum, txnum_t txid) {
    if (fileExists(getBinaryCacheFilename(CT_TXS, blockNum, txid))) {
        readTransFromBinary(trans, getBinaryCacheFilename(CT_TXS, blockNum, txid));
        trans.pBlock = nullptr;  // otherwise, it's pointing to an unintialized item
        trans.finishParse();     // set the pointer for the receipt
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
            cerr << "mkdir(" << created << ")" << string_q(80, ' ') << "\n";
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
    receipt = CReceipt(receipt.pTransaction);
    getObjectViaRPC(receipt, "eth_getTransactionReceipt", "[\"" + str_2_Hash(txHash) + "\"]");
    return true;
}

//--------------------------------------------------------------
void getTraces(CTraceArray& traces, const hash_t& hash) {
    string_q str;
    queryRawTrace(str, hash);

    CRPCResult generic;
    generic.parseJson3(str);  // pull out the result

    generic.result = cleanUpJson((char*)generic.result.c_str());  // NOLINT
    CTrace trace;
    traces.clear();
    while (trace.parseJson4(generic.result)) {
        traces.push_back(trace);
        trace = CTrace();  // reset
    }
}

//--------------------------------------------------------------
void getStateDiffAddrs(CAddressArray& addrs, const hash_t& hash) {
    string_q str;
    queryRawStateDiff(str, hash);
    cout << str << endl;
    // CRPCResult generic;
    // generic.parseJson3(str);  // pull out the result
    // generic.result = cleanUpJson((char*)generic.result.c_str());  // NOLINT
    // CTrace trace;
    // diffs.clear();
    // while (trace.parseJson4(generic.result)) {
    //     diffs.push_back(trace);
    //     trace = CTrace();  // reset
    // }
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
            dDos.loadTraceAsDdos(trans, bn, txid);
            trans.traces.push_back(dDos);

        } else if (txid == 99997 || txid == 99999) {
            // 99997 was a misconfigured miners early in the chain due to a bug.
            // 99999 is record for the winning miner
            CTrace rewardTrace;
            rewardTrace.loadTraceAsBlockReward(trans, bn, txid);
            trans.traces.push_back(rewardTrace);

            CTrace txFee;
            txFee.loadTraceAsTransFee(trans, bn, txid);
            trans.traces.push_back(txFee);

        } else if (txid == 99998) {
            // 99998 is an uncle reward
            uint64_t nUncles = getUncleCount(bn);
            for (size_t i = 0; i < nUncles; i++) {
                CBlock uncle;
                CTrace rewardTrace;
                getUncle(uncle, bn, i);
                rewardTrace.loadTraceAsUncleReward(trans, bn, uncle.blockNumber);
                trans.traces.push_back(rewardTrace);
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
    if (trans->blockNumber >= byzantiumBlock || isErigon()) {
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
        return queryBlock(block, uint_2_Str(getBlockProgress(BP_CLIENT).client), needTrace);

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
bool queryRawUncle(string_q& results, const string_q& blockNum, uint64_t index) {
    if (blockNum == "latest")
        return queryRawUncle(results, uint_2_Str(getBlockProgress(BP_CLIENT).client), index);
    string_q func = isHash(blockNum) ? "eth_getUncleByBlockHashAndIndex" : "eth_getUncleByBlockNumberAndIndex";
    uint64_t bn = str_2_Uint(blockNum);
    string_q params = "[" + quote(uint_2_Hex(bn)) + "," + quote(uint_2_Hex(index)) + "]";
    results = callRPC(func, params, true);
    return true;
}

//-------------------------------------------------------------------------
bool queryRawBlockTrace(string_q& blockStr, const string_q& hexNum) {
    string_q data = "[\"[BLOCKNUM]\"]";
    replace(data, "[BLOCKNUM]", hexNum);
    blockStr = callRPC("trace_block", data, true);
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
string_q getRawUncle(blknum_t bn, uint64_t index) {
    string_q numStr = uint_2_Str(bn);

    string_q str;
    queryRawUncle(str, numStr, index);

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
bool queryRawStateDiff(string_q& diffs, const string_q& hashIn) {
    diffs = "[" + callRPC("trace_replayTransaction", "[\"" + str_2_Hash(hashIn) + "\",[\"stateDiff\"]]", true) + "]";
    return true;
}

//-------------------------------------------------------------------------
bool queryRawLogs(string_q& results, const CLogFilter& query) {
    results = callRPC("eth_getLogs", query.toRPC(), true);
    return true;
}

//-------------------------------------------------------------------------
string_q getTokenBalanceOf(const address_t& token, const address_t& holder, blknum_t blockNum) {
    ostringstream cmd;
    cmd << "[{";
    cmd << "\"to\": \"" << token << "\", ";
    cmd << "\"data\": \"0x70a08231" << padLeft(substitute(holder, "0x", ""), 64, '0') << "\"";
    cmd << "}, \"" << uint_2_Hex(blockNum) << "\"]";
    string_q ret = callRPC("eth_call", cmd.str(), false).substr(0, 66);  // take only the first 32 bytes
    if (startsWith(ret, "0x"))
        return bnu_2_Str(str_2_BigUint(ret, 256));
    return "0";
}

//-------------------------------------------------------------------------
string_q getTokenSymbol(const address_t& token, blknum_t blockNum) {
    ostringstream cmd;
    cmd << "[{";
    cmd << "\"to\": \"" << token << "\", ";
    cmd << "\"data\": \"0x95d89b41\"";
    cmd << "}, \"" << uint_2_Hex(blockNum) << "\"]";
    string_q ret = callRPC("eth_call", cmd.str(), false);
    if (!contains(ret, "error") && !startsWith(ret, "0x"))
        return ret;
    return "";
}

//-------------------------------------------------------------------------
string_q getTokenState(const address_t& token, const string_q& what, const CAbi& abi_spec, blknum_t blockNum,
                       const string_q& bytes) {
    static map<string_q, string_q> sigMap;
    if (sigMap.size() == 0) {
        sigMap["totalSupply"] = "0x18160ddd";
        sigMap["decimals"] = "0x313ce567";
        sigMap["symbol"] = "0x95d89b41";
        sigMap["name"] = "0x06fdde03";
        sigMap["supportsInterface"] = "0x01ffc9a7";
    }

    if (sigMap[what].empty())
        return "";

    CEthCall theCall;
    theCall.address = token;
    theCall.encoding = sigMap[what];
    theCall.bytes = bytes;
    theCall.blockNumber = blockNum;
    theCall.abi_spec = abi_spec;
    if (doEthCall(theCall))
        return theCall.getCallResult();
    return "";
}

//-----------------------------------------------------------------------
void getTracesByFilter(CTraceArray& traces, const CTraceFilter& filter) {
    if (filter.Format() == CTraceFilter().Format())
        return;

    string_q toAddrs;
    for (auto addr : filter.toAddress) {
        if (!isZeroAddr(addr))
            toAddrs += ("\"" + addr + "\",");
    }
    if (!toAddrs.empty()) {
        toAddrs = "[" + trim(toAddrs, ',') + "]";
    }

    string_q fromAddrs;
    for (auto addr : filter.fromAddress) {
        if (!isZeroAddr(addr))
            fromAddrs += ("\"" + addr + "\",");
    }
    if (!fromAddrs.empty()) {
        fromAddrs = "[" + trim(fromAddrs, ',') + "]";
    }

#define AA(test, name, val) ((test) ? "\"" + string_q(name) + "\": " + (val) + "," : "")
    string_q params;
    params += AA(filter.fromBlock, "fromBlock", "\"" + uint_2_Hex(filter.fromBlock) + "\"");
    params += AA(filter.toBlock, "toBlock", "\"" + uint_2_Hex(filter.toBlock) + "\"");
    params += AA(!fromAddrs.empty(), "fromAddress", fromAddrs);
    params += AA(!toAddrs.empty(), "toAddress", toAddrs);
    params += AA(filter.after, "after", uint_2_Str(filter.after));
    params += AA(filter.count, "count", uint_2_Str(filter.count));
    params = "{" + trim(params, ',') + "}";

    cerr << substitute(params, " ", "") << endl;

    string_q result = "[" + callRPC("trace_filter", "[" + params + "]", true) + "]";
    CRPCResult generic;
    generic.parseJson3(result);                                   // pull out the result
    generic.result = cleanUpJson((char*)generic.result.c_str());  // NOLINT
    CTrace trace;
    traces.clear();
    while (trace.parseJson4(generic.result)) {
        traces.push_back(trace);
        trace = CTrace();  // reset
    }
}

//-------------------------------------------------------------------------
string_q getVersionFromClient(void) {
    string_q clientVersionFn = getPathToCache("tmp/clientVersion.txt");
    string_q contents;
    if (fileExists(clientVersionFn))
        contents = asciiFileToString(clientVersionFn);

    timestamp_t lastUpdate = date_2_Ts(fileLastModifyDate(clientVersionFn));
    timestamp_t now = date_2_Ts(Now());
    timestamp_t diff = now - lastUpdate;
    if (diff > 20 || !contains(contents, getCurlContext()->baseURL)) {
        // We do this to avoid constantly hitting the node just to see if it's there.
        // If the rpcProvider changed or we haven't checked in 20 seconds, check again.
        string_q clientVersion = callRPC("web3_clientVersion", "[]", false);
        if (!clientVersion.empty()) {
            if (folderExists(getPathToCache("tmp/")))
                stringToAsciiFile(clientVersionFn, getCurlContext()->baseURL + "\t" + clientVersion);
            return clientVersion;
        }
    }

    CStringArray parts;
    explode(parts, contents, '\t');
    if (parts.size() < 2)
        return uint_2_Str(NOPOS);
    return parts[1];
}

//-------------------------------------------------------------------------
bool isErigon(void) {
    return contains(toLower(getVersionFromClient()), "erigon");
}

//-------------------------------------------------------------------------
bool isGeth(void) {
    return contains(toLower(getVersionFromClient()), "geth") && !isErigon();
}

//-------------------------------------------------------------------------
bool isParity(void) {
    return contains(toLower(getVersionFromClient()), "parity") ||
           contains(toLower(getVersionFromClient()), "openethereum");
}

//-------------------------------------------------------------------------
bool hasParityTraces(void) {
    return isErigon() || isParity();
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
bool hasTraceAt(const string_q& hashIn, size_t where) {
    string_q cmd = "[\"" + str_2_Hash(hashIn) + "\",[\"" + uint_2_Hex(where) + "\"]]";
    string_q ret = callRPC("trace_get", cmd.c_str(), true);
    return ret.find("blockNumber") != string::npos;
}

//-------------------------------------------------------------------------
bool isTracingNode(void) {
    // short curcuit for some situations
    const CToml* config = getGlobalConfig("blockScrape");
    if (!config->getConfigBool("requires", "tracing", true))
        return true;

    // At block 50871 (firstTraceBlock) transaction 0, (hash:
    // 0x6df0b4a0d15ae3b925b9819646a0cff4d1bc0a53b294c0d84d884865302d13a5) we know there were exactly 23 traces as per
    // Parity. We check that here to see if the node is running with --tracing enabled. Not sure how this works with
    // Geth
    static int answered = int(-1);  // NOLINT
    if (answered != int(-1))        // NOLINT
        return answered;
    bool at23 = hasTraceAt("0x6df0b4a0d15ae3b925b9819646a0cff4d1bc0a53b294c0d84d884865302d13a5", 23);
    bool at24 = hasTraceAt("0x6df0b4a0d15ae3b925b9819646a0cff4d1bc0a53b294c0d84d884865302d13a5", 24);
    answered = (at23 && !at24);
    return answered;
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

//-------------------------------------------------------------------------
static string_q getFilename_local(cache_t type, const string_q& item1, const string_q& item2, const string_q& item3,
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
        case CT_MONITORS:
            os << "monitors/";
            break;
        case CT_RECONS:
            os << "recons/";
            break;
        default:
            ASSERT(0);  // should not happen
    }

    if (type == CT_MONITORS) {
        string_q addr = toLower(substitute(item1, "0x", ""));
        os << extract(addr, 0, 4) << "/" << extract(addr, 4, 4) << "/" << addr << ".bin";

    } else if (type == CT_RECONS) {
        string_q addr = toLower(substitute(item1, "0x", ""));
        string_q part1 = extract(addr, 0, 4);
        string_q part2 = extract(addr, 4, 4);
        string_q part3 = addr;
        replace(part3, part1, "");  // do not collapse
        replace(part3, part2, "");  // do not collapse
        os << part1 << "/" << part2 << "/" << part3 << "/";
        if (item2 != padNum9((uint64_t)NOPOS))
            os << item2 << "." << item3 << ".bin";

    } else {
        os << extract(item1, 0, 2) << "/" << extract(item1, 2, 2) << "/" << extract(item1, 4, 2) << "/";
        if (!asPath) {
            os << item1;
            os << ((type == CT_TRACES || type == CT_TXS) ? "-" + item2 : "");
            os << (type == CT_TRACES && !item3.empty() ? "-" + item3 : "");
            os << ".bin";
        }
    }
    return getPathToCache(os.str());
}

//-------------------------------------------------------------------------
string_q getPathToBinaryCache(cache_t type, blknum_t bn, txnum_t txid, const string_q& trc_id) {
    return getFilename_local(type, padNum9(bn), padNum5(txid), trc_id, true);
}

//-------------------------------------------------------------------------
string_q getBinaryCacheFilename(cache_t type, blknum_t bn, txnum_t txid, const string_q& trc_id) {
    return getFilename_local(type, padNum9(bn), padNum5(txid), trc_id, false);
}

//-------------------------------------------------------------------------
string_q getPathToBinaryCache(cache_t type, const address_t& addr, blknum_t bn, txnum_t txid) {
    return getFilename_local(type, addr, padNum9(bn), padNum5(txid), true);
}

//-------------------------------------------------------------------------
string_q getBinaryCacheFilename(cache_t type, const address_t& addr, blknum_t bn, txnum_t txid) {
    return getFilename_local(type, addr, padNum9(bn), padNum5(txid), false);
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
bool forEveryBlock_light(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip) {
    // Here we simply scan the numbers and either read from disc or query the node
    if (!func)
        return false;

    for (uint64_t i = start; i < start + count - 1; i = i + skip) {
        CBlock block;
        getBlock_light(block, i);
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
bool forEveryTransaction(TRANSVISITFUNC func, void* data, const string_q& trans_list) {
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
                LOG4("blockHash.txid", " ", hash, " ", txid);
                getTransaction(trans, hash, txid);

            } else {
                LOG4("transHash", hash);
                getTransaction(trans, hash);
            }
        } else {
            blknum_t blockNum = str_2_Uint(hash);  // so the input is poorly named, sue me
            LOG4("blockNum.txid", " ", blockNum, " ", txid);
            getTransaction(trans, blockNum, txid);
            if (fileExists(getBinaryCacheFilename(CT_TXS, blockNum, txid)))
                fromCache = true;
        }

        if (!fromCache) {
            LOG4("not from cache");
            CBlock block;
            trans.pBlock = &block;
            if (isHash(trans.hash)) {
                // Note: at this point, we are not fully formed, we need the receipt and the timestamp
                getBlock_light(block, trans.blockNumber);
                getFullReceipt(&trans, true);
                trans.timestamp = block.timestamp;
                trans.receipt.pTransaction = &trans;
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

//--------------------------------------------------------------------------
biguint_t weiPerEther(void) {
    return (modexp(10, 9, uint64_t(10000000000)) * modexp(10, 9, uint64_t(10000000000)));
}

//-----------------------------------------------------------------------
string_q headerRow(const string_q& className, const string_q& formatIn, const string_q& sep1, const string_q& sep2) {
    string_q format = substitute(substitute(formatIn, "{", "<field>"), "}", "</field>");
    string_q ret;
    while (contains(format, "<field>")) {
        string_q field = toLower(snagFieldClear(format, "field"));
        ret = ret + (sep2 + field + sep2 + sep1);
    }
    // TODO: This is a pretty bad hack that will get corrected once we port entirely to GoLang
    extern string_q renameExportFields(const string_q& className, const string_q& inStr);
    ret = renameExportFields(className, ret);

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
            os << headerRow(className, format, "\t", "");
            break;
        case CSV1:
            if (format.empty())
                return "";
            os << headerRow(className, format, ",", "\"");
            break;
        case YAML1:
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
    if (fmt == YAML1)
        return "";

    const char* STR_ERROR_MSG_TXT = "\"{[MSG]}\"";
    const char* STR_ERROR_MSG_JSON = "\"[MSG]\"";

    bool isText = (fmt == TXT1 || fmt == CSV1 || fmt == NONE1);

    CStringArray errors = errorsIn;
    for (auto curlError : getCurlContext()->curlErrors)
        errors.push_back(substitute(substitute(curlError, "\"", ""), "\n", ""));

    ostringstream errStream;
    bool first = true;
    for (auto error : errors) {
        string_q msg = (isText ? STR_ERROR_MSG_TXT : STR_ERROR_MSG_JSON);
        if (!first) {
            if (isText)
                errStream << endl;
            else
                errStream << ", ";
        }
        errStream << substitute(substitute(substitute(msg, "[MSG]", error), "{", cRed), "}", cOff);
        first = false;
    }

    if (isText)
        return errStream.str();  // only errors are reported for text or csv
    ASSERT(fmt == JSON1 || fmt == API1);

    ostringstream os;
    os << "]";  // finish the data array (or the error array)...

    if (!errStream.str().empty())
        os << ", \"errors\": [\n" << errStream.str() << "\n]";

    if (fmt == JSON1)
        return os.str() + " }";
    ASSERT(fmt == API1);

    CMetaData progress = getMetaData();
    blknum_t unripe = progress.unripe;
    blknum_t ripe = progress.ripe;
    blknum_t staging = progress.staging;
    blknum_t finalized = progress.finalized;
    blknum_t client = progress.client;
    if (isTestMode())
        unripe = ripe = staging = finalized = client = 0xdeadbeef;
    os << ", \"meta\": {";
    os << "\"unripe\": " << dispNumOrHex(unripe) << ",";
    os << "\"ripe\": " << dispNumOrHex(ripe) << ",";
    os << "\"staging\": " << dispNumOrHex(staging) << ",";
    os << "\"finalized\": " << dispNumOrHex(finalized);
    os << ",\"client\": " << dispNumOrHex(client);
    if (!extra.empty())
        os << extra;
    os << " }";
    os << " }";

    return os.str();
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
// TODO(tjayrush): These two things are appalling
//-----------------------------------------------------------------------
const string_q defHide =
    "CTransaction: price, nonce, input"
    "|CLogEntry: data, topics"
    "|CTrace: "
    "|CTraceAction: init"
    "|CTraceResult: code"
    "|CFunction: constant, payable, signature, encoding, type, articulate_str"
    "|CParameter: type, indexed, is_pointer, is_array, is_object, is_builtin, "
    "is_enabled, is_minimal, is_noaddfld, is_nowrite, is_omitempty, is_extra, is_flags";

const string_q defShow =
    "CTransaction: gasCost, articulatedTx, compressedTx, "
    "traces, isError, hasToken, date, ether"
    "|CLogEntry: articulatedLog, compressedLog"
    "|CTrace: articulatedTrace, compressedTrace"
    "|CTraceAction: "
    "|CTraceResult: "
    "|CFunction: "
    "|CParameter: ";

}  // namespace qblocks
