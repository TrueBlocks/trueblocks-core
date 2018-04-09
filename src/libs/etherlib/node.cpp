/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "node.h"

namespace qblocks {

    static QUITHANDLER theQuitHandler = NULL;
    //-------------------------------------------------------------------------
    void etherlib_init(const SFString& sourceIn, QUITHANDLER qh) {

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
        CNameValue::registerClass();
        CAccountName::registerClass();

        if (sourceIn != "remote" && sourceIn != "local" && sourceIn != "ropsten")
            getCurlContext()->source = "binary";
        else
            getCurlContext()->source = sourceIn;

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
    }

    //-------------------------------------------------------------------------
    bool getBlock(CBlock& block, blknum_t blockNum) {
        getCurlContext()->source = fileExists(getBinaryFilename(blockNum)) ? "binary" : "local";
        bool ret = queryBlock(block, asStringU(blockNum), true, false);
        getCurlContext()->source = "binary";
        return ret;
    }

    //-------------------------------------------------------------------------
    bool getBlock(CBlock& block, const SFHash& blockHash) {
        return queryBlock(block, blockHash, true, true);
    }

    //-------------------------------------------------------------------------
    bool getTransaction(CTransaction& trans, const SFHash& txHash) {
        getObjectViaRPC(trans, "eth_getTransactionByHash", "[\"" + fixHash(txHash) +"\"]");
        trans.finishParse();
        return true;
    }

    //-------------------------------------------------------------------------
    bool getTransaction(CTransaction& trans, const SFHash& blockHash, txnum_t txID) {
        getObjectViaRPC(trans, "eth_getTransactionByBlockHashAndIndex", "[\"" + fixHash(blockHash) +"\",\"" + toHex(txID) + "\"]");
        trans.finishParse();
        return true;
    }

    //-------------------------------------------------------------------------
    bool getTransaction(CTransaction& trans, blknum_t blockNum, txnum_t txID) {
        if (fileExists(getBinaryFilename(blockNum))) {
            CBlock block;
            readFromBinary(block, getBinaryFilename(blockNum));
            if (txID < block.transactions.getCount())
            {
                trans = block.transactions[(uint32_t)txID];
                trans.pBlock = NULL; // otherwise, it's pointing to a dead pointer
                return true;
            }
            // fall through to node
        }

        getObjectViaRPC(trans, "eth_getTransactionByBlockNumberAndIndex", "[\"" + toHex(blockNum) +"\",\"" + toHex(txID) + "\"]");
        trans.finishParse();
        return true;
    }

    //-------------------------------------------------------------------------
    bool getReceipt(CReceipt& receipt, const SFHash& txHash) {
        receipt = CReceipt();
        getObjectViaRPC(receipt, "eth_getTransactionReceipt", "[\"" + fixHash(txHash) + "\"]");
        return true;
    }

    //--------------------------------------------------------------
    void getTraces(CTraceArray& traces, const SFHash& hash) {

        SFString trace;
        queryRawTrace(trace, hash);

        CRPCResult generic;
        char *p = cleanUpJson((char*)(const char*)trace);
        generic.parseJson(p);

        p = cleanUpJson((char *)(generic.result.c_str()));  // NOLINT
        while (p && *p) {
            CTrace tr;
            uint32_t nFields = 0;
            p = tr.parseJson(p, nFields);
            if (nFields)
                traces[traces.getCount()] = tr;
        }
    }

    //-------------------------------------------------------------------------
    bool queryBlock(CBlock& block, const SFString& datIn, bool needTrace, bool byHash) {
        uint32_t unused = 0;
        return queryBlock(block, datIn, needTrace, byHash, unused);
    }

    //-------------------------------------------------------------------------
    bool queryBlock(CBlock& block, const SFString& datIn, bool needTrace, bool byHash, uint32_t& nTraces) {

        if (datIn == "latest")
            return queryBlock(block, asStringU(getLatestBlockFromClient()), needTrace, false);

        if (isHash(datIn)) {
            HIDE_FIELD(CTransaction, "receipt");
            getObjectViaRPC(block, "eth_getBlockByHash", "["+quote(datIn)+",true]");

        } else {
            uint64_t num = toLongU(datIn);
            if (getCurlContext()->source == "binary" && fileSize(getBinaryFilename(num)) > 0) {
                UNHIDE_FIELD(CTransaction, "receipt");
                block = CBlock();
                return readFromBinary(block, getBinaryFilename(num));

            }

            HIDE_FIELD(CTransaction, "receipt");
            getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(toHex(num))+",true]");
        }

        // If there are no transactions, we do not have to trace and we want to tell the caller that
        if (!block.transactions.getCount())
            return false;

        // We have the transactions, but we also want the receipts, and we need an error indication
        nTraces=0;
        for (uint32_t i=0;i<block.transactions.getCount();i++)
        {
            CTransaction *trans = &block.transactions[i];
            trans->pBlock = &block;

            UNHIDE_FIELD(CTransaction, "receipt");
            CReceipt receipt;
            getReceipt(receipt, trans->hash);
            trans->receipt = receipt; // deep copy
            if (block.blockNumber >= byzantiumBlock) {
                trans->isError = (receipt.status == 0);

            } else if (needTrace && trans->gas == receipt.gasUsed) {

                // If we've been told not to trace, quit here, but return sucess
                if (!getCurlContext()->isTracingOn())
                    return true;

                SFString trace;
                getCurlContext()->lightTracing(true);
                queryRawTrace(trace, trans->hash);
                trans->isError = getCurlContext()->lightTracing(false);
                nTraces++;
            }
        }

        return true;
    }

    //-------------------------------------------------------------------------
    bool queryRawBlock(SFString& blockStr, const SFString& datIn, bool needTrace, bool hashesOnly) {

        if (isHash(datIn)) {
            blockStr = callRPC("eth_getBlockByHash", "["+quote(datIn)+","+(hashesOnly?"false":"true")+"]", true);
        } else {
            blockStr = callRPC("eth_getBlockByNumber", "["+quote(toHex(datIn))+","+(hashesOnly?"false":"true")+"]", true);
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool queryRawTransaction(SFString& results, const SFHash& txHash) {
        SFString data = "[\"[HASH]\"]";
        data.Replace("[HASH]", txHash);
        results = callRPC("eth_getTransactionByHash", data, true);
        return true;
    }

    //-------------------------------------------------------------------------
    bool queryRawReceipt(SFString& results, const SFHash& txHash) {
        SFString data = "[\"[HASH]\"]";
        data.Replace("[HASH]", txHash);
        results = callRPC("eth_getTransactionReceipt", data, true);
        return true;
    }

    //-------------------------------------------------------------------------
    bool queryRawTrace(SFString& trace, const SFString& hashIn) {
        trace = "[" + callRPC("trace_transaction", "[\"" + fixHash(hashIn) +"\"]", true) + "]";
        return true;
    }

    //-------------------------------------------------------------------------
    bool queryRawLogs(SFString& results, const SFAddress& addr, uint64_t fromBlock, uint64_t toBlock) {
        SFString data = "[{\"fromBlock\":\"[START]\",\"toBlock\":\"[STOP]\", \"address\": \"[ADDR]\"}]";
        data.Replace("[START]", toHex(fromBlock));
        data.Replace("[STOP]",  toHex(toBlock));
        data.Replace("[ADDR]",  fromAddress(addr));
        results = callRPC("eth_getLogs", data, true);
        return true;
    }

    //-------------------------------------------------------------------------
    SFString getVersionFromClient(void) {
        return callRPC("web3_clientVersion", "[]", false);
    }

    //-------------------------------------------------------------------------
    bool getAccounts(SFAddressArray& addrs) {
        SFString results = callRPC("eth_accounts", "[]", false);
        while (!results.empty())
            addrs[addrs.getCount()] = nextTokenClear(results,',');
        return true;
    }

    //-------------------------------------------------------------------------
    uint64_t getLatestBlockFromClient(void) {
        SFString ret = callRPC("eth_blockNumber", "[]", false);
        uint64_t retN = toUnsigned(ret);
        if (retN == 0) {
            // Try a different way just in case. Geth, for example, doesn't
            // return blockNumber until the chain is synced (Parity may--don't know
            // We fall back to this method just in case
            SFString str = callRPC("eth_syncing", "[]", false);
            str.Replace("currentBlock:","|");
            nextTokenClear(str,'|');
            str = nextTokenClear(str,',');
            retN = toUnsigned(str);
        }
        return retN;
    }

    //--------------------------------------------------------------------------
    uint64_t getLatestBlockFromCache(CSharedResource *res) {

        uint64_t ret = 0;

        CSharedResource fullBlocks; // Don't move--need the scope
        CSharedResource *pRes = res;
        if (!pRes) {
            // We're reading so okay not to wait
            if (!fullBlocks.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
                if (!isTestMode())
                    cerr << "getLatestBlockFromCache failed: " << fullBlocks.LockFailure() << "\n";
                return ret;
            }
            pRes = &fullBlocks;
        }
        ASSERT(pRes->isOpen());

        pRes->Seek( (-1 * (long)sizeof(uint64_t)), SEEK_END);
        pRes->Read(ret);
        if (pRes != res)
            pRes->Release();
        return ret;
    }

    //--------------------------------------------------------------------------
    bool getLatestBlocks(uint64_t& cache, uint64_t& client, CSharedResource *res) {
        client = getLatestBlockFromClient();
        cache  = getLatestBlockFromCache(res);
        return true;
    }

    //-------------------------------------------------------------------------
    bool getCode(const SFString& addr, SFString& theCode) {
        SFString a = addr.startsWith("0x") ? addr.substr(2) : addr;
        a = padLeft(a,40,'0');
        theCode = callRPC("eth_getCode", "[\"0x" + a +"\"]", false);
        return theCode.length()!=0;
    }

    //-------------------------------------------------------------------------
    SFUintBN getBalance(const SFString& addr, blknum_t blockNum, bool isDemo) {
        SFString a = addr.substr(2);
        a = padLeft(a,40,'0');
        SFString ret = callRPC("eth_getBalance", "[\"0x" + a +"\",\""+toHex(blockNum)+"\"]", false);
        return toWei(ret);
    }

    //-------------------------------------------------------------------------
    bool hasTraceAt(const SFString& hashIn, uint32_t where) {
        SFString cmd = "[\"" + fixHash(hashIn) +"\",[\"" + toHex(where) + "\"]]";
        SFString ret = callRPC("trace_get", cmd.c_str(), true);
        return ret.find("blockNumber") != NOPOS;
    }

    //--------------------------------------------------------------
    uint32_t getTraceCount_binarySearch(const SFHash& hashIn, uint32_t first, uint32_t last) {
        if (last > first) {
            uint32_t mid = first + ((last - first) / 2);
            bool atMid  = hasTraceAt(hashIn, mid);
            bool atMid1 = hasTraceAt(hashIn, mid + 1);
            if (atMid && !atMid1)
                return mid; // found it
            if (!atMid) {
                // we're too high, so search below
                return getTraceCount_binarySearch(hashIn, first, mid-1);
            }
            // we're too low, so search above
            return getTraceCount_binarySearch(hashIn, mid+1, last);
        }
        return first;
    }

    // https://ethereum.stackexchange.com/questions/9883/why-is-my-node-synchronization-stuck-extremely-slow-at-block-2-306-843/10453
    //--------------------------------------------------------------
    uint32_t getTraceCount(const SFHash& hashIn) {
        // handle most likely cases linearly
        for (uint32_t n = 2 ; n < 8 ; n++) {
            if (!hasTraceAt(hashIn, n)) {
                if (verbose>2) cerr << "tiny trace" << (n-1) << "\n";
                return n-1;
            }
        }

        // binary search the rest
        uint32_t ret = 0;
        if (!hasTraceAt(hashIn, (1<<8))) { // small?
            ret = getTraceCount_binarySearch(hashIn, 0, (1<<8)-1);
            if (verbose>2) cerr << "small trace" << ret << "\n";
            return ret;
        } else if (!hasTraceAt(hashIn, (1<<16))) { // medium?
            ret = getTraceCount_binarySearch(hashIn, 0, (1<<16)-1);
            if (verbose>2) cerr << "medium trace" << ret << "\n";
            return ret;
        } else {
            ret = getTraceCount_binarySearch(hashIn, 0, (1<<30)); // TODO: is this big enough?
            if (verbose>2) cerr << "large trace" << ret << "\n";
        }
        return ret;
    }

    //-------------------------------------------------------------------------
    bool getSha3(const SFString& hexIn, SFString& shaOut) {
        shaOut = callRPC("web3_sha3", "[\"" + hexIn + "\"]", false);
        return true;
    }

    //-----------------------------------------------------------------------
    bool readOneBlock_fromJson(CBlock& block, const SFString& fileName) {
        if (!fileExists(fileName)) {
            cerr << "File not found " << fileName << "\n";
            return false;
        }
        block = CBlock(); // reset
        SFString contents = asciiFileToString(fileName);
        if (contents.Contains("null")) {
            contents.ReplaceAll("null", "\"0x\"");
            stringToAsciiFile(fileName, contents);
        }
        if (!contents.endsWith('\n')) {
            stringToAsciiFile(fileName, contents+"\n");
        }
        char *p = cleanUpJson((char *)(const char*)contents);
        uint32_t nFields=0;
        block.parseJson(p,nFields);
        return nFields;
    }

    //-----------------------------------------------------------------------
    void writeToJson(const CBaseNode& node, const SFString& fileName) {
        if (establishFolder(fileName)) {
            std::ofstream out(fileName);
            out << node.Format() << "\n";
            out.close();
        }
    }

    //-----------------------------------------------------------------------
    bool readFromJson(CBaseNode& node, const SFString& fileName) {
        return false;
    };

    //-----------------------------------------------------------------------
    bool writeToBinary(const CBaseNode& node, const SFString& fileName) {
        SFString created;
        if (establishFolder(fileName,created)) {
            if (!created.empty() && !isTestMode())
                cerr << "mkdir(" << created << ")" << SFString(' ',20) << "                                                     \n";
            SFArchive archive(WRITING_ARCHIVE);
            if (archive.Lock(fileName, binaryWriteCreate, LOCK_CREATE)) {
                node.SerializeC(archive);
                archive.Close();
                return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    bool readFromBinary(CBaseNode& item, const SFString& fileName) {
        // Assumes that the item is clear, so no Init
        SFArchive archive(READING_ARCHIVE);
        if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
            item.Serialize(archive);
            archive.Close();
            return true;
        }
        return false;
    }

    //-------------------------------------------------------------------------
    static SFString getFilename_local(uint64_t numIn, bool asPath, bool asJson) {

        char ret[512];
        bzero(ret,sizeof(ret));

        SFString num = padLeft(asStringU(numIn),9,'0');
        SFString fmt = (asPath ? "%s/%s/%s/" : "%s/%s/%s/%s");
        SFString fn  = (asPath ? "" : num + (asJson ? ".json" : ".bin"));

        sprintf(ret, (const char*)(blockCachePath("")+fmt),
                      (const char*)num.substr(0,2),
                      (const char*)num.substr(2,2),
                      (const char*)num.substr(4,2),
                      (const char*)fn);
        return ret;
    }

    //-------------------------------------------------------------------------
    SFString getJsonFilename(uint64_t num) {
        return getFilename_local(num, false, true);
    }

    //-------------------------------------------------------------------------
    SFString getBinaryFilename(uint64_t num) {
        SFString ret = getFilename_local(num, false, false);
        ret.Replace("/00/",  "/blocks/00/"); // can't use Substitute because it will change them all
        return ret;
    }

    //-------------------------------------------------------------------------
    SFString getBinaryPath(uint64_t num) {
        SFString ret = getFilename_local(num, true, false);
        ret.Replace("/00/",  "/blocks/00/"); // can't use Substitute because it will change them all
        return ret;
    }

    //----------------------------------------------------------------------------------
    bool readBloomArray(SFBloomArray& blooms, const SFString& fileName) {
        blooms.Clear();
        SFArchive archive(READING_ARCHIVE);
        if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
            archive >> blooms;
            archive.Close();
            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------
    bool writeBloomArray(const SFBloomArray& blooms, const SFString& fileName) {
        if (blooms.getCount() == 0 || (blooms.getCount() == 1 && blooms[0] == 0))
            return false;

        SFString created;
        if (establishFolder(fileName,created)) {
            if (!created.empty() && !isTestMode())
                cerr << "mkdir(" << created << ")" << SFString(' ',20) << "                                                     \n";
            SFArchive archive(WRITING_ARCHIVE);
            if (archive.Lock(fileName, binaryWriteCreate, LOCK_CREATE)) {
                archive << blooms;
                archive.Close();
                return true;
            }
        }
        return false;
    }

    //-------------------------------------------------------------------------
    bool forEveryBlock(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        // Here we simply scan the numbers and either read from disc or query the node
        if (!func)
            return false;

        for (uint64_t i = start ; i < start + count - 1 ; i = i + skip) {
            SFString fileName = getBinaryFilename(i);
            CBlock block;
            if (fileExists(fileName)) {
                block = CBlock();
                readFromBinary(block, fileName);
            } else {
                getBlock(block,i);
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
    bool forEveryBlock(BLOCKVISITFUNC func, void *data, const SFString& block_list) {
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        if (!func)
            return false;

        // Read every block from number start to start+count
        for (uint64_t i = start ; i < start + count ; i = i + skip) {
            CBlock block;
            getBlock(block,i);
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

        CSharedResource fullBlocks;
        if (!fullBlocks.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
            cerr << "forEveryNonEmptyBlockOnDisc failed: " << fullBlocks.LockFailure() << "\n";
            return false;
        }
        ASSERT(fullBlocks.isOpen());

        uint64_t nItems = fileSize(fullBlockIndex) / sizeof(uint64_t);
        uint64_t *contents = new uint64_t[nItems];
        if (contents) {
            // read the entire full block index
            fullBlocks.Read(contents, sizeof(uint64_t), nItems);
            fullBlocks.Release();  // release it since we don't need it any longer

            for (uint64_t i = 0 ; i < nItems ; i = i + skip) {
                // TODO: This should be a binary search not a scan. This is why it appears to wait
                uint64_t item = contents[i];
                if (inRange(item, start, start+count-1)) {
                    CBlock block;
                    if (getBlock(block,contents[i])) {
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

        getCurlContext()->source = "local"; // the empty blocks are not on disk, so we have to ask parity. Don't write them, though

        CSharedResource fullBlocks;
        if (!fullBlocks.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
            cerr << "forEveryEmptyBlockOnDisc failed: " << fullBlocks.LockFailure() << "\n";
            return false;
        }
        ASSERT(fullBlocks.isOpen());

        uint64_t nItems = fileSize(fullBlockIndex) / sizeof(uint64_t) + 1;  // we need an extra one for item '0'
        uint64_t *contents = new uint64_t[nItems+2];  // extra space
        if (contents) {
            // read the entire full block index
            fullBlocks.Read(&contents[0], sizeof(uint64_t), nItems-1);  // one less since we asked for an extra one
            fullBlocks.Release();  // release it since we don't need it any longer

            contents[0] = 0;  // the starting point (needed because we are build the empty list from the non-empty list
            uint64_t cnt = start;
            for (uint64_t i = 1 ; i < nItems ; i = i + skip) { // first one (at index '0') is assumed to be the '0' block
                while (cnt<contents[i]) {
                    CBlock block;
                    // Both 'queryBlock' and 'getBlock' return false if there are no
                    // transactions, so we ignore the return value
                    getBlock(block,cnt);
                    if (!(*func)(block, data)) {
                        getCurlContext()->source = "binary";
                        delete [] contents;
                        return false;
                    }
                    cnt++; // go to the next one
                }
                cnt++; // contents[i] has transactions, so skip it
            }
            delete [] contents;
        }
        getCurlContext()->source = "binary";
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
            SFString path = getBinaryPath(b).Substitute("/blocks/","/blooms/");
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
        for (uint32_t i=0;i<traces.getCount();i++) {
            CTrace trace = traces[i];
            if (!(*func)(trace, data))
                return false;
        }

        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryTraceInBlock(TRACEVISITFUNC func, void *data, const CBlock& block) {
        for (uint32_t i = 0 ; i < block.transactions.getCount() ; i++) {
            if (!forEveryTraceInTransaction(func, data, block.transactions[i]))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryLogInTransaction(LOGVISITFUNC func, void *data, const CTransaction& trans) {

        if (!func)
            return false;

//        cout << "Visiting " << trans.receipt.logs.getCount() << " logs\n";
//        cout.flush();
        for (uint32_t i = 0 ; i < trans.receipt.logs.getCount() ; i++) {
            CLogEntry log = trans.receipt.logs[i];
            if (!(*func)(log, data))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryLogInBlock(LOGVISITFUNC func, void *data, const CBlock& block) {
//        cout << "Visiting " << block.transactions.getCount() << " transactions\n";
//        cout.flush();
        for (uint32_t i = 0 ; i < block.transactions.getCount() ; i++) {
            if (!forEveryLogInTransaction(func, data, block.transactions[i]))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryTransactionInBlock(TRANSVISITFUNC func, void *data, const CBlock& block) {

        if (!func)
            return false;

        for (uint32_t i = 0 ; i < block.transactions.getCount() ; i++) {
            CTransaction trans = block.transactions[i];
            if (!(*func)(trans, data))
                return false;
        }

        return true;
    }

    //-------------------------------------------------------------------------
    bool forEveryTransactionInList(TRANSVISITFUNC func, void *data, const SFString& trans_list) {

        if (!func)
            return false;

        // trans_list is a list of tx_hash, blk_hash.tx_id, or blk_num.tx_id, or any combination
        SFString list = trans_list;
        while (!list.empty()) {
            SFString item = nextTokenClear(list, '|');
            bool hasDot = item.Contains(".");
            bool isHex = item.startsWith("0x");

            SFString hash = nextTokenClear(item, '.');
            uint64_t txID = toLongU(item);

            CTransaction trans;
            if (isHex) {
                if (hasDot) {
                    // We are not fully formed, we have to ask the node for the receipt
                    getTransaction(trans, hash, txID);  // blockHash.txID
                } else {
                    // We are not fully formed, we have to ask the node for the receipt
                    getTransaction(trans, hash);  // transHash
                }
            } else {
                getTransaction(trans, (uint32_t)toLongU(hash), txID);  // blockNum.txID
            }

            CBlock block;
            trans.pBlock = &block;
            getBlock(block, trans.blockNumber);
            if (block.transactions.getCount() > trans.transactionIndex)
                trans.isError = block.transactions[(uint32_t)trans.transactionIndex].isError;
            getReceipt(trans.receipt, trans.getValueByName("hash"));
            trans.finishParse();
            if (!isHash(trans.hash)) {
                // If the transaction has no hash here, either the block hash or the transaction hash being asked for doesn't exist. We need to
                // report which hash failed and why to the caller. Because we have no better way, we report that in the hash itself. There are
                // three cases, two with either block hash or block num one with transaction hash. Note: This will fail if we move to non-string hashes
                trans.hash = hash + "-" + (!isHex || hasDot ? "block_not_found" : "trans_not_found");
            }

            if (!(*func)(trans, data))
                return false;
        }
        return true;
    }

    //-------------------------------------------------------------------------
    SFString blockCachePath(const SFString& _part) {

        static SFString blockCache;
        if (blockCache.empty()) {
            CToml toml(configPath("quickBlocks.toml"));
            SFString path = toml.getConfigStr("settings", "blockCachePath", "<NOT_SET>");
            //cout << path << "\n";
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
        }
        if (!blockCache.endsWith("/"))
            blockCache += "/";
        return blockCache + _part;
    }

    //--------------------------------------------------------------------------
    SFUintBN weiPerEther = (modexp(10,9,10000000000)*modexp(10,9,10000000000));

}  // namespace qblocks
