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
#include "node.h"
#include "node_curl.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    extern size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
    extern size_t errorCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
    extern size_t nullCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

    //-------------------------------------------------------------------------
    CCurlContext::CCurlContext(void) {
        headerStr    = "Content-Type: application/json\n";
        baseURL      = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
        callBackFunc = writeCallback;
        curlNoteFunc = NULL;
        theID        = 1;
        nodeRequired = true;
        earlyAbort   = false;
        is_error     = false;
        postData     = "";
        result       = "";
        curlHandle   = NULL;
        headerPtr    = NULL;
//      source       = "binary";
    }

    //-------------------------------------------------------------------------
    string_q CCurlContext::getCurlID(void) {
        return uint_2_Str(isTestMode() ? 1 : theID++);
    }

//-------------------------------------------------------------------------
//#define DEBUG_RPC
#ifdef DEBUG_RPC
int x = 0;
#define PRINT(msg) \
cerr << string_q(120, '-') << "\n" << ++x << "." << msg << "\n"; \
cerr << "\tresult: \t[" << substitute(getCurlContext()->result, "\n", " ") << "]\n"; \
cerr << "\tearlyAbort:\t" << getCurlContext()->earlyAbort << "\n"; \
cerr << "\tcurlID: \t" << getCurlContext()->getCurlID() << "\n";
#define PRINTQ(msg) \
cerr << string_q(120, '-') << "\n" << ++x << "." << msg << "\n"; \
cerr << "\tcurlID: \t" << getCurlContext()->getCurlID() << "\n";
#define PRINTL(msg) \
cerr << string_q(120,'-') << "\n"; \
PRINT(msg);
#else  // DEBUG_RPC
#define PRINTQ(msg)
#define PRINT(msg)
#define PRINTL(msg)
#endif  // DEBUG_RPC

    //-------------------------------------------------------------------------
    void CCurlContext::setPostData(const string_q& method, const string_q& params) {
        clear();
        postData  = "{";
        postData +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
        postData +=  quote("method")  + ":"  + quote(method) + ",";
        postData +=  quote("params")  + ":"  + params + ",";
        postData +=  quote("id")      + ":"  + quote(getCurlContext()->getCurlID());
        postData += "}";
#ifdef PROVING
        if (expContext().proving) {
            if (expContext().proof.str().length() > 1)
                expContext().proof << ",";
            expContext().proof << postData;
        }
#endif

PRINT("postData: " + postData);

        curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_POSTFIELDS,    postData.c_str());
        curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_POSTFIELDSIZE, postData.length());
        curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_WRITEDATA,     this);
        curl_easy_setopt(getCurlContext()->getCurl(), CURLOPT_WRITEFUNCTION, callBackFunc);
    }

    //-------------------------------------------------------------------------
    void CCurlContext::clear(void) {
        earlyAbort   = false;
        is_error     = false;
        postData     = "";
        result       = "";
    }

    //-------------------------------------------------------------------------
    //TODO(tjayrush): global data
    static CCurlContext theCurlContext;

    //-------------------------------------------------------------------------
    CCurlContext *getCurlContext(void) {
        return &theCurlContext;
    }

    //--------------------------------------------------------------------------
    CURLCALLBACKFUNC CCurlContext::setCurlCallback(CURLCALLBACKFUNC func) {
        CURLCALLBACKFUNC prev = getCurlContext()->callBackFunc;
        getCurlContext()->callBackFunc = func;
        return prev;
    }

    //-------------------------------------------------------------------------
    CURL *CCurlContext::getCurl(void) {
        //TODO(tjayrush): global data
        if (!curlHandle) {
            curlHandle = curl_easy_init();
            if (!curlHandle) {
                fprintf(stderr, "Curl failed to initialize. Quitting...\n");
                exit(0);
            }

            string_q head = getCurlContext()->headerStr;
            while (!head.empty()) {
                string_q next = nextTokenClear(head, '\n');
                headerPtr = curl_slist_append(headerPtr, (char*)next.c_str());  // NOLINT
            }
            curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerPtr);
            curl_easy_setopt(curlHandle, CURLOPT_URL, getCurlContext()->baseURL.c_str());
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

#define OLD_CODE
    //-------------------------------------------------------------------------
    bool isNodeRunning(void) {
#ifdef OLD_CODE
        CURLCALLBACKFUNC prev = getCurlContext()->setCurlCallback(nullCallback);
        getCurlContext()->setPostData("web3_clientVersion", "[]");
        CURLcode res = curl_easy_perform(getCurlContext()->getCurl());
        getCurlContext()->setCurlCallback(prev);
        return (res == CURLE_OK);
#else
        getCurlContext()->clear();
        string_q postData;
        postData = "{";
        postData +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
        postData +=  quote("method")  + ":"  + quote("web3_clientVersion") + ",";
        postData +=  quote("params")  + ":"  + "[]" + ",";
        postData +=  quote("id")      + ":"  + quote(getCurlContext()->getCurlID());
        postData += "}";
extern size_t nullCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
        CURLcode ret;
        ret = curl_easy_setopt(getCurlContext()->curlHandle, CURLOPT_POSTFIELDS,    postData.c_str());
        ret = curl_easy_setopt(getCurlContext()->curlHandle, CURLOPT_POSTFIELDSIZE, postData.length());
        ret = curl_easy_setopt(getCurlContext()->curlHandle, CURLOPT_WRITEDATA,     NULL);
        ret = curl_easy_setopt(getCurlContext()->curlHandle, CURLOPT_WRITEFUNCTION, nullCallback);
        CURLcode res = curl_easy_perform(getCurlContext()->curlHandle);
        return (res == CURLE_OK);
#endif
    }

    //-------------------------------------------------------------------------
    string_q callRPC(const string_q& method, const string_q& params, bool raw) {

PRINTL("callRPC:\n\tmethod:\t\t" + method + params + "\n\tsource:\t\t" + getCurlContext()->provider);

        // getCurlContext()->callBackFunc = writeCallback;
        getCurlContext()->setPostData(method, params);
        CURLcode res = curl_easy_perform(getCurlContext()->getCurl());
        if (res != CURLE_OK && !getCurlContext()->earlyAbort) {
PRINT("CURL returned an error: ! CURLE_OK")
            cerr << "\n";
            cerr << cYellow << "\tWarning: " << cOff << "The request to the Ethereum node ";
            cerr << "resulted in\n\tfollowing error message: ";
            cerr << bTeal << curl_easy_strerror(res) << cOff << ".\n";
            cerr << "\tIt is impossible for QBlocks to proceed. Quitting...\n";
            cerr << "\n";
            quickQuitHandler(0);
        }

PRINT("CURL returned CURLE_OK")

        if (getCurlContext()->result.empty()) {
            cerr << cYellow;
            cerr << "\n";
            cerr << "\tWarning:" << cOff << "The Ethereum node resulted in an empty\n";
            cerr << "\tresponse. It is impossible for QBlocks to proceed. Quitting...\n";
            cerr << "\n";
            exit(0);
        } else if (contains(getCurlContext()->result, "error")) {
#ifndef DEBUG_RPC
            if (verbose > 1)
#endif
            {
                cerr << getCurlContext()->result;
                cerr << getCurlContext()->postData << "\n";
            }
        }

PRINTL("Received: " + getCurlContext()->result);
#ifdef DEBUG_RPC
x = 0;
#endif
        if (raw)
            return getCurlContext()->result;
        CRPCResult generic;
        generic.parseJson3(getCurlContext()->result);
        return generic.result;
    }

    //-------------------------------------------------------------------------
    void nodeNotRequired(void) {
        getCurlContext()->nodeRequired = false;
    }

    //-------------------------------------------------------------------------
    string_q setDataSource(const string_q& newSrc) {
        string_q old = getCurlContext()->provider;
        if (newSrc == "local" || newSrc == "binary")
            getCurlContext()->provider = newSrc;
        return old;
    }

    //-------------------------------------------------------------------------
    size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
        string_q part;
        part.reserve(size*nmemb+1);
        char *s = (char*)part.c_str();  // NOLINT
        strncpy(s, ptr, size * nmemb);
        s[size*nmemb]='\0';
        ASSERT(userdata);
        CCurlContext *data = (CCurlContext*)userdata;  // NOLINT
        data->result += s;

PRINTQ("data->result ==> " + string_q(s));

        if (data && data->curlNoteFunc)
            if (!(*data->curlNoteFunc)(ptr, size, nmemb, userdata))  // returns zero if it wants us to stop
                return 0;
        return size * nmemb;
    }

    //-------------------------------------------------------------------------
    size_t errorCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
        // Curl does not close the string, so we have to
        ptr[size*nmemb-1] = '\0';
        CCurlContext *data = (CCurlContext*)userdata;  // NOLINT
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
    size_t nullCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
        return size * nmemb;
    }
}  // namespace qblocks
