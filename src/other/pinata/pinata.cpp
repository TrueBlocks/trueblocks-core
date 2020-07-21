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
    etherlib_init(quickQuitHandler);

    CPinningRecordArray records;
    forEveryFileInFolder(indexFolder_blooms, visitBloom, &records);

    etherlib_cleanup();
    return 1;
}

//----------------------------------------------------------------
void pinToPinata(const string_q& indexFile, const string_q& bloomFile) {
    CURL *curl;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/pinning/pinFileToIPFS");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "pinata_api_key: VALUE");
        headers = curl_slist_append(headers, "pinata_secret_api_key: VALUE");
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
        curl_mime_filedata(part, indexFile.c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        CURLcode res = curl_easy_perform(curl);
	    if (res == 0) {
		    cout << result << endl;
	    }
	    cerr << "Curl result: " << res << endl;
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);
}

//----------------------------------------------------------------
bool visitBloom(const string& path, void *data) {
   if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitBloom, data);
    } else {
        if (endsWith(path, ".bloom")) {
            cerr << "Pushing " << path << " to Pinata" << endl;
            pinToPinata(path, "");
            string_q index = substitute(substitute(path, "blooms", "finalized"), ".bloom", ".bin");
            cerr << "Pushing " << index << " to Pinata" << endl;
            pinToPinata(index, "");
            cerr << "Sleeping for X seconds." << endl;
            for (uint32_t i = 0 ; i < SECONDS ; i++) {
                cerr << "."; usleep(1 * 1000000);
            }
            cerr << endl;
        }
    }
    return true;
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

