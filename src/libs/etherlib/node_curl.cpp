/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "node.h"
#include "node_curl.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    extern size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    extern size_t nullCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

    //-------------------------------------------------------------------------
    CCurlContext::CCurlContext(void) {
        headers      = "Content-Type: application/json\n";
        baseURL      = "http://localhost:8545";
        callBackFunc = write_callback;
        theID        = 1;
        Clear();
    }

    //-------------------------------------------------------------------------
    SFString CCurlContext::getCurlID(void) {
        return asString(isTestMode() ? 1 : theID++);
    }

    //-------------------------------------------------------------------------
    void CCurlContext::setPostData(const SFString& method, const SFString& params) {
        Clear();
        postData += "{";
        postData +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
        postData +=  quote("method")  + ":"  + quote(method) + ",";
        postData +=  quote("params")  + ":"  + params + ",";
        postData +=  quote("id")      + ":"  + quote(getCurlID());
        postData += "}";
        //#define DEBUG_RPC
#ifdef DEBUG_RPC
        cerr << postData << "\n";
        cerr.flush();
#endif
        curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDS,    (const char*)postData);
        curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDSIZE, postData.length());
        curl_easy_setopt(getCurl(), CURLOPT_WRITEDATA,     this);
        curl_easy_setopt(getCurl(), CURLOPT_WRITEFUNCTION, callBackFunc);
    }

    //-------------------------------------------------------------------------
    void CCurlContext::Clear(void) {
        tracing_on   = true;
        earlyAbort   = false;
        is_error     = false;
        postData     = "";
        result       = "";
//      is_tracing   = false;
//      source       = "binary";
    }

    //-------------------------------------------------------------------------
    void CCurlContext::tracingOff (void) { tracing_on = false; }
    void CCurlContext::tracingOn  (void) { tracing_on = true;  }
    bool CCurlContext::isTracingOn(void) { return tracing_on;  }

    //-------------------------------------------------------------------------
    bool CCurlContext::lightTracing(bool on) {
        bool ret = is_error;
        is_tracing = on;
        is_error   = false;
        return ret;
    }

    //-------------------------------------------------------------------------
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
    CURL *getCurl(bool cleanup) {
        static CURL *curl = NULL;
        static struct curl_slist *headers = NULL;
        if (!curl && !cleanup) {
            curl = curl_easy_init();
            if (!curl) {
                fprintf(stderr, "Curl failed to initialize. Quitting...\n");
                exit(0);
            }

            SFString head = getCurlContext()->headers;
            while (!head.empty()) {
                SFString next = nextTokenClear(head, '\n');
                headers = curl_slist_append(headers, (char*)(const char*)next);
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            if (getCurlContext()->source == "remote") {
                curl_easy_setopt(curl, CURLOPT_URL, "https://pmainnet.infura.io/");

            } else if (getCurlContext()->source == "ropsten") {
                curl_easy_setopt(curl, CURLOPT_URL, "https://testnet.infura.io/");

            } else {
                curl_easy_setopt(curl, CURLOPT_URL, (const char*)getCurlContext()->baseURL);
            }

        } else if (cleanup) {

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
    bool isNodeRunning(void) {
        CURLCALLBACKFUNC prev = getCurlContext()->setCurlCallback(nullCallback);
        getCurlContext()->setPostData("web3_clientVersion", "[]");
        CURLcode res = curl_easy_perform(getCurl());
        getCurlContext()->setCurlCallback(prev);
        return (res == CURLE_OK);
    }

    //-------------------------------------------------------------------------
    SFString callRPC(const SFString& method, const SFString& params, bool raw) {

        //getCurlContext()->callBackFunc = write_callback;
        getCurlContext()->setPostData(method, params);

        CURLcode res = curl_easy_perform(getCurl());
        if (res != CURLE_OK && !getCurlContext()->earlyAbort) {
            SFString currentSource = getCurlContext()->source;
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
                getCurlContext()->theID--;
                getCurlContext()->source = "remote";
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

        if (!getCurlContext()->is_tracing && getCurlContext()->result.empty()) {
            cerr << cYellow;
            cerr << "\n";
            cerr << "\tWarning:" << cOff << "The Ethereum node  resulted in an empty\n";
            cerr << "\tresponse. It is impossible forQuickBlocks to proceed. Quitting...\n";
            cerr << "\n";
            exit(0);
        } else if (getCurlContext()->result.Contains("error")) {
            if (verbose>1) {
                cerr << getCurlContext()->result;
                cerr << getCurlContext()->postData << "\n";
            }
        }

#ifdef DEBUG_RPC
        //    cout << "\n" << SFString('-',80) << "\n";
        //    cout << thePost << "\n";
        cout << SFString('=',60) << "\n";
        cout << "received: " << getCurlContext()->result << "\n";
        cout.flush();
#endif

        if (raw)
            return getCurlContext()->result;
        CRPCResult generic;
        char *p = cleanUpJson((char*)(const char*)getCurlContext()->result);
        generic.parseJson(p);
        return generic.result;
    }

    //-------------------------------------------------------------------------
    bool getObjectViaRPC(CBaseNode &node, const SFString& method, const SFString& params) {
        SFString ret = callRPC(method, params, false);
        node.parseJson((char *)(const char*)ret);
        return true;
    }

    //-------------------------------------------------------------------------
    size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        if (getCurlContext()->is_tracing) {
//            cout << bRed << "." << cOff;
//            cout.flush();
            // Curl does not close the string, so we have to
            ptr[size*nmemb-1] = '\0';
            if (strstr(ptr,"erro")!=NULL) {
                getCurlContext()->is_error = true;
                getCurlContext()->earlyAbort = true;
                return 0;
            }

        } else {
            SFString part;
            part.reserve(size*nmemb+1);
            char *s = (char*)(const char*)part;
            strncpy(s,ptr,size*nmemb);
            s[size*nmemb]='\0';
            ASSERT(userdata);
            CCurlContext *data = (CCurlContext*)userdata;
            data->result += s;
            // Starting around block 3804005, there was a hack wherein the byte code 5b5b5b5b5b5b5b5b5b5b5b5b
            // repeated thousands of times, doing nothing. If we don't handle this, it dominates the scanning
            // for no reason
            if (strstr(s, "5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b") != NULL) {
                // This is the hack trace (there are many), so skip it
                cerr << "Curl response contains '5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b'. Aborting.\n";
                cerr.flush();
                getCurlContext()->earlyAbort = true;
                return 0;
            }
        }

        return size*nmemb;
    }

    //-------------------------------------------------------------------------
    size_t nullCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
        return size*nmemb;
    }
}  // namespace qblocks
