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
    baseURL = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
    debugging = getGlobalConfig()->getConfigBool("dev", "debug_curl", false);
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
#define PRINT(msg)                                                                                                     \
    if (debugging) {                                                                                                   \
        cout.flush();                                                                                                  \
        cerr.flush();                                                                                                  \
        string_q res = substitute(result, "\n", " ");                                                                  \
        replaceAll(res, "{", "\n{\n");                                                                                 \
        replaceAll(res, "}", "\n}\n");                                                                                 \
        replaceAll(res, "[", "\n[\n");                                                                                 \
        replaceAll(res, "]", "\n]\n");                                                                                 \
        replaceAll(res, ",", "\n");                                                                                    \
        cerr << string_q(50, '-') << endl;                                                                             \
        cerr << msg << endl;                                                                                           \
        CStringArray lines;                                                                                            \
        explode(lines, res, '\n');                                                                                     \
        sort(lines.begin(), lines.end());                                                                              \
        cerr << cGreen << "\tresult:\n" << cOff;                                                                       \
        for (auto line : lines) {                                                                                      \
            if (!line.empty())                                                                                         \
                cerr << "\t  " << line << endl;                                                                        \
        }                                                                                                              \
        cerr << cGreen << "\tearlyAbort: " << cOff << "\t" << earlyAbort << endl;                                      \
        cerr << cGreen << "\tcurlID: " << cOff << "\t" << getCurlID() << endl;                                         \
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
            fprintf(stderr, "Curl failed to initialize. Quitting...\n");
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
static const char* STR_CURLERROR =
    "The request to the Ethereum node ([{METHOD}]:[{PARAMS}]) resulted|in following error message: [VAL].";

//-------------------------------------------------------------------------
static const char* STR_CURLRESEMPTY = "The Ethereum node returned an empty response.";

static const char* STR_ERROR_NODEREQUIRED = "The Ethereum RPC: '[RPC]' was not found.";

//-------------------------------------------------------------------------
void displayCurlError(const string_q& msg, const string_q& val) {
    errorMessage(substitute(substitute(msg, "[VAL]", val), "[RPC]", getCurlContext()->baseURL));
    quickQuitHandler(EXIT_FAILURE);
    return;
}

//-------------------------------------------------------------------------
string_q callRPC(const string_q& method, const string_q& params, bool raw) {
    return getCurlContext()->perform(method, params, raw);
}

//-------------------------------------------------------------------------
string_q CCurlContext::perform(const string_q& method, const string_q& params, bool raw) {
    PRINTL("perform:\n\tmethod:\t\t" + method + "\n\tparams:\t\t" + params);
    //        getCurlContext()->methodMap[method]++;
    getCurlContext()->methodCnt++;
    getCurlContext()->setPostData(method, params);
    CURLcode res = curl_easy_perform(curlHandle);
    if (res != CURLE_OK && !earlyAbort) {
        PRINT("CURL returned an error: ! CURLE_OK")
        displayCurlError(substitute(substitute(STR_CURLERROR, "[{METHOD}]", method), "[{PARAMS}]", params),
                         curl_easy_strerror(res));
    }

    PRINT("CURL returned CURLE_OK")
    if (result.empty()) {
        displayCurlError(STR_CURLRESEMPTY);

    } else if (contains(result, "error")) {
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
void namesNotRequired(void) {
    setenv("NO_NAMES", "true", true);
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

    if (isNodeRunning())
        return;

    displayCurlError(STR_ERROR_NODEREQUIRED);
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
