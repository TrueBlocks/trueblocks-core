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
    extern void      setNoTracing            (bool val);
    extern bool      getObjectViaRPC         (CBaseNode &node, const SFString& method, const SFString& params);
    extern bool      lightTracing            (bool on);
    extern CURL     *getCurl                 (bool cleanup=false);
    extern SFString  callRPC                 (const SFString& method, const SFString& params, bool raw);
    extern bool      isNodeRunning           (void);
    extern bool      is_error;
    extern bool      is_tracing;

}  // namespace qblocks
