/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "node.h"

namespace qblocks {

namespace qbGlobals {
    static SFString source;
}
//-------------------------------------------------------------------------
SFString curSource(void) { return qbGlobals::source; }

//-------------------------------------------------------------------------
CURL *getCurl(bool cleanup=false)
{
    static CURL *curl = NULL;
    static struct curl_slist *headers = NULL;
    if (!curl)
    {
        curl = curl_easy_init();
        if (!curl)
        {
            fprintf(stderr, "Curl failed to initialize. Quitting...\n");
            exit(0);
        }

        headers = curl_slist_append(headers, "Content-Type: application/json");

        if (qbGlobals::source == "infura")
        {
            // we have to use Infura
            headers = curl_slist_append(headers, "Infura-Ethereum-Preferred-Client: parity");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL,        "https://mainnet.infura.io/WkavvX9Hk5tvp34LhN7W");

        } else
        {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL,        "http://localhost:8545");
        }

    } else if (cleanup)
    {
        if (headers)
            curl_slist_free_all(headers);
        if (curl)
            curl_easy_cleanup(curl);
        headers = NULL;
        curl = NULL;
        return NULL;
    }

    return curl;
}

//-------------------------------------------------------------------------
void etherlib_init(const SFString& sourceIn)
{
    // In case we create any lock files, so
    // they get cleaned up
    registerQuitHandler(defaultQuitHandler);

    qbGlobals::source = sourceIn;

    CBlock::registerClass();
    CTransaction::registerClass();
    CReceipt::registerClass();
    CLogEntry::registerClass();
    CPriceQuote::registerClass();
    CBalHistory::registerClass();
    CAccountWatch::registerClass();
    CIncomeStatement::registerClass();
    CTrace::registerClass();
    CTraceAction::registerClass();
    CTraceResult::registerClass();

    HIDE_FIELD(CTransaction, "isError");
    HIDE_FIELD(CTransaction, "isInternalTx");
    HIDE_FIELD(CTransaction, "date");
    HIDE_FIELD(CTransaction, "ether");

    // initialize curl
    getCurl();

    establishFolder(configPath());
}

//-------------------------------------------------------------------------
void etherlib_cleanup(void)
{
    // cleanup curl
    getCurl(true);
}

extern size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
static bool earlyAbort=false;
//-------------------------------------------------------------------------
// Use 'curl' to make an arbitrary rpc call
//-------------------------------------------------------------------------
SFString callRPC(const SFString& method, const SFString& params, bool raw)
{
    static uint32_t id = 1;
    SFString thePost, received;

    thePost += "{";
    thePost +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
    thePost +=  quote("method")  + ":"  + quote(method) + ",";
    thePost +=  quote("params")  + ":"  + params        + ",";
    thePost +=  quote("id")      + ":"  + quote(asString(id++));
    thePost += "}";

//#define DEBUG_RPC
#ifdef DEBUG_RPC
    cerr << "\n" << SFString('-',80) << "\n";
    cerr << thePost << "\n";
    cerr << SFString('-',80) << "\n";
    cerr.flush();
#endif

    curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDS,    (const char*)thePost);
    curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDSIZE, thePost.length());

    curl_easy_setopt(getCurl(), CURLOPT_WRITEDATA,     &received);
    curl_easy_setopt(getCurl(), CURLOPT_WRITEFUNCTION, write_callback);

    earlyAbort = false;
    CURLcode res = curl_easy_perform(getCurl());
    if (res != CURLE_OK && !earlyAbort)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        exit(0);
    }

#ifdef DEBUG_RPC
//    cout << "\n" << SFString('-',80) << "\n";
//    cout << thePost << "\n";
    cout << SFString('=',60) << "\n";
    cout << received << "\n";
    cout.flush();
#endif

    if (raw)
        return received;
    CRPCResult generic;
    char *p = cleanUpJson((char*)(const char*)received);
    generic.parseJson(p);
    return generic.result;
}

//-------------------------------------------------------------------------
bool getObjectViaRPC(CBaseNode &node, const SFString& method, const SFString& params)
{
    SFString ret = callRPC(method, params, false);
    node.parseJson((char *)(const char*)ret);
    return true;
}

// TODO: remove golobal data
static SFUint32 nTrans=0,nTraced=0;
//-------------------------------------------------------------------------
bool queryBlock(CBlock& block, const SFString& numIn, bool needTrace)
{
    if (numIn=="latest")
        return queryBlock(block, asString(getLatestBlockFromClient()), needTrace);

    long num = toLong(numIn);
    if ((qbGlobals::source.Contains("binary") || qbGlobals::source.Contains("nonemp")) && fileSize(getBinaryFilename1(num))>0) {
        //		if (verbose) { cerr << "Reading binary block: " << num << "\n"; cerr.flush(); }
        UNHIDE_FIELD(CTransaction, "receipt");
        return readOneBlock_fromBinary(block, getBinaryFilename1(num));

    } else if (qbGlobals::source.Contains("Only")) {
        return false;

    }

    if (qbGlobals::source == "json" && fileSize(getJsonFilename1(num))>0)
    {
        //		if (verbose) { cerr << "Reading json block: " << num << "\n"; cerr.flush(); }
        UNHIDE_FIELD(CTransaction, "receipt");
        return readOneBlock_fromJson(block, getJsonFilename1(num));

    }

    //	if (verbose) { cerr << "Getting block from node: " << num << "\n"; cerr.flush(); }
    HIDE_FIELD(CTransaction, "receipt");
    getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(asString(num))+",true]");

    // If there are no transactions, we're done
    if (!block.transactions.getCount())
    {
        // We only write binary if there are transactions
        //writeToBinary(block, getBinaryFilename1(num));
        //writeToJson(block, getJsonFilename1(num)); //We've stopped writing JSON for now because of disc space
        return false;
    }

    // We have the transactions, but we also want the receipts
    SFUint32 nTraces=0;
    for (uint32_t i=0;i<block.transactions.getCount();i++)
    {
        CTransaction *trans = &block.transactions[i];
        trans->pBlock = &block;

        UNHIDE_FIELD(CTransaction, "receipt");
        CReceipt receipt;
        getReceipt(receipt, trans->hash);
        trans->receipt = receipt; // deep copy
        if (needTrace && trans->gas == receipt.gasUsed)
        {
            SFString trace;
            queryRawTrace(trace, trans->hash);
            trans->isError = trace.ContainsI("error");
            nTraces++;
        }
    }

    nTrans  += block.transactions.getCount();
    nTraced += nTraces;
    if (verbose)
    {
        SFString fileName = getBinaryFilename1(toLong(numIn));
        SFString fmt;
        fmt += SFString("Block ") + cYellow  + "#" + asStringU(block.blockNumber)  + cOff;
        fmt += SFString(" (")     + cYellow  + padNum3T(block.transactions.getCount()) + "/" + asString(nTrans)  + cOff + " trans";
        fmt +=                      cYellow  + padNum3T(nTraces)                       + "/" + asString(nTraced) + cOff + " traced) written to ";
        fmt +=                      cMagenta + fileName.Substitute(blockFolder, "./")  + cOff + ".";
        fprintf(stderr, "%s\r", (const char*)fmt);
    }

    return true;
}

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, SFUint32 numIn)
{
    // Use queryBlock if you just want to read the block (any method)
    SFString save = qbGlobals::source;
    if (qbGlobals::source=="fastest")
        qbGlobals::source = (fileExists(getBinaryFilename1(numIn)) ? "binary" : "parity");
    bool ret = queryBlock(block, asStringU(numIn), true);
    qbGlobals::source = save;
    return ret;
}

//-------------------------------------------------------------------------
bool queryRawBlock(SFString& block, const SFString& numIn, bool needTrace, bool hashesOnly)
{
    block = callRPC("eth_getBlockByNumber", "["+quote(numIn)+","+(hashesOnly?"false":"true")+"]", true);
    return true;
}

//-------------------------------------------------------------------------
SFString hexxy(SFUint32 x)
{
    SFUintBN bn = x;
    return toLower(SFString(to_hex(bn).c_str()));
}

//-------------------------------------------------------------------------
bool queryRawReceipt(const SFHash& txHash, SFString& results)
{
    SFString data = "[\"[HASH]\"]";
    data.Replace("[HASH]", txHash);
    results = callRPC("eth_getTransactionReceipt", data, true);
    return true;
}

//-------------------------------------------------------------------------
bool queryRawLogs(SFUint32 fromBlock, SFUint32 toBlock, const SFAddress& addr, SFString& results)
{
    SFString data = "[{\"fromBlock\":\"0x[START]\",\"toBlock\":\"0x[STOP]\", \"address\": \"[ADDR]\"}]";
    data.Replace("[START]", hexxy(fromBlock));
    data.Replace("[STOP]",  hexxy(toBlock));
    data.Replace("[ADDR]",  fromAddress(addr));
    results = callRPC("eth_getLogs", data, true);
    return true;
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const SFString& hash)
{
    SFString h = hash.startsWith("0x") ? hash.substr(2) : hash;
    h = padLeft(h, 64, '0');
    getObjectViaRPC(trans, "eth_getTransactionByHash", "[\"0x" + h +"\"]");
    trans.finishParse();
    return true;
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const SFString& hash, SFUint32 transID)
{
    SFString h = hash.startsWith("0x") ? hash.substr(2) : hash;
    h = padLeft(h, 64, '0');
    SFUintBN t(transID);
    SFString ts = to_hex(t).c_str();
    getObjectViaRPC(trans, "eth_getTransactionByBlockHashAndIndex", "[\"0x" + h +"\",\"0x" + ts + "\"]");
    trans.finishParse();
    return true;
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, blknum_t blockNum, SFUint32 transID)
{
    SFUintBN h(blockNum);
    SFUintBN t(transID);
    SFString hs = to_hex(h).c_str();
    SFString ts = to_hex(t).c_str();
    getObjectViaRPC(trans, "eth_getTransactionByBlockNumberAndIndex", "[\"0x" + hs +"\",\"0x" + ts + "\"]");
    trans.finishParse();
    return true;
}

//-------------------------------------------------------------------------
bool getReceipt(CReceipt& receipt, const SFString& hash)
{
    SFString h = hash.startsWith("0x") ? hash.substr(2) : hash;
    h = padLeft(h, 64, '0');
    getObjectViaRPC(receipt, "eth_getTransactionReceipt", "[\"0x" + h + "\"]");
    return true;
}

//--------------------------------------------------------------
void getTraces(CTraceArray& traces, const SFHash& hash)
{
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
bool queryRawTrace(SFString& trace, const SFString& hashIn)
{
    SFString h = hashIn.startsWith("0x") ? hashIn.substr(2) : hashIn;
    h = padLeft(h, 64, '0');
    trace = "[" + callRPC("trace_transaction", "[\"0x" + h +"\"]", true) + "]";
    return true;
}

//-------------------------------------------------------------------------
SFString getVersionFromClient(void)
{
    return callRPC("web3_clientVersion", "[]", false);
}

//-------------------------------------------------------------------------
bool getCode(const SFString& addr, SFString& theCode)
{
    SFString a = addr.startsWith("0x") ? addr.substr(2) : addr;
    a = padLeft(a,40,'0');
    theCode = callRPC("eth_getCode", "[\"0x" + a +"\"]", false);
    return theCode.length()!=0;
}

//-------------------------------------------------------------------------
SFUintBN getBalance(const SFString& addr, blknum_t blockNum, bool isDemo)
{
    SFString a = addr.substr(2);
    a = padLeft(a,40,'0');
    SFString ret = callRPC("eth_getBalance", "[\"0x" + a +"\","+quote(asStringU(blockNum))+"]", false);
    return toWei(ret);
}

//-------------------------------------------------------------------------
bool getSha3(const SFString& hexIn, SFString& shaOut)
{
    shaOut = callRPC("web3_sha3", "[\"" + hexIn + "\"]", false);
    return true;
}

//-------------------------------------------------------------------------
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    ASSERT(userdata);
    SFString part;
    part.reserve(size*nmemb+1);
    char *s = (char*)(const char*)part;
    strncpy(s,ptr,size*nmemb);
    s[size*nmemb]='\0';
    (*(SFString*)userdata) += s;

    // At block 3804005, there was a hack wherein the byte code
    // 5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b repeated
    // thousands of time and doing nothing. If we don't handle this it
    // dominates the scanning for no reason
    if (strstr(s, "5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b") != NULL) {
        // This is the hack trace (there are many), so skip it
        cerr << "Curl response contains '5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b'. Aborting.\n";
        cerr.flush();
        earlyAbort = true;
        return 0;
    }
//    cerr << s << "\n";

    return size*nmemb;
}

//-----------------------------------------------------------------------
void writeToJson(const CBaseNode& node, const SFString& fileName)
{
    if (establishFolder(fileName))
    {
        std::ofstream out(fileName);
        SFString fmt = node.Format();
        fmt.ReplaceAll("\"to\": \"0x\"","\"to\": null");
        out << fmt << "\n";
        out.close();
    }
}

//-----------------------------------------------------------------------
void writeToBinary(const CBaseNode& node, const SFString& fileName)
{
    SFString created;
    if (establishFolder(fileName,created))
    {
        if (!created.empty())
            cerr << "mkdir(" << created << ")" << SFString(' ',20) << "                                                     \n";
        SFArchive archive(false, curVersion, true);
        if (archive.Lock(fileName, binaryWriteCreate, LOCK_CREATE))
        {
            ((CBlock *)&node)->Serialize(archive);
            archive.Close();
        }
    }
}

//-----------------------------------------------------------------------
bool readOneBlock_fromBinary(CBlock& block, const SFString& fileName)
{
    block = CBlock(); // reset
    SFArchive archive(true, curVersion, true);
    if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT))
    {
        block.Serialize(archive);
        archive.Close();
        return block.blockNumber;
    }
    return false;
}

//-----------------------------------------------------------------------
bool readOneBlock_fromJson(CBlock& block, const SFString& fileName)
{
    block = CBlock(); // reset
    SFString contents = asciiFileToString(fileName);
    if (contents.Contains("null"))
    {
        contents.ReplaceAll("null", "\"0x\"");
        stringToAsciiFile(fileName, contents);
    }
    if (!contents.endsWith('\n'))
    {
        stringToAsciiFile(fileName, contents+"\n");
    }
    char *p = cleanUpJson((char *)(const char*)contents);
    uint32_t nFields=0;
    block.parseJson(p,nFields);
    return nFields;
}

//----------------------------------------------------------------------------------
SFBloom readOneBloom(blknum_t bn) {
    SFBloom ret = 0;
    SFString fileName = getBinaryFilename1(bn).Substitute("/blocks/", "/blooms/");
    SFArchive archive(true, curVersion, true);
    if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
        archive >> ret;
        archive.Close();
    }
    return ret;
}

//-----------------------------------------------------------------------
void writeOneBloom(const SFString& fileName, const SFBloom& bloom) {
    SFString created;
    if (establishFolder(fileName,created)) {
        if (!created.empty())
            cerr << "mkdir(" << created << ")" << SFString(' ',20) << "                                                     \n";
        SFArchive archive(false, curVersion, true);
        if (archive.Lock(fileName, binaryWriteCreate, LOCK_CREATE)) {
            archive << bloom;
            archive.Close();
        }
    }
}

//-----------------------------------------------------------------------
bool verifyBlock(const CBlock& qBlock, SFString& result)
{
    SFString nStr; // the node's block
    queryRawBlock(nStr, asString(qBlock.blockNumber), true, expContext().hashesOnly);
    nStr = cleanUpJson((char*)(const char*)nStr);

    SFString qStr = qBlock.Format().Substitute("blockNumber","number"); // our block
    qStr = cleanUpJson((char*)(const char*)qStr);
//#define DEBUG_VERIFY
#if DEBUG_VERIFY
    cout << SFString('#',80) << "\nqb: ";
    cout << qStr << "\n";
    cout << SFString('-',80) << "\nnd: ";
    cout << nStr << "\n\n";
#endif

    // We extract the field values from the node for those fields that we store in our cache. We then
    // use the string generated from our data to substitute in the node generated string with empty.
    // If the strings are identical, the data is identical. We return the differences if the check fails
    SFString nnStr = "{";
    SFString fields = "gasLimit|gasUsed|hash|logsBloom|number|parentHash|timestamp|transactions|"; // we only store these data fields
    while (!fields.empty())
    {
        SFString field = nextTokenClear(fields,'|');
        long f1 = nStr.find(field);
        SFString tail = nStr.substr(f1);
        long f2 = tail.find(",");
        if (field=="transactions")
            f2 = tail.find("],")+1;
        SFString tField = tail.Left(f2==-1?tail.length():f2).Substitute("0x"+SFString('0',512),"0x0");
        nnStr += (tField + ",");
#if DEBUG_VERIFY
        cout << field << " = " << tField << "\n";
#endif
    }
    nnStr += "}";
    nnStr.Replace(",}","}");
#if DEBUG_VERIFY
    cout << nnStr << "\n\n";
#endif
    nnStr.Replace(qStr,"");
    if (!nnStr.empty())
    {
        result = ("  qb: " + qStr + "\n  nd: " + nnStr + "\n");
        return false;
    }
    return true;
}

static SFString storagePath;
//-------------------------------------------------------------------------
void setStorageRoot(const SFString& path)
{
    storagePath = path;
    if (!storagePath.endsWith('/'))
        storagePath += "/";
    establishFolder(storagePath);
}

//-------------------------------------------------------------------------
SFString getStorageRoot(void) {
    return storagePath;
}

//-------------------------------------------------------------------------
static SFString getFilename_local(SFUint32 numIn, bool asPath, bool asJson)
{
    if (storagePath.empty())
    {
        cerr << "You must set the storage path with setStorageRoot(path). Quitting...\n";
        exit(0);
    }

    char ret[512];
    bzero(ret,sizeof(ret));

    SFString num = padLeft(asString(numIn),9,'0');
    SFString fmt = (asPath ? "%s/%s/%s/" : "%s/%s/%s/%s");
    SFString fn  = (asPath ? "" : num + (asJson ? ".json" : ".bin"));

    sprintf(ret, (const char*)(storagePath+fmt), (const char*)num.Left(2), (const char*)num.substr(2,2), (const char*)num.substr(4,2), (const char*)fn);
    return ret;
}

//-------------------------------------------------------------------------
SFString getJsonFilename1(SFUint32 num)
{
    return getFilename_local(num, false, true);
}

//-------------------------------------------------------------------------
SFString getBinaryFilename1(SFUint32 num)
{
    SFString ret = getFilename_local(num, false, false);
    ret.Replace("/00/",  "/blocks/00/"); // can't use Substitute because it will change them all
    return ret;
}

//-------------------------------------------------------------------------
SFString getJsonPath1(SFUint32 num)
{
    return getFilename_local(num, true, true);
}

//-------------------------------------------------------------------------
SFString getBinaryPath1(SFUint32 num)
{
    SFString ret = getFilename_local(num, true, false);
    ret.Replace("/00/",  "/blocks/00/"); // can't use Substitute because it will change them all
    return ret;
}

//-------------------------------------------------------------------------
bool forEveryBlockOnDisc(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count, SFUint32 skip)
{
    if (!func)
        return false;

    // Read every block from number start to start+count
    for (SFUint32 i = start ; i < start + count ; i = i + skip)
    {
        CBlock block;
        getBlock(block,i);
        if (!(*func)(block, data))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool forEveryEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count, SFUint32 skip)
{
    if (!func)
        return false;

    SFString save = qbGlobals::source;
    qbGlobals::source = "parity"; // the empty blocks are not on disk, so we have to ask parity. Don't write them, though

    CSharedResource fullBlocks;
    if (!fullBlocks.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT))
    {
        cerr << "forEveryNonEmptyBlockOnDisc failed: " << fullBlocks.LockFailure() << "\n";
        return false;
    }
    ASSERT(fullBlocks.isOpen());

    SFUint32 nItems = fileSize(fullBlockIndex) / sizeof(SFUint32) + 1;  // we need an extra one for item '0'
    SFUint32 *contents = new SFUint32[nItems+2];  // extra space
    if (contents)
    {
        // read the entire full block index
        fullBlocks.Read(&contents[0], sizeof(SFUint32), nItems-1);  // one less since we asked for an extra one
        fullBlocks.Release();  // release it since we don't need it any longer

        contents[0] = 0;  // the starting point (needed because we are build the empty list from the non-empty list
        SFUint32 cnt = start;
        for (SFUint32 i = 1 ; i < nItems ; i = i + skip) // first one (at index '0') is assumed to be the '0' block
        {
            while (cnt<contents[i])
            {
                CBlock block;
                // Both 'queryBlock' and 'getBlock' return false if there are no
                // transactions, so we ignore the return value
                getBlock(block,cnt);
                if (!(*func)(block, data))
                {
                    qbGlobals::source = save;
                    delete [] contents;
                    return false;
                }
                cnt++; // go to the next one
            }
            cnt++; // contents[i] has transactions, so skip it
        }
        delete [] contents;
    }
    qbGlobals::source = save;
    return true;
}

//-------------------------------------------------------------------------
template<class T>
inline bool inRange(T val, T mn, T mx) {
    return (val >= mn && val <= mx);
}

//-------------------------------------------------------------------------
bool forEveryNonEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count, SFUint32 skip)
{
    // Read the non-empty block index file and spit it out only non-empty blocks
    if (!func)
        return false;

    CSharedResource fullBlocks;
    if (!fullBlocks.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT))
    {
        cerr << "forEveryNonEmptyBlockOnDisc failed: " << fullBlocks.LockFailure() << "\n";
        return false;
    }
    ASSERT(fullBlocks.isOpen());

    SFUint32 nItems = fileSize(fullBlockIndex) / sizeof(SFUint32);
    SFUint32 *contents = new SFUint32[nItems];
    if (contents)
    {
        // read the entire full block index
        fullBlocks.Read(contents, sizeof(SFUint32), nItems);
        fullBlocks.Release();  // release it since we don't need it any longer

        for (SFUint32 i = 0 ; i < nItems ; i = i + skip)
        {
            // TODO: This should be a binary search not a scan. This is why it appears to wait
//cout << "inRange(" << contents[i] << ", " << start << ", " << start+count-1 << "): ";
            if (inRange((SFUint32)contents[i], start, start+count-1))
            {
//cout << "true\n";
//              if (verbose) { cerr << "Getting block " << contents[i] << "\n"; cerr.flush(); }
                CBlock block;
                if (getBlock(block,contents[i]))
                {
                    bool ret = (*func)(block, data);
                    if (!ret)
                    {
                        // Cleanup and return if user tells us to
                        delete [] contents;
                        return false;
                    }
                }
            } else {
//cout << "false\n";
            }
//cout.flush();
        }
        delete [] contents;
    }
    return true;
}

//-------------------------------------------------------------------------
SFUint32 getLatestBlockFromClient(void)
{
    CBlock block;
    getObjectViaRPC(block, "eth_getBlockByNumber", "[\"latest\",true]");
    return block.blockNumber;
}

//--------------------------------------------------------------------------
SFUint32 getLatestBloomFromCache(void) {
    return toLongU(asciiFileToString(bloomFolder + "lastBloom.txt"));
}

//--------------------------------------------------------------------------
SFUint32 getLatestBlockFromCache(CSharedResource *res) {

    SFUint32 ret = 0;

    CSharedResource fullBlocks; // Don't move--need the scope
    CSharedResource *pRes = res;
    if (!pRes)
    {
        // We're reading so okay not to wait
        if (!fullBlocks.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT))
        {
            cerr << "getLatestBlockFromCache failed: " << fullBlocks.LockFailure() << "\n";
            return ret;
        }
        pRes = &fullBlocks;
    }
    ASSERT(pRes->isOpen());

    pRes->Seek(-1*sizeof(SFUint32),SEEK_END);
    pRes->Read(ret);
    if (pRes != res)
        pRes->Release();
    return ret;
}

//--------------------------------------------------------------------------
bool getLatestBlocks(SFUint32& cache, SFUint32& client, CSharedResource *res)
{
    client = getLatestBlockFromClient();
    cache  = getLatestBlockFromCache(res);
    return true;
}

//--------------------------------------------------------------------------
inline SFString TIMER_IN(double& startTime) {
    CStringExportContext ctx;
    ctx << (qbNow()-startTime) << ": ";
    startTime = qbNow();
    return ctx.str;
}
inline SFString TIMER_TICK(double startTime) {
    CStringExportContext ctx;
    ctx << "in " << cGreen << (qbNow()-startTime) << cOff << " seconds.";
    return ctx.str;
}
#define TIMER()  TIMER_IN(startTime)
#define TIMER_T() TIMER_TICK(startTime)

//--------------------------------------------------------------------------
class CInMemoryCache
{
public:
    CInMemoryCache(void) {
        blocks    = NULL;
        // blocksOnDisc;
        blockFile = miniBlockCache;

        trans     = NULL;
        // transOnDisc
        transFile = miniTransCache;

        nBlocks   = fileSize(blockFile) / sizeof(CMiniBlock);
        nTrans    = fileSize(transFile) / sizeof(CMiniTrans);

        loaded = false;
    }

    ~CInMemoryCache(void) {
        Clear();
    }

    void Clear(void) {
        if ( blocks   ) delete [] blocks;
        blocks = NULL;
        if ( trans    ) delete [] trans;
        trans = NULL;
        blocksOnDisc.Release();
        transOnDisc.Release();
        loaded = false;
    }

    bool Load(SFUint32 _start, SFUint32 _count)
    {
        start = min(_start,        getLatestBlockFromCache());
        count = min(_start+_count, getLatestBlockFromCache()) - _start;
        if (loaded)
            return true;
        loaded = true; // only come through here once, even if we fail to load

        double startTime = qbNow();
        blocks = new CMiniBlock[nBlocks];
        if (!blocks)
        {
            cerr << "Could not allocate memory for the blocks (size needed: " << nBlocks << ").\n";
            return false;
        }
        bzero(blocks, sizeof(CMiniBlock)*(nBlocks));
        if (verbose)
            cerr << TIMER() << "Allocated room for " << nBlocks << " miniBlocks.\n";

        // Next, we try to open the mini-block database
        if (!blocksOnDisc.Lock(blockFile, binaryReadOnly, LOCK_WAIT))
        {
            cerr << "Could not open the mini-block database: " << blockFile << ".\n";
            return false;
        }
        blocksOnDisc.Seek(0, SEEK_SET);

        // Read the entire mini-block database into memory in one chunk
        size_t nRead = blocksOnDisc.Read(blocks, nBlocks, sizeof(CMiniBlock));
        blocksOnDisc.Release();  // We're done with it
        if (nRead != nBlocks)
        {
            cerr << "Error encountered reading mini-blocks database.\n Quitting...";
            return false;
        }
        if (verbose)
            cerr << TIMER() << "Read " << nRead << " miniBlocks into memory.\n";

        // See if we can allocation enough space for the mini-transaction database
        SFUint32 fs = fileSize(transFile);
        SFUint32 ms = sizeof(CMiniTrans);
        SFUint32 nTrans   = fs / ms;
        trans = new CMiniTrans[nTrans];
        if (!trans)
        {
            cerr << "Could not allocate memory for the transactions (size needed: " << nTrans << ").\n";
            return false;
        }
        bzero(trans, sizeof(CMiniTrans)*(nTrans));
        if (verbose)
            cerr << TIMER() << "Allocated room for " << nTrans << " transactions.\n";

        // Next, we try to open the mini-transaction database
        if (!transOnDisc.Lock(transFile, binaryReadOnly, LOCK_WAIT))
        {
            cerr << "Could not open the mini-transaction database: " << transFile << ".\n";
            return false;
        }

        // Read the entire mini-transaction database into memory in one chunk
        // TODO: What is the correct value for this?
#define READ_SIZE 204800
        nRead = 0;
        while (nRead < nTrans)
        {
            nRead += transOnDisc.Read(&trans[nRead], READ_SIZE, sizeof(CMiniTrans));
            if (verbose)
                progressBar(nRead,nTrans,TIMER_T());
        }
        transOnDisc.Release();
        cerr << "\n" << TIMER();
        return true;
    }
    SFUint32 firstBlock(void) { return 0; }
    SFUint32 lastBlock (void) { return nBlocks; }

public:
    bool            loaded;
    CMiniBlock     *blocks;
    CSharedResource blocksOnDisc;
    SFString        blockFile;

    CMiniTrans     *trans;
    CSharedResource transOnDisc;
    SFString        transFile;

private:
    SFUint32    nBlocks;
    SFUint32    nTrans;
    SFUint32    start;
    SFUint32    count;
};

static CInMemoryCache theCache;
//--------------------------------------------------------------------------
void clearInMemoryCache(void)
{
    theCache.Clear();
}
//--------------------------------------------------------------------------
bool forEveryMiniBlockInMemory(MINIBLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
    if (!theCache.Load(start,count))
        return false;
    SFUint32 first = theCache.firstBlock();
    SFUint32 last = theCache.lastBlock();

    bool done=false;
    for (SFUint32 i=first;i<last&&!done;i++)
    {
        if (inRange((SFUint32)theCache.blocks[i].blockNumber, start, start+count-1))
        {
            if (!(*func)(theCache.blocks[i], &theCache.trans[0], data))
                return false;

        } else if (theCache.blocks[i].blockNumber >= start+count)
        {
            done=true;

        } else
        {
            // do nothing
        }
    }

    return true;
}

/*
 SFAddress from;
 SFAddress to;
 CReceipt
    SFAddress contractAddress;
    CLogEntryArray logs;
        SFAddress address;
        SFString data;
        SFUint32 logIndex;
        SFBigUintArray topics;
        CTrace
            SFStringArray traceAddress;
            CTraceAction action;
                SFAddress from;
                SFAddress to;
 */

//--------------------------------------------------------------------------
bool forEveryFullBlockInMemory(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
    if (qbGlobals::source != "mem")
        return false;
    if (!theCache.Load(start,count))
        return false;

    SFUint32 first = theCache.firstBlock();
    SFUint32 last = theCache.lastBlock();

    bool done=false;
    for (SFUint32 i=first;i<last&&!done;i++)
    {
        if (inRange((SFUint32)theCache.blocks[i].blockNumber, start, start+count-1))
        {
            CBlock block;
            theCache.blocks[i].toBlock(block);
            SFUint32 gasUsed=0;
            for (SFUint32 tr=theCache.blocks[i].firstTrans;tr<theCache.blocks[i].firstTrans+theCache.blocks[i].nTrans;tr++)
            {
                CTransaction tt;
                theCache.trans[tr].toTrans(tt);
                gasUsed += tt.receipt.gasUsed;
                block.transactions[block.transactions.getCount()] = tt;
            }
            block.gasUsed = gasUsed;
            if (!(*func)(block, data))
                return false;

        } else if (theCache.blocks[i].blockNumber >= start+count)
        {
            done=true;

        } else
        {
            // do nothing
        }
    }

    return true;
}

//-------------------------------------------------------------------------
bool forEveryTransaction(TRANSVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
    if (!func)
        return false;
    return true;
}

//-------------------------------------------------------------------------
bool forEveryTransactionTo(TRANSVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
    if (!func)
        return false;
    return true;
}

//-------------------------------------------------------------------------
bool forEveryTransactionFrom(TRANSVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
    if (!func)
        return false;
    return true;
}

//-------------------------------------------------------------------------
bool forEveryBloomFile(FILEVISITOR func, void *data, SFUint32 start, SFUint32 count, SFUint32 skip) {
    if (start == 0 || count == (SFUint32)-1) { // visit everything since we're given the default
        forEveryFileInFolder(bloomFolder, func, data);
        return true;
    }

    // visit only the folder the user tells us to visit
    blknum_t st = (start / 1000) * 1000;
    blknum_t ed = ((start+count+1000) / 1000) * 1000;
    for (blknum_t b = st ; b < ed ; b += 1000) {
        SFString path = getBinaryPath1(b).Substitute("/blocks/","/blooms/");
        forEveryFileInFolder(path, func, data);
    }
    return true;
}
}  // namespace qblocks
