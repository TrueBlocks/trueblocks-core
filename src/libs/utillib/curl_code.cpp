/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "database.h"
#include "options_base.h"

namespace qblocks {

//---------------------------------------------------------------------------------------------------
class CResponseData {
  public:
    CURLCALLBACKFUNC noteFunc;
    string_q response;
    CResponseData(void) : noteFunc(NULL) {
    }
};

//-------------------------------------------------------------------------
size_t internalCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    // We need to save the last character...
    char l = ptr[nmemb - 1];

    // ...because we're overwriting it (curl doesn't end the data with a '\0')...
    ptr[nmemb - 1] = '\0';

    // ... now we can copy the string...
    string result = ptr;

    // ...but we need the last character...
    result += l;

    // Now we copy out to the caller's buffer and make a note of it if told to
    CResponseData* dataPtr = reinterpret_cast<CResponseData*>(userdata);
    if (dataPtr) {
        dataPtr->response += result;
        if (dataPtr->noteFunc)
            (*dataPtr->noteFunc)(ptr, size, nmemb, userdata);
    }

    // We've handeled everything, tell curl to keep going
    return size * nmemb;
}

//---------------------------------------------------------------------------------------------------
string_q urlToString(const string_q& url, CURLCALLBACKFUNC noteFunc) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        errorMessage("Curl failed to initialize.");
        quickQuitHandler(0);
    }
    CResponseData result;
    result.noteFunc = noteFunc;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, internalCallback);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        string_q msg = "curl_easy_perform() failed with: ";
        msg += curl_easy_strerror(res);
        errorMessage(msg);
        quickQuitHandler(0);
    }
    curl_easy_cleanup(curl);
    return result.response;
}

}  // namespace qblocks
