#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    typedef size_t (*CURLCALLBACKFUNC)(char *ptr, size_t size, size_t nmemb, void *userdata);

    class CCurlContext {
    public:
        string_q         headers;
        string_q         baseURL;
        CURLCALLBACKFUNC callBackFunc;
        bool             earlyAbort;
        string_q         postData;
        string_q         result;
        string_q         provider;
        bool             is_error;
        size_t           theID;

        CCurlContext(void);
        string_q getCurlID(void);
        void setPostData(const string_q& method, const string_q& params);
        void Clear(void);
        CURLCALLBACKFUNC setCurlCallback(CURLCALLBACKFUNC func);
    };

    extern CURL         *getCurl         (bool cleanup = false);
    extern bool          isNodeRunning   (void);
    extern bool          nodeHasBalances (void);
    extern bool          getObjectViaRPC (CBaseNode &node, const string_q& method, const string_q& params);
    extern string_q      callRPC         (const string_q& method, const string_q& params, bool raw);
    extern CCurlContext *getCurlContext  (void);
    extern size_t        writeCallback   (char *ptr, size_t size, size_t nmemb, void *userdata);
    extern size_t        traceCallback   (char *ptr, size_t size, size_t nmemb, void *userdata);
}  // namespace qblocks
