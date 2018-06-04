#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include <string>
#include "basetypes.h"
#include "biglib.h"
#include "curl_code.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    CURL *getCurl_internal(bool cleanup = false) {
        static CURL *curl = NULL;
        if (!curl) {

            curl = curl_easy_init();
            if (!curl) {
                fprintf(stderr, "Curl failed to initialize. Quitting...\n");
                exit(0);
            }

        } else if (cleanup) {

            if (curl)
                curl_easy_cleanup(curl);
            return NULL;
        }
        return curl;
    }

    //-------------------------------------------------------------------------
    size_t internalCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {

        // We need to save the last character...
        char l = ptr[nmemb-1];

        // ...because we're overwriting it (curl doesn't end the data with a '\0')...
        ptr[nmemb-1] = '\0';

        // ... now we can copy the string...
        string result = ptr;

        // ...but we need the last character...
        result += l;

        // Now we copy out to the caller's buffer
        string *str = reinterpret_cast<string*>(userdata);
        ASSERT(str);
        *str += result;

        // We've handeled everything, tell curl to keep going
        return size * nmemb;
    }

    //---------------------------------------------------------------------------------------------------
    SFString urlToString(const SFString& url) {
        if (url.empty()) {
            getCurl_internal(true);
            return EMPTY;
        }

        string result;
        curl_easy_setopt(getCurl_internal(), CURLOPT_URL,           (const char*)url);
        curl_easy_setopt(getCurl_internal(), CURLOPT_WRITEDATA,     &result);
        curl_easy_setopt(getCurl_internal(), CURLOPT_WRITEFUNCTION, internalCallback);
        CURLcode res = curl_easy_perform(getCurl_internal());
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            exit(0);
        }

        return result.c_str();
    }

    //-------------------------------------------------------------------------
    class Cleanup {
    public:
        uint32_t unused;
        Cleanup() { unused = 0; }
        ~Cleanup() {
            urlToString();
        }
    };

    // destructor gets called on pragram exit to clear curl handle
    static Cleanup cleanUp;

}  // namespace qblocks
