//{"IpfsHash":"QmQEkGu44f1cqcszfsCmx15MaUDYZSTsX5E7oWWVRyxRZR","PinSize":4756031,"Timestamp":"2020-07-21T01:46:26.818Z"}
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
#include "etherlib.h"
#include "pinningRecord.h"

#define SECONDS 2

extern size_t testCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
extern bool visitBloom(const string& path, void *data);
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();

    string_q fileName = "./test.fil";
    string_q API_KEY = "pinata_api_key: 4586b0cfbb404d87be6e";
    string_q API_SECRET_KEY = "pinata_secret_api_key: 9989d172ecb411ed57017501105ee2f8da832330470ae03cdc76157d165fc858";

    CURL *curl;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/pinning/pinFileToIPFS");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, API_KEY.c_str());
        headers = curl_slist_append(headers, API_SECRET_KEY.c_str());
        headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        string_q result;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, testCallback);
        curl_mime *mime;
        curl_mimepart *part;
        mime = curl_mime_init(curl);
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "file");
        curl_mime_filedata(part, fileName.c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        CURLcode res = curl_easy_perform(curl);
        cout << "Curl response: ";
        if (res == 0) {
            cout << result << endl;
        } else {
            cerr << res << ": " << curl_easy_strerror(res) << endl;
        }
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);

    return 1;
}

//-------------------------------------------------------------------------
size_t testCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string_q part;
    part.reserve(size * nmemb + 1);
    char* s = (char*)part.c_str();  // NOLINT
    strncpy(s, ptr, size * nmemb);
    s[size * nmemb] = '\0';
    string_q *str = (string_q*)userdata;
    ASSERT(str);
    *str += s;
    //cout << s << endl;
    return size * nmemb;
}

