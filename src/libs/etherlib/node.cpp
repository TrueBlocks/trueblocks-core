/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "node.h"

namespace qblocks {

//-------------------------------------------------------------------------
CURL *getCurl(bool cleanup=false)
{
    static CURL *curl = NULL;
    static struct curl_slist *headers = NULL;
    if (!curl && !cleanup)
    {
        curl = curl_easy_init();
        if (!curl)
        {
            fprintf(stderr, "Curl failed to initialize. Quitting...\n");
            exit(0);
        }

        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (getSource() == "infura")
        {
            // we have to use Infura
#if 0
            headers = curl_slist_append(headers, "Infura-Ethereum-Preferred-Client: parity");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL,        "https:/""/mainnet.infura.io/WkavvX9Hk5tvp34LhN7W");
#endif
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL,        "https://pmainnet.infura.io/");

        } else
        {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL,        "http://localhost:8545");
        }

    } else if (cleanup)
    {
        if (headers)
            curl_slist_free_all(headers);
        headers = NULL;
        if (curl)
            curl_easy_cleanup(curl);
        curl = NULL;
        return NULL;
    }

    return curl;
}

//-------------------------------------------------------------------------
extern size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
static bool earlyAbort=false;
double startTime = qbNow();
bool is_error=false;
bool is_tracing=false;
//-------------------------------------------------------------------------
void etherlib_init(const SFString& sourceIn)
{
    setStorageRoot(blockCachePath(""));

    // In case we create any lock files, so
    // they get cleaned up
    registerQuitHandler(defaultQuitHandler);

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
    CAbi::registerClass();
    CFunction::registerClass();
    CParameter::registerClass();
    CAccount::registerClass();
    CRPCResult::registerClass();
    CNameValue::registerClass();
    CAccountName::registerClass();

    setSource(sourceIn);
    // if curl has already been initialized, we want to clear it out
    getCurl(true);
    // initialize curl
    getCurl();

    establishFolder(configPath(""));
    startTime = qbNow();
}

//-------------------------------------------------------------------------
void etherlib_cleanup(void)
{
    // cleanup curl
    getCurl(true);
}

static uint32_t theID = 1;
//-------------------------------------------------------------------------
SFString getCurlID(void) {
    return asString(isTestMode() ? 1 : theID++);
}

//-------------------------------------------------------------------------
class CCurlData {
public:
    SFString url;
    SFString result;
    CCurlData(const SFString& method, const SFString& params) {
        url += "{";
        url +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
        url +=  quote("method")  + ":"  + quote(method) + ",";
        url +=  quote("params")  + ":"  + params + ",";
        url +=  quote("id")      + ":"  + quote(getCurlID());
        url += "}";
    }
};

//-------------------------------------------------------------------------
// Use 'curl' to make an arbitrary rpc call
//-------------------------------------------------------------------------
SFString callRPC(const SFString& method, const SFString& params, bool raw)
{
    CCurlData curlData(method, params);

//#define DEBUG_RPC
#ifdef DEBUG_RPC
    cerr << "\n" << SFString('-',80) << "\n";
    cerr << thePost << "\n";
    cerr << SFString('-',80) << "\n";
    cerr.flush();
#endif

    curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDS,    (const char*)curlData.url);
    curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDSIZE, curlData.url.length());

    curl_easy_setopt(getCurl(), CURLOPT_WRITEDATA,     &curlData);
    curl_easy_setopt(getCurl(), CURLOPT_WRITEFUNCTION, write_callback);

    earlyAbort = false;
    CURLcode res = curl_easy_perform(getCurl());
    if (res != CURLE_OK && !earlyAbort)
    {
        SFString currentSource = getSource();
        SFString fallBack = getenv("FALLBACK");
        if (!fallBack.empty() && currentSource != fallBack) {
            if (fallBack != "infura") {
                cerr << cYellow;
                cerr << "\n";
                cerr << "\tWarning: " << cOff << "Only the 'infura' fallback is supported.\n";
                cerr << "\tIt is impossible for QuickBlocks to proceed. Quitting...\n";
                cerr << "\n";
                exit(0);
            }

            if (fallBack == "infura" && method.startsWith("trace_")) {
                cerr << cYellow;
                cerr << "\n";
                cerr << "\tWarning: " << cOff << "A trace request was made to the fallback\n";
                cerr << "\tnode. " << toProper(fallBack) << " does not support tracing. It ";
                cerr << "is impossible\n\tfor QuickBlocks to proceed. Quitting...\n";
                cerr << "\n";
                exit(0);
            }
            theID--;
            setSource(fallBack);
            // reset curl
            getCurl(true); getCurl();
            // since we failed, we leave the new source, otherwise we would have to save
            // the results and reset it here.
            return callRPC(method, params, raw);
        }
        cerr << cYellow;
        cerr << "\n";
        cerr << "\tWarning: " << cOff << "The request to the Ethereum node ";
        cerr << "resulted in\n\tfollowing error message: ";
        cerr << bTeal << curl_easy_strerror(res) << cOff << ".\n";
        cerr << "\tIt is impossible for QuickBlocks to proceed. Quitting...\n";
        cerr << "\n";
        exit(0);
    }

    if (!is_tracing && curlData.result.empty()) {
        cerr << cYellow;
        cerr << "\n";
        cerr << "\tWarning:" << cOff << "The Ethereum node  resulted in an empty\n";
        cerr << "\tresponse. It is impossible forQuickBlocks to proceed. Quitting...\n";
        cerr << "\n";
        exit(0);
    }

#ifdef DEBUG_RPC
//    cout << "\n" << SFString('-',80) << "\n";
//    cout << thePost << "\n";
    cout << SFString('=',60) << "\n";
    cout << "received: " << curlData.result << "\n";
    cout.flush();
#endif

    if (raw)
        return curlData.result;
    CRPCResult generic;
    char *p = cleanUpJson((char*)(const char*)curlData.result);
    generic.parseJson(p);
    return generic.result;
}

//-------------------------------------------------------------------------
extern size_t nullCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size*nmemb;
}

//-------------------------------------------------------------------------
bool isNodeRunning(void) {
    CCurlData curlData("web3_clientVersion", "[]");
    curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDS,    (const char*)curlData.url);
    curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDSIZE, curlData.url.length());
    curl_easy_setopt(getCurl(), CURLOPT_WRITEDATA,     &curlData);
    curl_easy_setopt(getCurl(), CURLOPT_WRITEFUNCTION, nullCallback);
    return (curl_easy_perform(getCurl()) == CURLE_OK);
}

//-------------------------------------------------------------------------
bool getObjectViaRPC(CBaseNode &node, const SFString& method, const SFString& params) {
    SFString ret = callRPC(method, params, false);
    node.parseJson((char *)(const char*)ret);
    return true;
}

// TODO: remove global data
static bool no_tracing=false;
void setNoTracing(bool val) { no_tracing = val; }
//-------------------------------------------------------------------------
bool queryBlock(CBlock& block, const SFString& numIn, bool needTrace, bool byHash) {
    uint32_t unused = 0;
    return queryBlock(block, numIn, needTrace, byHash, unused);
}

#define byzantiumBlock 4370000
//-------------------------------------------------------------------------
bool queryBlock(CBlock& block, const SFString& numIn, bool needTrace, bool byHash, uint32_t& nTraces) {

    if (numIn=="latest")
        return queryBlock(block, asStringU(getLatestBlockFromClient()), needTrace, false);

    SFUint32 num = toLongU(numIn);
    if ((getSource().Contains("binary") || getSource().Contains("nonemp")) && fileSize(getBinaryFilename1(num))>0) {
        UNHIDE_FIELD(CTransaction, "receipt");
        return readOneBlock_fromBinary(block, getBinaryFilename1(num));

    } else if (getSource().Contains("Only")) {
        return false;

    }

    if (getSource() == "json" && fileSize(getJsonFilename1(num))>0)
    {
        UNHIDE_FIELD(CTransaction, "receipt");
        return readOneBlock_fromJson(block, getJsonFilename1(num));

    }

    HIDE_FIELD(CTransaction, "receipt");
    getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(asStringU(num))+",true]");

    // If there are no transactions, we're done
    if (!block.transactions.getCount() || no_tracing)
    {
        // We only write binary if there are transactions
        //writeToBinary(block, getBinaryFilename1(num));
        //writeToJson(block, getJsonFilename1(num)); //We've stopped writing JSON for now because of disc space
        return false;
    }

    // We have the transactions, but we also want the receipts
    nTraces=0;
    for (uint32_t i=0;i<block.transactions.getCount();i++)
    {
        CTransaction *trans = &block.transactions[i];
        trans->pBlock = &block;

        UNHIDE_FIELD(CTransaction, "receipt");
        CReceipt receipt;
        getReceipt(receipt, trans->hash);
        trans->receipt = receipt; // deep copy
        if (num >= byzantiumBlock) {
            trans->isError = receipt.isError;

        } else if (needTrace && trans->gas == receipt.gasUsed) {
            SFString trace;
            is_error = false;
            is_tracing = true;
            queryRawTrace(trace, trans->hash);
            is_tracing = false;
            trans->isError = is_error;
            nTraces++;
        }
    }

    return true;
}

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, SFUint32 numIn)
{
    SFString save = getSource();
    if (getSource() == "fastest")
        setSource(fileExists(getBinaryFilename1(numIn)) ? "binary" : "parity");
    bool ret = queryBlock(block, asStringU(numIn), true, false);
    setSource(save);
    return ret;
}

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, const SFHash& hash) {
    // TODO: Note--this does not work because queryBlock takes blockNum in second param
    bool ret = queryBlock(block, hash, true, true);
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
bool queryRawReceipt(SFString& results, const SFHash& txHash)
{
    SFString data = "[\"[HASH]\"]";
    data.Replace("[HASH]", txHash);
    results = callRPC("eth_getTransactionReceipt", data, true);
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
bool queryRawLogs(SFString& results, const SFAddress& addr, SFUint32 fromBlock, SFUint32 toBlock)
{
    SFString data = "[{\"fromBlock\":\"0x[START]\",\"toBlock\":\"0x[STOP]\", \"address\": \"[ADDR]\"}]";
    data.Replace("[START]", hexxy(fromBlock));
    data.Replace("[STOP]",  hexxy(toBlock));
    data.Replace("[ADDR]",  fromAddress(addr));
    results = callRPC("eth_getLogs", data, true);
    return true;
}

//-------------------------------------------------------------------------
bool getAccounts(SFAddressArray& addrs) {
    SFString results = callRPC("eth_accounts", "[]", false);
    while (!results.empty())
        addrs[addrs.getCount()] = nextTokenClear(results,',');
    return true;
}

    //-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const SFString& hashIn)
{
    getObjectViaRPC(trans, "eth_getTransactionByHash", "[\"" + fixHash(hashIn) +"\"]");
    trans.finishParse();
    return true;
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const SFString& hashIn, SFUint32 transID)
{
    SFUintBN t(transID);
    SFString ts = to_hex(t).c_str();
    getObjectViaRPC(trans, "eth_getTransactionByBlockHashAndIndex", "[\"" + fixHash(hashIn) +"\",\"0x" + ts + "\"]");
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
bool getReceipt(CReceipt& receipt, const SFString& hashIn)
{
    getObjectViaRPC(receipt, "eth_getTransactionReceipt", "[\"" + fixHash(hashIn) + "\"]");
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
    trace = "[" + callRPC("trace_transaction", "[\"" + fixHash(hashIn) +"\"]", true) + "]";
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
SFUintBN getTokenBalance(const SFAddress& token, const SFAddress& holder, blknum_t blockNum) {

    ASSERT(isAddress(token));
    ASSERT(isAddress(holder));

    SFString t = "0x" + padLeft(token.substr(2), 40, '0');  // address to send the command to
    SFString h =        padLeft(holder.substr(2), 64, '0'); // encoded data for the transaction

    SFString cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"0x70a08231[HOLDER]\"}, \"[BLOCK]\"]";
    cmd.Replace("[TOKEN]",  t);
    cmd.Replace("[HOLDER]", h);
    cmd.Replace("[BLOCK]",  asStringU(blockNum));

    return toWei(callRPC("eth_call", cmd, false));
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
    if (is_tracing) {
        // Curl does not close the string, so we have to
        ptr[size*nmemb-1] = '\0';
        if (strstr(ptr,"erro")!=NULL) {
            is_error = true;
            earlyAbort = true;
            return 0;
        }

    } else {
        SFString part;
        part.reserve(size*nmemb+1);
        char *s = (char*)(const char*)part;
        strncpy(s,ptr,size*nmemb);
        s[size*nmemb]='\0';
        ASSERT(userdata);
        CCurlData *data = (CCurlData*)userdata;
        data->result += s;
        // Starting around block 3804005, there was a hack wherein the byte code 5b5b5b5b5b5b5b5b5b5b5b5b
        // repeated thousands of times, doing nothing. If we don't handle this, it dominates the scanning
        // for no reason
        if (strstr(s, "5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b") != NULL) {
            // This is the hack trace (there are many), so skip it
            cerr << "Curl response contains '5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b'. Aborting.\n";
            cerr.flush();
            earlyAbort = true;
            return 0;
        }
    }

    return size*nmemb;
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
void writeToBinary(const CBaseNode& node, const SFString& fileName) {
    SFString created;
    if (establishFolder(fileName,created)) {
        if (!created.empty() && !isTestMode())
            cerr << "mkdir(" << created << ")" << SFString(' ',20) << "                                                     \n";
        SFArchive archive(WRITING_ARCHIVE);
        if (archive.Lock(fileName, binaryWriteCreate, LOCK_CREATE)) {
            ((CBlock *)&node)->Serialize(archive);
            archive.Close();
        }
    }
}

//-----------------------------------------------------------------------
bool readFromJson(CBaseNode& node, const SFString& fileName) {
    return false;
};

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

//-----------------------------------------------------------------------
bool readOneBlock_fromBinary(CBlock& block, const SFString& fileName)
{
    block = CBlock(); // reset
    SFArchive archive(READING_ARCHIVE);
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
    if (!fileExists(fileName)) {
        cerr << "File not found " << fileName << "\n";
        return false;
    }
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
    SFArchive archive(READING_ARCHIVE);
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
        if (!created.empty() && !isTestMode())
            cerr << "mkdir(" << created << ")" << SFString(' ',20) << "                                                     \n";
        SFArchive archive(WRITING_ARCHIVE);
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
    queryRawBlock(nStr, asStringU(qBlock.blockNumber), true, expContext().hashesOnly);
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
        size_t f1 = nStr.find(field);
        SFString tail = nStr.substr(f1);
        size_t f2 = tail.find(",");
        if (field=="transactions")
            f2 = tail.find("],") + 1;
        SFString tField = tail.Left(f2 == NOPOS ? tail.length() : f2).Substitute("0x"+SFString('0',512),"0x0");
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

    SFString num = padLeft(asStringU(numIn),9,'0');
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

    SFString save = getSource();
    setSource("parity"); // the empty blocks are not on disk, so we have to ask parity. Don't write them, though

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
                    setSource(save);
                    delete [] contents;
                    return false;
                }
                cnt++; // go to the next one
            }
            cnt++; // contents[i] has transactions, so skip it
        }
        delete [] contents;
    }
    setSource(save);
    return true;
}

//-------------------------------------------------------------------------
bool forEveryBlock(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count, SFUint32 skip) {
    // Here we simply scan the numbers and either read from disc or query the node
    if (!func)
        return false;

    SFString save = getSource();
    setSource("fastest");

    for (SFUint32 i = start ; i < start + count - 1 ; i = i + skip) {
        SFString fileName = getBinaryFilename1(i);
        CBlock block;
        if (fileExists(fileName))
            readOneBlock_fromBinary(block, fileName);
        else
            getBlock(block,i);

        bool ret = (*func)(block, data);
        if (!ret) {
            // Cleanup and return if user tells us to
            setSource(save);
            return false;
        }
    }
    setSource(save);
    return true;
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
            if (inRange((SFUint32)contents[i], start, start+count-1))
            {
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
            }
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

    pRes->Seek( (-1 * (long)sizeof(SFUint32)), SEEK_END);
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

//-------------------------------------------------------------------------
bool forEveryBlock(BLOCKVISITFUNC func, void *data, const SFString& block_list) {
    return true;
}

//-------------------------------------------------------------------------
bool forEveryTransaction(TRANSVISITFUNC func, void *data, const SFString& trans_list)
{
    if (!func)
        return false;

    // trans_list is a list of tx_hash, blk_hash.tx_id, or blk_num.tx_id, or any combination
    SFString list = trans_list;
    while (!list.empty()) {
        SFString item = nextTokenClear(list, '|');
        bool hasDot = item.Contains(".");

        SFString hash = nextTokenClear(item, '.');
        SFUint32 txID = toLongU(item);

        CTransaction trans;
        if (hash.startsWith("0x")) {
            if (hasDot) {
                // We are not fully formed, we have to ask the node for the receipt
                getTransaction(trans, hash, txID);  // blockHash.txID
            } else {
                // We are not fully formed, we have to ask the node for the receipt
                getTransaction(trans, hash);  // transHash
            }
        } else {
            getTransaction(trans, (uint32_t)toLongU(hash), txID);  // blockHash.txID
        }
        CBlock block;
        trans.pBlock = &block;
        getBlock(block, trans.blockNumber);
        getReceipt(trans.receipt, trans.getValueByName("hash"));
        trans.receipt.pTrans = &trans;
        trans.finishParse();
        if (!(*func)(trans, data))
            return false;
    }
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
    ASSERT(blockCache.endsWith("/"));
    return blockCache + _part;
}

}  // namespace qblocks
