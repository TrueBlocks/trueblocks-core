#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "etherlib.h"

namespace qblocks {
    
    //-------------------------------------------------------------------------
    typedef size_t (*CURLCALLBACKFUNC)(char *ptr, size_t size, size_t nmemb, void *userdata);

    class CCurlContext {
    public:
        SFString         headers;
        SFString         baseURL;
        CURLCALLBACKFUNC callBackFunc;
        bool             earlyAbort;
        SFString         url;
        SFString         result;
        SFString         source;
        bool             tracing_on;
        bool             is_error;
        bool             is_tracing;
        uint32_t         theID;

        CCurlContext(void);
        SFString getCurlID(void);
        void setPostData(const SFString& method, const SFString& params);
        void Clear(void);
        void tracingOff (void);
        void tracingOn  (void);
        bool isTracingOn(void);
        bool lightTracing(bool on);
        CURLCALLBACKFUNC setCurlCallback(CURLCALLBACKFUNC func);
    };

    extern CURL         *getCurl         (bool cleanup=false);
    extern bool          isNodeRunning   (void);
    extern bool          getObjectViaRPC (CBaseNode &node, const SFString& method, const SFString& params);
    extern SFString      callRPC         (const SFString& method, const SFString& params, bool raw);
    extern CCurlContext *getCurlContext  (void);
    extern size_t        write_callback  (char *ptr, size_t size, size_t nmemb, void *userdata);
}  // namespace qblocks
