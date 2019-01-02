/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

namespace qblocks {

    static QUITHANDLER theQuitHandler = NULL;
    //-------------------------------------------------------------------------
    void etherlib_init(const string_q& sourceIn, QUITHANDLER qh) {

        string_q fallBack = getEnvStr("FALLBACK");
        if (getCurlContext()->nodeRequired && !isNodeRunning() && fallBack.empty()) {
            cerr << endl;
            cerr << "\t" << cTeal << "Warning: " << cOff << "This program requires a locally running node. Please start your " << endl;
            cerr << "\tnode or export FALLBACK=infura in your environment before running this " << endl;
            cerr << "\tcommand. Quitting..." << endl;
            cerr << endl;
            quickQuitHandler(EXIT_FAILURE);
        }

        establishFolder(blockCachePath(""));

        // In case we create any lock files, so
        // they get cleaned up
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
        CRPCResult::registerClass();
        CAccountName::registerClass();

        if (sourceIn != "remote" && sourceIn != "local" && sourceIn != "ropsten")
            getCurlContext()->provider = "binary";
        else
            getCurlContext()->provider = sourceIn;

        // if curl has already been initialized, we want to clear it out
        getCurl(true);
        // initialize curl
        getCurl();

        establishFolder(configPath(""));
    }

    //-------------------------------------------------------------------------
    void etherlib_cleanup(void) {
#ifdef PROVING
        if (expContext().proving) {
            string_q proof = expContext().proof.str();
            cout << bGreen << "proof: " << cOff << proof << "\n";
            if (verbose)
                cout << bGreen << "hex: " << cOff  << chr_2_HexStr(proof) << "\n";
            cout << bGreen << "hash: " << cOff  << getSha3(chr_2_HexStr(proof)) << "\n";
            cout.flush();
        }
#endif
        getCurl(true);
        clearInMemoryCache();
        if (theQuitHandler)
            (*theQuitHandler)(-1);
        else
            cleanFileLocks();
    }

    //-------------------------------------------------------------------------
    bool getBlock(CBlock& block, blknum_t blockNum) {
        getCurlContext()->provider = fileExists(getBinaryFilename(blockNum)) ? "binary" : "local";
        bool ret = queryBlock(block, uint_2_Str(blockNum), true, false);
        getCurlContext()->provider = "binary";
        return ret;
    }

    //-------------------------------------------------------------------------
    bool getBlock(CBlock& block, const hash_t& blockHash) {
        return queryBlock(block, blockHash, true, true);
    }

    //-------------------------------------------------------------------------
    bool getTransaction(CTransaction& trans, const hash_t& txHash) {
        getObjectViaRPC(trans, "eth_getTransactionByHash", "[\"" + str_2_Hash(txHash) +"\"]");
        trans.finishParse();
        return true;
    }

    //-------------------------------------------------------------------------
    bool getTransaction(CTransaction& trans, const hash_t& blockHash, txnum_t txID) {
        getObjectViaRPC(trans, "eth_getTransactionByBlockHashAndIndex",
                                    "[\"" + str_2_Hash(blockHash) +"\",\"" + uint_2_Hex(txID) + "\"]");
        trans.finishParse();
        return true;
    }

    //-------------------------------------------------------------------------
    bool getTransaction(CTransaction& trans, blknum_t blockNum, txnum_t txID) {

        if (fileExists(getBinaryFilename(blockNum))) {
            CBlock block;
            readBlockFromBinary(block, getBinaryFilename(blockNum));
            if (txID < block.transactions.size()) {
                trans = block.transactions[txID];
                trans.pBlock = NULL;  // otherwise, it's pointing to a dead pointer
                return true;
            }
            // fall through to node
        }

        getObjectViaRPC(trans, "eth_getTransactionByBlockNumberAndIndex",
                                    "[\"" + uint_2_Hex(blockNum) +"\",\"" + uint_2_Hex(txID) + "\"]");
        trans.finishParse();
        return true;
    }

    //-------------------------------------------------------------------------
    bool getReceipt(CReceipt& receipt, const hash_t& txHash) {
        receipt = CReceipt();
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

        CTrace trace;
        while (trace.parseJson3(generic.result)) {
            traces.push_back(trace);
            trace = CTrace();  // reset
        }
    }

    //-------------------------------------------------------------------------
    bool queryBlock(CBlock& block, const string_q& datIn, bool needTrace, bool byHash) {
        size_t unused = 0;
        return queryBlock(block, datIn, needTrace, byHash, unused);
    }

    //-------------------------------------------------------------------------
    bool queryBlock(CBlock& block, const string_q& datIn, bool needTrace, bool byHash, size_t& nTraces) {

        if (datIn == "latest")
            return queryBlock(block, uint_2_Str(getLatestBlockFromClient()), needTrace, false);

        if (isHash(datIn)) {
            HIDE_FIELD(CTransaction, "receipt");
            getObjectViaRPC(block, "eth_getBlockByHash", "["+quote(datIn)+",true]");

        } else {
            uint64_t num = str_2_Uint(datIn);
            if (getCurlContext()->provider == "binary" && fileSize(getBinaryFilename(num)) > 0) {
                UNHIDE_FIELD(CTransaction, "receipt");
                block = CBlock();
                return readBlockFromBinary(block, getBinaryFilename(num));

            }

            HIDE_FIELD(CTransaction, "receipt");
            getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(uint_2_Hex(num))+",true]");
        }

        // If there are no transactions, we do not have to trace and we want to tell the caller that
        if (!block.transactions.size())
            return false;

        // We have the transactions, but we also want the receipts, and we need an error indication
        nTraces = 0;
        for (size_t i = 0 ; i < block.transactions.size() ; i++) {
            CTransaction *trans = &block.transactions.at(i);  // taking a non-const reference
            trans->pBlock = &block;

            UNHIDE_FIELD(CTransaction, "receipt");
            CReceipt receipt;
            getReceipt(receipt, trans->hash);
            trans->receipt = receipt;  // deep copy
            if (block.blockNumber >= byzantiumBlock) {
                trans->isError = (receipt.status == 0);

            } else if (needTrace && trans->gas == receipt.gasUsed) {

                string_q unused;
                CURLCALLBACKFUNC prev = getCurlContext()->setCurlCallback(traceCallback);
                getCurlContext()->is_error = false;
                queryRawTrace(unused, trans->hash);
                trans->isError = getCurlContext()->is_error;
                getCurlContext()->setCurlCallback(prev);
                nTraces++;
            }
        }

        return true;
    }

    //-------------------------------------------------------------------------
    bool queryRawBlock(string_q& blockStr, const string_q& datIn, bool needTrace, bool hashesOnly) {

        if (isHash(datIn)) {
            blockStr = callRPC("eth_getBlockByHash",
                                "[" + quote(datIn) + "," + (hashesOnly ? "false" : "true") + "]", true);
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
        trace = "[" + callRPC("trace_transaction", "[\"" + str_2_Hash(hashIn) +"\"]", true) + "]";
        return true;
    }

    //-------------------------------------------------------------------------
    bool queryRawLogs(string_q& results, const address_t& addr, uint64_t fromBlock, uint64_t toBlock) {
        string_q data = "[{\"fromBlock\":\"[START]\",\"toBlock\":\"[STOP]\", \"address\": \"[ADDR]\"}]";
        replace(data, "[START]", uint_2_Hex(fromBlock));
        replace(data, "[STOP]",  uint_2_Hex(toBlock));
        replace(data, "[ADDR]",  addr_2_Str(addr));
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

    //-------------------------------------------------------------------------
    uint64_t getLatestBlockFromClient(void) {
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
    uint64_t getLatestBlockFromCache(void) {

        CArchive fullBlockCache(READING_ARCHIVE);
        if (!fullBlockCache.Lock(fullBlockIndex, binaryReadOnly, LOCK_NOWAIT)) {
            if (!isTestMode())
                cerr << "getLatestBlockFromCache failed: " << fullBlockCache.LockFailure() << "\n";
            return 0;
        }
        ASSERT(fullBlockCache.isOpen());

        uint64_t ret = 0;
        fullBlockCache.Seek( (-1 * (long)sizeof(uint64_t)), SEEK_END);  // NOLINT
        fullBlockCache.Read(ret);
        fullBlockCache.Release();
        return ret;
    }

    //--------------------------------------------------------------------------
    bool getLatestBlocks(uint64_t& cache, uint64_t& client) {
        client = getLatestBlockFromClient();
        cache  = getLatestBlockFromCache();
        return true;
    }

    //-------------------------------------------------------------------------
    wei_t getBalanceAt(const string_q& addr, blknum_t num) {
        if (num == NOPOS)
            num = getLatestBlockFromClient();
        string_q params = "[\"[{ADDR}]\",\"[{NUM}]\"]";
        replace(params, "[{ADDR}]", str_2_Addr(addr));
        replace(params, "[{NUM}]",  uint_2_Hex(num));
        return str_2_Wei(callRPC("eth_getBalance", params, false));
    }

    //-------------------------------------------------------------------------
    bool nodeHasBalances(void) {
        // Account 0xa1e4380a3b1f749673e270229993ee55f35663b4 owned 2000000000000000000000 (2000 ether)
        // at block zero. If the node is holding balances (i.e. its an archive node), then it will
        // return that value for block 1 as well. Otherwise, it will return a zero balance.
        // NOTE: Unimportantly, account 0xa1e4380a3b1f749673e270229993ee55f35663b4 transacted in the first ever transaction.
        return getBalanceAt("0xa1e4380a3b1f749673e270229993ee55f35663b4", 1) == str_2_Wei("2000000000000000000000");
    }

    //-------------------------------------------------------------------------
    string_q getCodeAt(const string_q& addr, blknum_t num) {
        if (num == NOPOS)
            num = getLatestBlockFromClient();
        string_q params = "[\"[{ADDR}]\",\"[{NUM}]\"]";
        replace(params, "[{ADDR}]", str_2_Addr(addr));
        replace(params, "[{NUM}]",  uint_2_Hex(num));
        return callRPC("eth_getCode", params, false);
    }

    //-------------------------------------------------------------------------
    bool isContractAt(const address_t& addr, blknum_t num) {
        return !substitute(getCodeAt(addr, num), "0x", "").empty();
    }

    //-------------------------------------------------------------------------
    uint64_t getNonceAt(const address_t& addr, blknum_t num) {
        if (num == NOPOS)
            num = getLatestBlockFromClient();
        string_q params = "[\"[{ADDR}]\",\"[{NUM}]\"]";
        replace(params, "[{ADDR}]", str_2_Addr(addr));
        replace(params, "[{NUM}]",  uint_2_Hex(num));
        return str_2_Uint(callRPC("eth_getTransactionCount", params, false));
    }

    //-------------------------------------------------------------------------
    string_q getStorageAt(const string_q& addr, uint64_t pos, blknum_t num) {
        if (num == NOPOS)
            num = getLatestBlockFromClient();
        string_q params = "[\"[{ADDR}]\",\"[{POS}]\",\"[{NUM}]\"]";
        replace(params, "[{ADDR}]", str_2_Addr(addr));
        replace(params, "[{POS}]",  uint_2_Hex(pos));
        replace(params, "[{NUM}]",  uint_2_Hex(num));
        return callRPC("eth_getStorageAt", params, false);
        // Calculating the correct position depends on the storage to retrieve. Consider the following
        // contract deployed at 0x295a70b2de5e3953354a6a8344e616ed314d7251 by address
        // 0x391694e7e0b0cce554cb130d723a9d27458f9298.
        //
        // contract Storage {
        //     uint pos0;
        //     mapping(address => uint) pos1;
        //
        //     function Storage() {
        //         pos0 = 1234;
        //         pos1[msg.sender] = 5678;
        //     }
        // }
        //
        // Retrieving the value of pos0 is straight forward:
        //
        // curl -X POST --data '{"jsonrpc":"2.0", "method": "eth_get StorageAt", "params": ["0x295a70b2de5e3953354a6a8344e616ed314d7251", "0x0", "latest"],
        //      "id": 1}' localhost:8545
        //
        // returns {"jsonrpc":"2.0","id":1,"result":"0x00000000000000000000000000000000000000000000000000000000000004d2"}
        //
        // Retrieving an element of the map is harder. The position of an element in the map is calculated with:
        //
        //      keccack(LeftPad32(key, 0), LeftPad32(map_position, 0))
        //
        // This means to retrieve the storage on pos1["0x391694e7e0b0cce554cb130d723a9d27458f9298"] we need to calculate
        // the position with:
        //
        //      keccak(decodeHex("000000000000000000000000391694e7e0b0cce554cb130d723a9d27458f9298" + 
        //                       "0000000000000000000000000000000000000000000000000000000000000001"))
        //
        // The geth console which comes with the web3 library can be used to make the calculation:
        //
        //      > var key = "000000000000000000000000391694e7e0b0cce554cb130d723a9d27458f9298" + "0000000000000000000000000000000000000000000000000000000000000001"
        //      undefined
        //      > web3.sha3(key, {"encoding": "hex"})
        //      "0x6661e9d6d8b923d5bbaab1b96e1dd51ff6ea2a93520fdc9eb75d059238b8c5e9"
        //
        // Now to fetch the storage:
        //
        //      curl -X POST --data '{"jsonrpc":"2.0", "method": "eth_get StorageAt", "params": ["0x295a70b2de5e3953354a6a8344e616ed314d7251",
        //                  "0x6661e9d6d8b923d5bbaab1b96e1dd51ff6ea2a93520fdc9eb75d059238b8c5e9", "latest"], "id": 1}' localhost:8545
        //
        // returns: {"jsonrpc":"2.0","id":1,"result":"0x000000000000000000000000000000000000000000000000000000000000162e"}
        //
    }

    //-------------------------------------------------------------------------
    uint64_t addFilter(address_t addr, const CTopicArray& topics, blknum_t num) {
        // Creates a filter object, based on filter options, to notify when the state changes (logs). To check if the state has
        // changed, call eth_getFilterChanges.
        //
        // A note on specifying topic filters:
        // Topics are order-dependent. A transaction with a log with topics [A, B] will be matched by the following topic filters:
        //
        // [] "anything"
        // [A] "A in first position (and anything after)"
        // [null, B] "anything in first position AND B in second position (and anything after)"
        // [A, B] "A in first position AND B in second position (and anything after)"
        // [[A, B], [A, B]] "(A OR B) in first position AND (A OR B) in second position (and anything after)"
        //
        // Parameters
        //  Object - The filter options:
        //  fromBlock:  QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined block or "pending",
        //                  "earliest" for not yet mined transactions.
        //  toBlock:    QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined block or "pending",
        //                  "earliest" for not yet mined transactions.
        //  address:    DATA|Array, 20 Bytes - (optional) Contract address or a list of addresses from which logs should originate.
        //  topics:     Array of DATA, - (optional) Array of 32 Bytes DATA topics. Topics are order-dependent. Each topic can also be an
        //                  array of DATA with "or" options.
        //  params: [{
        //      "fromBlock": "0x1",
        //      "toBlock": "0x2",
        //      "address": "0x8888f1f195afa192cfee860698584c030f4c9db1",
        //      "topics": ["0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b",
        //                  null,
        //                  [ "0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b", "0x0000000000000000000000000aff3454fce5edbc8cca8697c15331677e6ebccc"]]
        //      }]
        //  Returns QUANTITY - A filter id.
        return 0;
    }

    //-------------------------------------------------------------------------
    bool nodeHasTraces(void) {
        // At block 50871 transaction 0, (hash: 0x6df0b4a0d15ae3b925b9819646a0cff4d1bc0a53b294c0d84d884865302d13a5)
        // we know there were exactly 23 traces as per Parity. We check that here to see if the node is
        // running with --tracing enabled. Not sure how this works with Geth
        size_t count = getTraceCount("0x6df0b4a0d15ae3b925b9819646a0cff4d1bc0a53b294c0d84d884865302d13a5");
        return (count == 23);
    }

    //-------------------------------------------------------------------------
    bool hasTraceAt(const string_q& hashIn, size_t where) {
        string_q cmd = "[\"" + str_2_Hash(hashIn) +"\",[\"" + uint_2_Hex(where) + "\"]]";
        string_q ret = callRPC("trace_get", cmd.c_str(), true);
        return ret.find("blockNumber") != string::npos;
    }

    //--------------------------------------------------------------
    size_t getTraceCount_binarySearch(const hash_t& hashIn, size_t first, size_t last) {
        if (last > first) {
            size_t mid = first + ((last - first) / 2);
            bool atMid  = hasTraceAt(hashIn, mid);
            bool atMid1 = hasTraceAt(hashIn, mid + 1);
            if (atMid && !atMid1)
                return mid;  // found it
            if (!atMid) {
                // we're too high, so search below
                return getTraceCount_binarySearch(hashIn, first, mid-1);
            }
            // we're too low, so search above
            return getTraceCount_binarySearch(hashIn, mid+1, last);
        }
        return first;
    }

    // https://ethereum.stackexchange.com/questions/9883/why-is-my-node-synchronization-stuck
    // -extremely-slow-at-block-2-306-843/10453
    //--------------------------------------------------------------
    size_t getTraceCount(const hash_t& hashIn) {
        // handle most likely cases linearly
        for (size_t n = 2 ; n < 8 ; n++) {
            if (!hasTraceAt(hashIn, n)) {
                if (verbose > 2) cerr << "tiny trace" << (n - 1) << "\n";
                return n-1;
            }
        }

        // binary search the rest
        size_t ret = 0;
        if (!hasTraceAt(hashIn, (1 << 8))) {  // small?
            ret = getTraceCount_binarySearch(hashIn, 0, (1 << 8) - 1);
            if (verbose > 2) cerr << "small trace" << ret << "\n";
            return ret;
        } else if (!hasTraceAt(hashIn, (1 << 16))) {  // medium?
            ret = getTraceCount_binarySearch(hashIn, 0, (1 << 16) - 1);
            if (verbose > 2) cerr << "medium trace" << ret << "\n";
            return ret;
        } else {
            ret = getTraceCount_binarySearch(hashIn, 0, (1 << 30));
            if (verbose > 2) cerr << "large trace" << ret << "\n";
        }
        return ret;
    }

    //-------------------------------------------------------------------------
    string_q getSha3(const string_q& hexIn) {
        return callRPC("web3_sha3", "[\"" + hexIn + "\"]", false);
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

    //-----------------------------------------------------------------------
    bool writeNodeToBinary(const CBaseNode& node, const string_q& fileName) {
        string_q created;
        if (establishFolder(fileName, created)) {
            if (!created.empty() && !isTestMode())
                cerr << "mkdir(" << created << ")" << string_q(75, ' ') << "\n";
            CArchive nodeCache(WRITING_ARCHIVE);
            if (nodeCache.Lock(fileName, binaryWriteCreate, LOCK_CREATE)) {
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
        if (nodeCache.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
            item.Serialize(nodeCache);
            nodeCache.Close();
            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------
    bool writeBlockToBinary(const CBlock& block, const string_q& fileName) {
        // CArchive blockCache(READING_ARCHIVE);  -- so search hits
        return writeNodeToBinary(block, fileName);
    }

    //-----------------------------------------------------------------------
    bool readBlockFromBinary(CBlock& block, const string_q& fileName) {
        // CArchive blockCache(READING_ARCHIVE);  -- so search hits
        return readNodeFromBinary(block, fileName);
    }

    //----------------------------------------------------------------------------------
    bool readBloomArray(CBloomArray& blooms, const string_q& fileName) {
        blooms.clear();
        CArchive bloomCache(READING_ARCHIVE);
        if (bloomCache.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
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
            if (bloomCache.Lock(fileName, binaryWriteCreate, LOCK_CREATE)) {
                bloomCache << blooms;
                bloomCache.Close();
                return true;
            }
        }
        return false;
    }

    //-------------------------------------------------------------------------
    static string_q getFilename_local(uint64_t numIn, bool asPath, bool asJson) {

        char ret[512];
        bzero(ret, sizeof(ret));

        string_q num = padLeft(uint_2_Str(numIn), 9, '0');
        string_q fmt = (asPath ? "%s/%s/%s/" : "%s/%s/%s/%s");
        string_q fn  = (asPath ? "" : num + (asJson ? ".json" : ".bin"));

        sprintf(ret, (blockCachePath("") + fmt).c_str(),  // NOLINT
                      extract(num, 0, 2).c_str(), extract(num, 2, 2).c_str(), extract(num, 4, 2).c_str(),
                      fn.c_str());
        return ret;
    }

    //-------------------------------------------------------------------------
    string_q getJsonFilename(uint64_t num) {
        return getFilename_local(num, false, true);
    }

    //-------------------------------------------------------------------------
    string_q getBinaryFilename(uint64_t num) {
        string_q ret = getFilename_local(num, false, false);
        replace(ret, "/00/",  "/blocks/00/");  // can't use Substitute because it will change them all
        return ret;
    }

    //-------------------------------------------------------------------------
    string_q getBinaryPath(uint64_t num) {
        string_q ret = getFilename_local(num, true, false);
        replace(ret, "/00/",  "/blocks/00/");  // can't use Substitute because it will change them all
        return ret;
    }

    //-------------------------------------------------------------------------
    bool forEveryBlock(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        // Here we simply scan the numbers and either read from disc or query the node
        if (!func)
            return false;

        for (uint64_t i = start ; i < start + count - 1 ; i = i + skip) {
            string_q fileName = getBinaryFilename(i);
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
    bool forEveryBlock(BLOCKVISITFUNC func, void *data, const string_q& block_list) {
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        if (!func)
            return false;

        // Read every block from number start to start+count
        for (uint64_t i = start ; i < start + count ; i = i + skip) {
            CBlock block;
            getBlock(block, i);
            if (!(*func)(block, data))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryNonEmptyBlockByNumber(UINT64VISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        if (!func)
            return false;

        CArchive fullBlockCache(READING_ARCHIVE);
        if (!fullBlockCache.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
            cerr << "forEveryNonEmptyBlockOnDisc failed: " << fullBlockCache.LockFailure() << "\n";
            return false;
        }
        ASSERT(fullBlockCache.isOpen());

        uint64_t nItems = fileSize(fullBlockIndex) / sizeof(uint64_t);
        uint64_t *items = new uint64_t[nItems];
        if (items) {
            // read the entire full block index
            fullBlockCache.Read(items, sizeof(uint64_t), nItems);
            fullBlockCache.Release();  // release it since we don't need it any longer

            for (uint64_t i = 0 ; i < nItems ; i = i + skip) {
                // TODO(tjayrush): This should be a binary search not a scan. This is why it appears to wait
                uint64_t item = items[i];
                if (inRange(item, start, start + count - 1)) {
                    bool ret = (*func)(items[i], data);
                    if (!ret) {
                        // Cleanup and return if user tells us to
                        delete [] items;
                        return false;
                    }
                } else {
                    // do nothing
                }
            }
            delete [] items;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryEmptyBlockByNumber(UINT64VISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        if (!func)
            return false;

        CArchive fullBlockCache(READING_ARCHIVE);
        if (!fullBlockCache.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
            cerr << "forEveryEmptyBlockOnDisc failed: " << fullBlockCache.LockFailure() << "\n";
            return false;
        }
        ASSERT(fullBlockCache.isOpen());

        uint64_t nItems = fileSize(fullBlockIndex) / sizeof(uint64_t) + 1;
        uint64_t *items = new uint64_t[nItems+2];
        if (!items) {
            cerr << "forEveryEmptyBlockOnDisc failed: could not allocate memory\n";
            return false;
        }

        fullBlockCache.Read(&items[0], sizeof(uint64_t), nItems);
        fullBlockCache.Release();

        CBlockRangeArray ranges;
        ranges.reserve(nItems * 35 / 100);  // less than 1/3 of blocks are empty

        uint64_t previous = (uint64_t)(start-1);
        uint64_t end = (start + count);
        for (size_t i = 0 ; i < nItems ; i++) {
            uint64_t current = items[i];
            if (start == 0 || (current >= start-1)) {
                int64_t diff = ((int64_t)current - (int64_t)previous) - 1;
                uint64_t udiff = (uint64_t)diff;
                if ((previous+1) <= (previous+udiff))
                    ranges.push_back(make_pair(previous+1, min(end, current)));
            }
            previous = current;
            if (current >= end)
                break;
        }

        blknum_t next = NOPOS;
        for (auto range : ranges) {
            blknum_t st = (next == NOPOS ? range.first : (max(next, range.first)));
            for (blknum_t bn = st ; bn < range.second ; bn += skip) {
                if (!(*func)(bn, data)) {
                    if (items)
                        delete [] items;
                    return false;
                }
                next = bn + skip;
            }
        }
        if (items)
            delete [] items;
        return true;
    }

    //-------------------------------------------------------------------------
    class CPassThru {
    public:
        BLOCKVISITFUNC origFunc;
        void *origData;
    };

    //-------------------------------------------------------------------------
    bool passThruFunction(uint64_t num, void *data) {
        CBlock block;
        getBlock(block, num);
        CPassThru *passThru = (CPassThru*)data;
        return passThru->origFunc(block, passThru->origData);
    }

    //-------------------------------------------------------------------------
    bool forEveryNonEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        CPassThru passThru;
        passThru.origFunc = func;
        passThru.origData = data;
        getCurlContext()->provider = "local";
        bool ret = forEveryNonEmptyBlockByNumber(passThruFunction, &passThru, start, count, skip);
        getCurlContext()->provider = "binary";
        return ret;
    }

    //-------------------------------------------------------------------------
    bool forEveryEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        CPassThru passThru;
        passThru.origFunc = func;
        passThru.origData = data;
        getCurlContext()->provider = "local";
        bool ret = forEveryEmptyBlockByNumber(passThruFunction, &passThru, start, count, skip);
        getCurlContext()->provider = "binary";
        return ret;
    }

    //-------------------------------------------------------------------------
    bool forEveryBloomFile(FILEVISITOR func, void *data, uint64_t start, uint64_t count, uint64_t skip) {

        // If the caller does not specify start/end block numbers, visit every bloom file
        if (start == 0 || count == (uint64_t)-1)
            return forEveryFileInFolder(bloomFolder, func, data);

        // The user is asking for certain files and not others. The bext we can do is limit which folders
        // to visit, which we do here. Caller must protect against too early or too late files by number.
        // Use interger math to figure out which folders to visit
        blknum_t st = (start / 1000) * 1000;
        blknum_t ed = ((start+count+1000) / 1000) * 1000;
        for (blknum_t b = st ; b < ed ; b += 1000) {
            string_q path = substitute(getBinaryPath(b), "/blocks/", "/blooms/");
            if (!forEveryFileInFolder(path, func, data))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryTraceInTransaction(TRACEVISITFUNC func, void *data, const CTransaction& trans) {

        if (!func)
            return false;

        CTraceArray traces;
        getTraces(traces, trans.hash);
        for (size_t i = 0 ; i < traces.size() ; i++) {
            CTrace trace = traces[i];
            if (!(*func)(trace, data))
                return false;
        }

        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryTraceInBlock(TRACEVISITFUNC func, void *data, const CBlock& block) {
        for (size_t i = 0 ; i < block.transactions.size() ; i++) {
            if (!forEveryTraceInTransaction(func, data, block.transactions[i]))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryLogInTransaction(LOGVISITFUNC func, void *data, const CTransaction& trans) {

        if (!func)
            return false;

//        cout << "Visiting " << trans.receipt.logs.size() << " logs\n";
//        cout.flush();
        for (size_t i = 0 ; i < trans.receipt.logs.size() ; i++) {
            CLogEntry log = trans.receipt.logs[i];
            if (!(*func)(log, data))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryLogInBlock(LOGVISITFUNC func, void *data, const CBlock& block) {
//        cout << "Visiting " << block.transactions.size() << " transactions\n";
//        cout.flush();
        for (size_t i = 0 ; i < block.transactions.size() ; i++) {
            if (!forEveryLogInTransaction(func, data, block.transactions[i]))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryTransactionInBlock(TRANSVISITFUNC func, void *data, const CBlock& block) {

        if (!func)
            return false;

        for (size_t i = 0 ; i < block.transactions.size() ; i++) {
            CTransaction trans = block.transactions[i];
            if (!(*func)(trans, data))
                return false;
        }

        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryTransactionInList(TRANSVISITFUNC func, void *data, const string_q& trans_list) {

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
            uint64_t txID = str_2_Uint(item);

            CTransaction trans;
            if (hasHex) {
                if (hasDot) {
                    // blockHash.txID
                    getTransaction(trans, hash, txID);

                } else {
                    // transHash
                    getTransaction(trans, hash);
                }
            } else {
                // blockNum.txID
                getTransaction(trans, str_2_Uint(hash), txID);
            }

            CBlock block;
            trans.pBlock = &block;
            if (isHash(trans.hash)) {
                // Note: at this point, we are not fully formed, we have to ask the node for the receipt
                getBlock(block, trans.blockNumber);
                if (block.transactions.size() > trans.transactionIndex)
                    trans.isError = block.transactions[trans.transactionIndex].isError;
                getReceipt(trans.receipt, trans.getValueByName("hash"));
                trans.finishParse();
            } else {
                // If the transaction has no hash here, there was a problem. Let the caller know
                trans.hash = orig + " invalid";
            }

            if (!(*func)(trans, data))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    string_q blockCachePath(const string_q& _part) {

        static string_q blockCache;
        if (blockCache.empty()) {
            CToml toml(configPath("quickBlocks.toml"));
            string_q path = toml.getConfigStr("settings", "blockCachePath", "<NOT_SET>");
            // cout << path << "\n";
            if (path == "<NOT_SET>") {
                path = configPath("cache/");
                toml.setConfigStr("settings", "blockCachePath", path);
                toml.writeFile();
            }
            CFilename folder(path);
            if (!folderExists(folder.getFullPath()))
                establishFolder(folder.getFullPath());
            if (!folder.isValid()) {
                cerr << "Invalid path (" << folder.getFullPath() << ") in config file. Quitting...\n";
                exit(0);
            }
            blockCache = folder.getFullPath();
            if (!endsWith(blockCache, "/"))
                blockCache += "/";
        }
        return substitute((blockCache + _part), "//", "/");
    }

    //--------------------------------------------------------------------------
    biguint_t weiPerEther = (modexp(10, 9, uint64_t(10000000000)) * modexp(10, 9, uint64_t(10000000000)));

    //-----------------------------------------------------------------------
    void manageFields(const string_q& listIn, bool show) {
        string_q list = substitute(listIn, " ", "");
        while (!list.empty()) {
            string_q fields = nextTokenClear(list, '|');
            string_q cl = nextTokenClear(fields, ':');
            CBaseNode *item =  createObjectOfType(cl);
            while (item && !fields.empty()) {
                string_q fieldName = nextTokenClear(fields, ',');
                if (fieldName == "all") {
                    if (show) {
                        item->getRuntimeClass()->showAllFields();
                    } else {
                        item->getRuntimeClass()->hideAllFields();
                    }
                } else if (fieldName == "none") {
                    if (show) {
                        item->getRuntimeClass()->hideAllFields();
                    } else {
                        item->getRuntimeClass()->showAllFields();
                    }
                } else {
                    CFieldData *f = item->getRuntimeClass()->findField(fieldName);
                    if (f)
                        f->setHidden(!show);
                }
            }
            delete item;
        }
    }

    //-----------------------------------------------------------------------
    string_q defHide =
        "CTransaction: nonce, input"
    "|" "CLogEntry: data, topics"
    "|" "CTrace: blockHash, blockNumber, transactionHash, transactionPosition, traceAddress, subtraces"
    "|" "CTraceAction: init"
    "|" "CTraceResult: code"
    "|" "CFunction: constant, payable, signature, encoding, type, articulate_str"
    "|" "CParameter: type, indexed, isPointer, isArray, isObject";

    string_q defShow =
        "CTransaction: price, gasCost, articulatedTx, traces, isError, date, ether"
    "|" "CLogEntry: articulatedLog"
    "|" "CTrace: articulatedTrace"
    "|" "CTraceAction: "
    "|" "CTraceResult: "
    "|" "CFunction: "
    "|" "CParameter: ";

}  // namespace qblocks
