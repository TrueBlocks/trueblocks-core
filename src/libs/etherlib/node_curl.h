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

    //-------------------------------------------------------------------------
    extern CURLCALLBACKFUNC setCurlCallback  (CURLCALLBACKFUNC func);
    extern size_t           write_callback   (char *ptr, size_t size, size_t nmemb, void *userdata);
    
    //-------------------------------------------------------------------------
    extern void      tracingOff              (void);
    extern void      tracingOn               (void);
    extern bool      isTracingOn             (void);
    extern bool      lightTracing            (bool on);

    extern bool      isNodeRunning           (void);

    extern CURL     *getCurl                 (bool cleanup=false);
    extern SFString  callRPC                 (const SFString& method, const SFString& params, bool raw);
    extern bool      getObjectViaRPC         (CBaseNode &node, const SFString& method, const SFString& params);

}  // namespace qblocks
