/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
    size_t write_callback_internal(char *ptr, size_t size, size_t nmemb, void *userdata) {
        string result;
        size_t i = 0;
        for (i = 0 ; i < nmemb ; i++)
            result += ptr[i];
        result[i] = '\0';

        // store the new dasta in the caller's string
        string *str = reinterpret_cast<string*>(userdata); ASSERT(str);
        *str += result;

        // we handeled everything, tell curl to keep going
        return size * nmemb;
    }

    //---------------------------------------------------------------------------------------------------
    SFString urlToString(const SFString& url) {
        if (url.empty()) {
            getCurl_internal(true);
            return EMPTY;
        }

        string result;
        curl_easy_setopt(getCurl_internal(), CURLOPT_URL, (const char*)url);
        curl_easy_setopt(getCurl_internal(), CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(getCurl_internal(), CURLOPT_WRITEFUNCTION, write_callback_internal);
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
