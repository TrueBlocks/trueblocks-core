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
#include <thread>  // NOLINT
#include "node.h"
#include "node_curl.h"

namespace qblocks {

//-------------------------------------------------------------------------
extern size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
extern size_t errorCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
extern size_t nullCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

//-------------------------------------------------------------------------
CCurlContext::CCurlContext(void) {
    baseURL = getRpcProvider();
    debugging = getGlobalConfig("")->getConfigBool("dev", "debug_curl", false);
    callBackFunc = writeCallback;
    curlNoteFunc = NULL;
    theID = 1;
    nodeRequired = true;
    earlyAbort = false;
    is_error = false;
    postData = "";
    result = "";
    curlHandle = NULL;
    headerPtr = NULL;
    //      source       = "binary";
    //      methodMap    = "";
    methodCnt = 0;
    //      curlErrors;
    reportErrors = false;
}

//-------------------------------------------------------------------------
string_q CCurlContext::getCurlID(void) {
    return uint_2_Str(isTestMode() ? 1 : theID++);
}

//-------------------------------------------------------------------------
#define DEBUG_RPC
#ifdef DEBUG_RPC

//-------------------------------------------------------------------------
string_q showResult(const string_q& result, const string_q& msg) {
    if (result.empty())
        return "";
    string_q res = result;
    cleanString(res, false);
    replaceAny(res, "{}[]", "");
    replaceAll(res, ",", "\n");
    replaceAll(res, "\"result\":", "");
    ostringstream os;
    os << string_q(50, '-') << endl;
    os << msg << endl;
    CStringArray lines;
    explode(lines, res, '\n');
    sort(lines.begin(), lines.end());
    os << cGreen << "result:\n" << cOff;
    for (auto line : lines) {
        replaceAny(line, "\n", "");
        if (!line.empty() && !contains(line, "\"id\":") && !contains(line, "\"chainId\":") &&
            !contains(line, "\"condition\":") && !contains(line, "\"creates\":") && !contains(line, "\"publicKey\":") &&
            !contains(line, "\"jsonrpc\":") && !contains(line, "\"raw\":") && !contains(line, "\"standardV\":") &&
            !contains(line, "\"transactionLogIndex\":") && !contains(line, "\"type\":") &&
            !contains(line, "\"author\":") && !contains(line, "\"sealFields\":"))
            os << "--[" << line << "]--" << endl;
    }
    return os.str();
}
string_q showCommand(const string_q& method, const string_q& params, const string_q& url) {
    ostringstream os;
    os << "curl -s ";
    os << "--data '{\"method\":\"" << method;
    os << "\",\"params\":" << params << ",\"id\":\"1\",\"jsonrpc\":\"2.0\"}' ";
    os << "-H \"Content-Type: application/json\" ";
    os << "-X POST " << url;
    return os.str();
}
#define PRINT(msg)                                                                                                     \
    if (debugging) {                                                                                                   \
        cout.flush();                                                                                                  \
        cerr.flush();                                                                                                  \
        cerr << showResult(result, msg);                                                                               \
        cerr << cGreen << "earlyAbort: " << cOff << earlyAbort << endl;                                                \
        cerr << cGreen << "curlID: " << cOff << getCurlID() << endl;                                                   \
    }

#define PRINTQ(msg)                                                                                                    \
    if (data->debugging) {                                                                                             \
        cout.flush();                                                                                                  \
        cerr.flush();                                                                                                  \
        cerr << string_q(50, '-') << endl;                                                                             \
        cerr << "." << msg << endl;                                                                                    \
    }

#define PRINTL(msg)                                                                                                    \
    if (debugging) {                                                                                                   \
        cout.flush();                                                                                                  \
        cerr.flush();                                                                                                  \
        cerr << string_q(50, '-') << endl;                                                                             \
        PRINT(msg);                                                                                                    \
    }
#else  // DEBUG_RPC
#define showCommand(a, b, c) string_q("")
#define PRINTQ(msg)
#define PRINT(msg)
#define PRINTL(msg)
#endif  // DEBUG_RPC

//-------------------------------------------------------------------------
void CCurlContext::setPostData(const string_q& method, const string_q& params) {
    clear();
    postData = "{";
    postData += quote("jsonrpc") + ":" + quote("2.0") + ",";
    postData += quote("method") + ":" + quote(method) + ",";
    postData += quote("params") + ":" + params + ",";
    postData += quote("id") + ":" + quote(getCurlContext()->getCurlID());
    postData += "}";

    PRINT("postData: " + postData);

    curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_POSTFIELDSIZE, postData.length());
    curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_WRITEDATA, this);
    curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_WRITEFUNCTION, callBackFunc);
}

//-------------------------------------------------------------------------
void CCurlContext::clear(void) {
    earlyAbort = false;
    is_error = false;
    postData = "";
    result = "";
}

//-------------------------------------------------------------------------
typedef map<thread::id, CCurlContext*> CCurlThreadMap;

//-------------------------------------------------------------------------
CCurlContext* getCurlContext(void) {
    static CCurlThreadMap g_threadMap;
    thread::id threadID = this_thread::get_id();
    if (g_threadMap[threadID])
        return g_threadMap[threadID];
    // TODO(tjayrush): this memory is never released
    CCurlContext* cntx = new CCurlContext;
    g_threadMap[threadID] = cntx;
    if (verbose)
        cerr << "Created curl context `" << cntx << " for thread " << threadID << endl;
    return cntx;
}

//--------------------------------------------------------------------------
CURLCALLBACKFUNC CCurlContext::setCurlCallback(CURLCALLBACKFUNC func) {
    CURLCALLBACKFUNC prev = callBackFunc;
    callBackFunc = func;
    return prev;
}

//-------------------------------------------------------------------------
CURL* CCurlContext::getCurl(void) {
    // TODO(tjayrush): global data
    if (!curlHandle) {
        curlHandle = curl_easy_init();
        if (!curlHandle) {
            cerr << "Curl failed to initialize. Quitting..." << endl;
            quickQuitHandler(EXIT_FAILURE);
        }

        // curl_slist_append makes a copy of the string
        CStringArray heads;
        explode(heads, "Content-Type: application/json\n", '\n');
        for (auto head : heads)
            headerPtr = curl_slist_append(headerPtr, (char*)head.c_str());  // NOLINT
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerPtr);
        curl_easy_setopt(curlHandle, CURLOPT_URL, baseURL.c_str());
    }

    return curlHandle;
}

//-------------------------------------------------------------------------
void CCurlContext::releaseCurl(void) {
    if (headerPtr)
        curl_slist_free_all(headerPtr);
    headerPtr = NULL;

    if (curlHandle)
        curl_easy_cleanup(curlHandle);
    curlHandle = NULL;
}

//-------------------------------------------------------------------------
bool isNodeRunning(void) {
    CURLCALLBACKFUNC prev = getCurlContext()->setCurlCallback(nullCallback);
    getCurlContext()->setPostData("web3_clientVersion", "[]");
    CURLcode res = curl_easy_perform(getCurlContext()->getCurl());
    getCurlContext()->setCurlCallback(prev);
    return (res == CURLE_OK);
}

//-------------------------------------------------------------------------
static const char* STR_ERROR_CURLERR =
    "The RPC server ([{PROVIDER}]) was not found. Either start it, or edit the `rpcProvider`|"
    "value in the file `$CONFIG`.";

//-------------------------------------------------------------------------
static const char* STR_ERROR_CURLEMPTY = "The Ethereum node (`[{PROVIDER}]`) returned an empty response.";

//-------------------------------------------------------------------------
void curlErrorAndExit(const string_q& msgIn, const string_q& curlErr, const string& method, const string& params) {
    string_q errMsg = msgIn;
    replaceAll(errMsg, "[{METHOD}]", method);
    replaceAll(errMsg, "[{PARAMS}]", params);
    replaceAll(errMsg, "[{CURLERR}]", curlErr);
    replaceAll(errMsg, "[{PROVIDER}]", getCurlContext()->baseURL);

    errorMessage(errMsg);
    quickQuitHandler(EXIT_FAILURE);

    return;
}

//-------------------------------------------------------------------------
string_q callRPC(const string_q& method, const string_q& params, bool raw) {
    return getCurlContext()->perform(method, params, raw);
}

//-------------------------------------------------------------------------
string_q CCurlContext::perform(const string_q& method, const string_q& params, bool raw) {
    PRINTL("perform:\nmethod: " + method + "\nparams: " + params);
    PRINTL(showCommand(method, params, getCurlContext()->baseURL))
    //        getCurlContext()->methodMap[method]++;
    getCurlContext()->methodCnt++;
    getCurlContext()->setPostData(method, params);
    CURLcode res = curl_easy_perform(curlHandle);
    if (res != CURLE_OK && !earlyAbort) {
        PRINT("CURL returned an error: ! CURLE_OK")
        curlErrorAndExit(STR_ERROR_CURLERR, curl_easy_strerror(res), method, params);
    }

    PRINT("CURL returned CURLE_OK")
    string_q check = substitute(substitute(result, "VM execution error", ""), "\"error\":", "");
    if (result.empty()) {
        curlErrorAndExit(STR_ERROR_CURLEMPTY, "", method, params);

    } else if (contains(check, "error")) {  // don't consider an error in the VM
        if (reportErrors) {
            string_q waste = result;
            curlErrors.push_back(extractRPCError(waste));
        }
        LOG4(result + " " + postData);
        return result;
    }

    // PRINTL("Received: " + result);
    if (raw)
        return result;
    CRPCResult generic;
    generic.parseJson3(result);
    return generic.result;
}

//-------------------------------------------------------------------------
void nodeNotRequired(void) {
    getCurlContext()->nodeRequired = false;
}

//-------------------------------------------------------------------------
void nodeRequired(void) {
    getCurlContext()->nodeRequired = true;
    checkNodeRequired();
}

//-------------------------------------------------------------------------
void checkNodeRequired(void) {
    if (!getCurlContext()->nodeRequired)
        return;

    // Note -- this only tests if it's running. It does not quit on failure
    if (isNodeRunning())
        return;

    // This reports the error and quits
    curlErrorAndExit(STR_ERROR_CURLERR, "Could not connect to server", "web3_clientVersion", "");
}

//-------------------------------------------------------------------------
string_q setDataSource(const string_q& newSrc) {
    string_q ret = getCurlContext()->dataSource;
    if (newSrc == "local" || newSrc == "binary")
        getCurlContext()->dataSource = newSrc;
    return ret;
}

//-------------------------------------------------------------------------
size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string_q part;
    part.reserve(size * nmemb + 1);
    char* s = (char*)part.c_str();  // NOLINT
    strncpy(s, ptr, size * nmemb);
    s[size * nmemb] = '\0';
    ASSERT(userdata);
    CCurlContext* data = (CCurlContext*)userdata;  // NOLINT
    data->result += s;

    // PRINTQ("data->result ==> " + string_q(s));

    if (data && data->curlNoteFunc)
        if (!(*data->curlNoteFunc)(ptr, size, nmemb, userdata))  // returns zero if it wants us to stop
            return 0;
    return size * nmemb;
}

//-------------------------------------------------------------------------
size_t errorCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    // Curl does not close the string, so we have to
    ptr[size * nmemb - 1] = '\0';
    CCurlContext* data = (CCurlContext*)userdata;  // NOLINT
    data->result = "ok";
    if (strstr(ptr, "erro") != NULL) {
        data->result = "error";
        data->is_error = true;
        data->earlyAbort = true;
        return 0;
    }

    return size * nmemb;
}

//-------------------------------------------------------------------------
size_t nullCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    return size * nmemb;
}
}  // namespace qblocks
