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
        SFString         postData;
        SFString         result;
        SFString         provider;
        bool             is_error;
        uint32_t         theID;

        CCurlContext(void);
        SFString getCurlID(void);
        void setPostData(const SFString& method, const SFString& params);
        void Clear(void);
        CURLCALLBACKFUNC setCurlCallback(CURLCALLBACKFUNC func);
    };

    extern CURL         *getCurl         (bool cleanup=false);
    extern bool          isNodeRunning   (void);
    extern bool          nodeHasBalances (void);
    extern bool          getObjectViaRPC (CBaseNode &node, const SFString& method, const SFString& params);
    extern SFString      callRPC         (const SFString& method, const SFString& params, bool raw);
    extern CCurlContext *getCurlContext  (void);
    extern size_t        writeCallback   (char *ptr, size_t size, size_t nmemb, void *userdata);
    extern size_t        traceCallback   (char *ptr, size_t size, size_t nmemb, void *userdata);
}  // namespace qblocks
