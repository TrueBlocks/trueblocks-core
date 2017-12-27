/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "node.h"

namespace qblocks {
    
    //--------------------------------------------------------------------------
    CURLCALLBACKFUNC callBackFunc=NULL;
    CURLCALLBACKFUNC setCurlCallback(CURLCALLBACKFUNC func) {
        CURLCALLBACKFUNC prev = callBackFunc;
        callBackFunc = func;
        return prev;
    }
    
    //-------------------------------------------------------------------------
    static SFString curlHeaders =
    "Content-Type: application/json|";
    
    //-------------------------------------------------------------------------
    CURL *getCurl(bool cleanup)
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
            
            SFString head = curlHeaders;
            while (!head.empty()) {
                SFString next = nextTokenClear(head, '|');
                headers = curl_slist_append(headers, (char*)(const char*)next);
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            
            if (getSource() == "infura")
            {
                curl_easy_setopt(curl, CURLOPT_URL,        "https://pmainnet.infura.io/");
                
            } else
            {
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
    extern size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    static bool earlyAbort=false;
    bool is_error=false;
    bool is_tracing=false;
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
    size_t nullCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
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
    bool lightTracing(bool on) {
        bool ret = is_error;
        is_tracing = on;
        is_error   = false;
        return ret;
    }
    
    //-------------------------------------------------------------------------
    size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        if (callBackFunc)
            return (*callBackFunc)(ptr, size, nmemb, userdata);
        
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
    
}  // namespace qblocks
