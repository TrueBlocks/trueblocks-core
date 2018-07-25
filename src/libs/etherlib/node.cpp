/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
        if (!isNodeRunning() && fallBack.empty() && getCurlContext()->provider != "None") {
            cerr << "\n\t";
            cerr << cTeal << "Warning: " << cOff << "QuickBlocks requires a running Ethereum\n";
            cerr << "\tnode to operate properly. Please start your node.\n";
            cerr << "\tAlternatively, export FALLBACK=infura in your\n";
            cerr << "\tenvironment before running this command. Quitting...\n\n";
            cerr.flush();
            exit(0);
        }

        establishFolder(blockCachePath(""));

        // In case we create any lock files, so
        // they get cleaned up
        if (theQuitHandler == NULL || qh != defaultQuitHandler) {
            // Set this once, unless it's non-default
            theQuitHandler = qh;
extern void registerQuitHandler(QUITHANDLER qh);
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

        string_q trace;
        queryRawTrace(trace, hash);

        CRPCResult generic;
        char *p = cleanUpJson((char*)trace.c_str());  // NOLINT
        generic.parseJson(p);

        p = cleanUpJson((char *)(generic.result.c_str()));  // NOLINT
        while (p && *p) {
            CTrace tr;
            size_t nFields = 0;
            p = tr.parseJson(p, nFields);
            if (nFields)
                traces.push_back(tr);
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
        string_q results;
        queryRawBlock(results, numStr, true, false);
        CRPCResult generic;
        char *p = cleanUpJson((char*)results.c_str());  // NOLINT
        generic.parseJson(p);
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
            addrs.push_back(nextTokenClear(results, ','));
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
    bool getCode(const string_q& addr, string_q& theCode) {
        string_q a = startsWith(addr, "0x") ? extract(addr, 2) : addr;
        a = padLeft(a, 40, '0');
        theCode = callRPC("eth_getCode", "[\"0x" + a +"\"]", false);
        return theCode.length() != 0;
    }

    //-------------------------------------------------------------------------
    biguint_t getBalance(const string_q& addr, blknum_t blockNum, bool isDemo) {
        string_q a = extract(addr, 2);
        a = padLeft(a, 40, '0');
        string_q ret = callRPC("eth_getBalance", "[\"0x" + a + "\",\"" + uint_2_Hex(blockNum) + "\"]", false);
        return str_2_Wei(ret);
    }

    //-------------------------------------------------------------------------
    bool nodeHasBalances(void) {
        // Account 0xa1e4380a3b1f749673e270229993ee55f35663b4 owned 2000000000000000000000 (2000 ether)
        // at block zero. If the node is holding balances (i.e. its an archive node), then it will
        // return that value for block 1 as well. Otherwise, it will return a zero balance.
        // NOTE: Account 0xa1e4380a3b1f749673e270229993ee55f35663b4 transacted in the first ever transaction.
        return getBalance("0xa1e4380a3b1f749673e270229993ee55f35663b4", 1, false) ==
                            str_2_Wei("2000000000000000000000");
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
    bool getSha3(const string_q& hexIn, string_q& shaOut) {
        shaOut = callRPC("web3_sha3", "[\"" + hexIn + "\"]", false);
        return true;
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
        string_q contents = asciiFileToString(fileName);
        if (contains(contents, "null")) {
            replaceAll(contents, "null", "\"0x\"");
            stringToAsciiFile(fileName, contents);
        }
        if (!endsWith(contents, '\n')) {
            stringToAsciiFile(fileName, contents + "\n");
        }
        char *p = cleanUpJson((char *)contents.c_str());  // NOLINT
        size_t nFields = 0;
        node.parseJson(p, nFields);
        return nFields;
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
    bool forEveryNonEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {

        // Read the non-empty block index file and spit it out only non-empty blocks
        if (!func)
            return false;

        CArchive fullBlockCache(READING_ARCHIVE);
        if (!fullBlockCache.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
            cerr << "forEveryNonEmptyBlockOnDisc failed: " << fullBlockCache.LockFailure() << "\n";
            return false;
        }
        ASSERT(fullBlockCache.isOpen());

        uint64_t nItems = fileSize(fullBlockIndex) / sizeof(uint64_t);
        uint64_t *contents = new uint64_t[nItems];
        if (contents) {
            // read the entire full block index
            fullBlockCache.Read(contents, sizeof(uint64_t), nItems);
            fullBlockCache.Release();  // release it since we don't need it any longer

            for (uint64_t i = 0 ; i < nItems ; i = i + skip) {
                // TODO(tjayrush): This should be a binary search not a scan. This is why it appears to wait
                uint64_t item = contents[i];
                if (inRange(item, start, start + count - 1)) {
                    CBlock block;
                    if (getBlock(block, contents[i])) {
                        bool ret = (*func)(block, data);
                        if (!ret) {
                            // Cleanup and return if user tells us to
                            delete [] contents;
                            return false;
                        }
                    }
                } else {
                    // do nothing
                }
            }
            delete [] contents;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        if (!func)
            return false;

        getCurlContext()->provider = "local";   // the empty blocks are not on disk, so we have to
                                                // ask parity. Don't write them, though

        CArchive fullBlockCache(READING_ARCHIVE);
        if (!fullBlockCache.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
            cerr << "forEveryEmptyBlockOnDisc failed: " << fullBlockCache.LockFailure() << "\n";
            return false;
        }
        ASSERT(fullBlockCache.isOpen());

        uint64_t nItems = fileSize(fullBlockIndex) / sizeof(uint64_t) + 1;  // we need an extra one for item '0'
        uint64_t *contents = new uint64_t[nItems+2];  // extra space
        if (contents) {
            // read the entire full block index
            fullBlockCache.Read(&contents[0], sizeof(uint64_t), nItems-1);  // one less since we asked for an extra one
            fullBlockCache.Release();  // release it since we don't need it any longer

            contents[0] = 0;  // Starting point (because we are build the empty list from the non-empty list)
            uint64_t cnt = start;
            for (uint64_t i = 1 ; i < nItems ; i = i + skip) {  // first one is assumed to be the '0' block
                while (cnt < contents[i]) {
                    CBlock block;
                    // Both 'queryBlock' and 'getBlock' return false if there are no
                    // transactions, so we ignore the return value
                    getBlock(block, cnt);
                    if (!(*func)(block, data)) {
                        getCurlContext()->provider = "binary";
                        delete [] contents;
                        return false;
                    }
                    cnt++;  // go to the next one
                }
                cnt++;  // contents[i] has transactions, so skip it
            }
            delete [] contents;
        }
        getCurlContext()->provider = "binary";
        return true;
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
            bool hasDot = contains(item, ".");
            bool hasHex = startsWith(item, "0x");

            string_q hash = nextTokenClear(item, '.');
            uint64_t txID = str_2_Uint(item);

            CTransaction trans;
            if (hasHex) {
                if (hasDot) {
                    // We are not fully formed, we have to ask the node for the receipt
                    getTransaction(trans, hash, txID);  // blockHash.txID
                } else {
                    // We are not fully formed, we have to ask the node for the receipt
                    getTransaction(trans, hash);  // transHash
                }
            } else {
                getTransaction(trans, str_2_Uint(hash), txID);  // blockNum.txID
            }

            CBlock block;
            trans.pBlock = &block;
            getBlock(block, trans.blockNumber);
            if (block.transactions.size() > trans.transactionIndex)
                trans.isError = block.transactions[trans.transactionIndex].isError;
            getReceipt(trans.receipt, trans.getValueByName("hash"));
            trans.finishParse();
            if (!isHash(trans.hash)) {
                // If the transaction has no hash here, either the block hash or the transaction hash being asked
                // for doesn't exist. We need to report which hash failed and why to the caller. Because we have
                // no better way, we report that in the hash itself. There are three cases, two with either block
                // hash or block num one with transaction hash. Note: This will fail if we move to non-string hashes
                trans.hash = hash + "-" + (!hasHex || hasDot ? "block_not_found" : "trans_not_found");
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

}  // namespace qblocks
