/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
extern bool pinlib_pinChunk(const string_q& fn, CPinnedChunk& item);
extern string_q pinOneChunk(const string_q& fileName, const string_q& type);
bool visitToPin(const string_q& fullPath, void* unused) {
    LOG_INFO("fullPath: ", fullPath);

    CPinnedChunk pinRecord;
    pinlib_pinChunk(fullPath, pinRecord);

    string_q range = substitute(pinRecord.range, indexFolder_finalized, "");
    range = substitute(range, indexFolder_blooms, "");
    range = substitute(range, ".bin", "");

    ostringstream os;
    os << range << "\t" << pinRecord.bloomHash << "\t" << pinRecord.indexHash << endl;
    string_q manifestFile = chainConfigsTxt_manifest;
    os << asciiFileToString(manifestFile);
    stringToAsciiFile(manifestFile, os.str());

    ostringstream rs;
    rs << range << "\t" << pinRecord.Format("[{BLOOMHASH}]\t[{INDEXHASH}]") << endl;
    appendToAsciiFile(cacheFolder_tmp + "newpins.txt", rs.str());
    LOG_INFO("Pinned ", fullPath, " ", pinRecord.Format());

    return !shouldQuit();
}

//-------------------------------------------------------------------------
static size_t curlCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string_q part;
    part.reserve(size * nmemb + 1);
    char* s = (char*)part.c_str();  // NOLINT
    strncpy(s, ptr, size * nmemb);
    s[size * nmemb] = '\0';
    string_q* str = (string_q*)userdata;  // NOLINT
    ASSERT(str);
    *str += s;
    return size * nmemb;
}

//----------------------------------------------------------------
bool pinlib_pinChunk(const string_q& fullPath, CPinnedChunk& item) {
    item.range = fullPath;
    string_q indexStr = pinOneChunk(item.range, "finalized");
    if (!contains(indexStr, "IpfsHash")) {
        // LOG_ERR("Could not pin index for blocks ", item.range, " file to Pinata. Quitting...");
        LOG_ERR("Could not pin index for blocks file to Pinata. Quitting...");
        return false;
    }

    replace(indexStr, "IpfsHash", "ipfs_pin_hash");
    replace(indexStr, "PinSize", "size");
    CPinnedChunk index;
    index.parseJson3(indexStr);
    item.indexHash = index.ipfs_pin_hash;
    blknum_t end;
    timestamp_t ts;
    blknum_t start = path_2_Bn(item.range, end, ts);
    LOG_INFO(bBlue, "  Pinned index for blocks ", start, " to ", end, " at ", item.indexHash, cOff);

    string_q bloomStr = pinOneChunk(item.range, "blooms");
    if (!contains(bloomStr, "IpfsHash")) {
        LOG_ERR("Could not pin bloom for blocks ", item.range, " file to Pinata. Quitting...");
        return false;
    }

    replace(bloomStr, "IpfsHash", "ipfs_pin_hash");
    replace(bloomStr, "PinSize", "size");
    CPinnedChunk bloom;
    bloom.parseJson3(bloomStr);
    item.bloomHash = bloom.ipfs_pin_hash;
    LOG_INFO(bBlue, "  Pinned bloom for blocks ", start, " to ", end, " at ", item.bloomHash, cOff);
    return true;
}

//----------------------------------------------------------------
string_q pinOneChunk(const string_q& fileName, const string_q& type) {
    string_q source =
        (type == "blooms" ? substitute(substitute(fileName, ".bin", ".bloom"), "/finalized/", "/blooms/") : fileName);
    string_q zip = source;
    zip = source + ".gz";
    // clang-format off
    string_q cmd1 = "yes | gzip -n -k " + source; // + " 2>/dev/null";
    if (system(cmd1.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    CApiKey lic;
    if (!getApiKey(lic)) {
        cerr << "You need to put Pinata API keys in $CONFIG/blockScrape.toml" << endl;
        return "";
    }

    string_q result;
    CURL* curl;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/pinning/pinFileToIPFS");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("pinata_api_key: " + lic.key).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + lic.secret).c_str());
        headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
        curl_mime* mime;
        curl_mimepart* part;
        mime = curl_mime_init(curl);
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "file");
        curl_mime_filedata(part, zip.c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result += curl_easy_strerror(res);
        }
        curl_mime_free(mime);
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);
    ::remove(zip.c_str());
    return result;
}
