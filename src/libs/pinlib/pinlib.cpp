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

#include "pinlib.h"

namespace qblocks {

extern bool pinlib_getFileByHash(const hash_t& hash, const string_q& outFilename);
#define hashToEmptyFile "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm"

//-------------------------------------------------------------------------
void pinlib_init(QUITHANDLER qh) {
    acctlib_init(qh);
    CPinnedChunk::registerClass();
    CPinataPin::registerClass();
    CPinataPinlist::registerClass();
    CPinApiLicense::registerClass();
    CPinataMetadata::registerClass();
    CPinataRegion::registerClass();
    CPinManifest::registerClass();
}

//-------------------------------------------------------------------------
void pinlib_cleanup(void) {
    acctlib_cleanup();
}

//----------------------------------------------------------------
bool pinlib_pinChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item) {
    if (!pinlib_readPinList(pList, false)) {
        return false;
    }

    // If already pinned, no reason to pin it again...
    CPinnedChunk copy;
    if (pinlib_findChunk(pList, fileName, copy)) {
        LOG_WARN("Pin for blocks ", fileName, " already exists.");
        item = copy;
        return true;
    }

    item.fileName = fileName;
    string_q indexStr = pinlib_pinOneFile(fileName, "finalized");
    if (!contains(indexStr, "IpfsHash")) {
        // LOG_ERR("Could not pin index for blocks ", fileName, " file to Pinata. Quitting...");
        LOG_ERR("Could not pin index for blocks file to Pinata. Quitting...");
        return false;
    }

    replace(indexStr, "IpfsHash", "ipfs_pin_hash");
    replace(indexStr, "PinSize", "size");
    replace(indexStr, "Timestamp", "date_pinned");
    CPinataPin index;
    index.parseJson3(indexStr);
    item.indexHash = index.ipfs_pin_hash;
    blknum_t end;
    timestamp_t ts;
    blknum_t start = bnFromPath(fileName, end, ts);
    LOG_INFO(bBlue, "  Pinned index for blocks ", start, " to ", end, " at ", item.indexHash, cOff);

    string_q bloomStr = pinlib_pinOneFile(fileName, "blooms");
    if (!contains(bloomStr, "IpfsHash")) {
        LOG_ERR("Could not pin bloom for blocks ", fileName, " file to Pinata. Quitting...");
        return false;
    }

    replace(bloomStr, "IpfsHash", "ipfs_pin_hash");
    replace(bloomStr, "PinSize", "size");
    replace(bloomStr, "Timestamp", "date_pinned");
    CPinataPin bloom;
    bloom.parseJson3(bloomStr);
    item.bloomHash = bloom.ipfs_pin_hash;
    LOG_INFO(bBlue, "  Pinned bloom for blocks ", start, " to ", end, " at ", item.bloomHash, cOff);

    // add it to the array
    pList.push_back(item);

    // write the array (after sorting it) to the database
    sort(pList.begin(), pList.end());
    return pinlib_writePinList(pList);
}

//---------------------------------------------------------------------------
bool pinlib_unpinChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item) {
    if (!pinlib_readPinList(pList, false))
        return false;

    // If we don't think it's pinned, Pinata may, so proceed even if not found
    CPinnedChunk copy;
    if (!pinlib_findChunk(pList, fileName, copy)) {
        item = copy;
        item.fileName = fileName;
        // return true;
    }

    CPinnedChunkArray array;
    for (auto pin : pList) {
        if (pin.fileName == fileName) {
            cout << "Unpinning: " << pin.fileName << endl;
            pinlib_unpinOneFile(pin.indexHash);
            pinlib_unpinOneFile(pin.bloomHash);
            item = pin;
        } else {
            cout << "Keeping " << pin.fileName << "\r";
            cout.flush();
            array.push_back(pin);
        }
    }
    cout << endl;

    pList.clear();
    pList = array;
    sort(pList.begin(), pList.end());
    return pinlib_writePinList(pList);
}

//-------------------------------------------------------------------------
bool pinlib_getChunkByHash(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item) {
    if (!pinlib_readPinList(pList, true))
        return false;

    // If we don't think it's pinned, Pinata may, so proceed even if not found
    if (!pinlib_findChunk(pList, fileName, item)) {
        // return true;
    }

    // clang-format off
    string_q fn = fileName + ".bin";
    pinlib_getFileByHash(item.indexHash, fn + ".gz");

    string_q cmd = "mv " + getCachePath("tmp/" + fn) + " " + indexFolder_finalized + fn;
    if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    return true;
}

//---------------------------------------------------------------------------
bool pinlib_findChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item) {
    if (!pinlib_readPinList(pList, false))
        return false;

    CPinnedChunk search;
    search.fileName = fileName;
    const vector<CPinnedChunk>::iterator it = find(pList.begin(), pList.end(), search);
    if (it != pList.end()) {
        item = *it;
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------
bool pinlib_forEveryPin(CPinnedChunkArray& pList, PINFUNC func, void* data) {
    if (!func)
        return false;

    if (!pinlib_readPinList(pList, true))
        return false;

    for (auto pin : pList) {
        if (!(*func)(pin, data))
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool pinlib_readPinList(CPinnedChunkArray& pinArray, bool required) {
    if (!pinArray.empty())
        return true;

    string_q binFile = getCachePath("tmp/pins.bin");
    string_q textFile = configPath("manifest/manifest.txt");

    time_q binDate = fileLastModifyDate(binFile);
    time_q textDate = fileLastModifyDate(textFile);

    // LOG8("binDate: ", binDate.Format(FMT_JSON));
    // LOG8("textDate: ", textDate.Format(FMT_JSON));

    if (binDate > textDate && fileExists(binFile)) {
        CArchive pinFile(READING_ARCHIVE);
        if (!pinFile.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_ERR("Could not open pin file for reading. Quitting...");
            return false;
        }
        pinFile >> pinArray;
        pinFile.Release();

    } else if (!fileExists(textFile)) {
        if (required) {
            LOG_ERR("Pins file is required, but not found. Quitting...");
            return false;
        }
        return true;

    } else {
        pinArray.clear();  // redundant, but fine
        string_q contents = asciiFileToString(textFile);
        CPinManifest report;
        report.parseJson3(contents);
        for (auto pin : report.pins)
            pinArray.push_back(pin);
        LOG4("Done Loading pins");
        sort(pinArray.begin(), pinArray.end());
        pinlib_writePinList(pinArray);
    }
    return true;
}

//---------------------------------------------------------------------------
bool pinlib_writePinList(CPinnedChunkArray& pList) {
    string_q binFile = getCachePath("tmp/pins.bin");
    establishFolder(binFile);

    lockSection();  // disallow control+C until we write both files
    CArchive pinFile(WRITING_ARCHIVE);
    if (!pinFile.Lock(binFile, modeWriteCreate, LOCK_WAIT)) {
        cerr << "Could not lock pin file for writing." << endl;
        unlockSection();  // enable control+C
        return false;
    }
    pinFile << pList;
    pinFile.Release();
    unlockSection();  // enable control+C

    return true;
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
string_q pinlib_pinOneFile(const string_q& fileName, const string_q& type) {
    LOG4("Starting pin");

    CPinApiLicense lic;
    if (!pinlib_getApiKeys(lic)) {
        cerr << "You need to put Pinata API keys in ~/.quickBlocks/blockScrape.toml" << endl;
        return "";
    }

    string_q source =
        (type == "blooms" ? substitute(substitute(fileName, ".bin", ".bloom"), "/finalized/", "/blooms/") : fileName);
    string_q zip = source;
    // LOG4("source: ", source, " ", fileExists(source));
    zip = source + ".gz";
    // clang-format off
    string_q cmd1 = "yes | gzip -n --keep " + source; // + " 2>/dev/null";
    if (system(cmd1.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on
    // LOG4("zip: ", zip, " ", fileExists(zip));

    string_q result;
    CURL* curl;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/pinning/pinFileToIPFS");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("pinata_api_key: " + lic.apiKey).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + lic.secretKey).c_str());
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

    // clang-format off
    // string_q cmd2 = "yes | rm -f " + zip;
    // if (system(cmd2.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    // LOG4("Finishing pin: ", result);
    LOG4("Finishing pin");
    return result;
}

//----------------------------------------------------------------
string_q pinlib_unpinOneFile(const string_q& hash) {
    LOG4("Starting unpin");

    CPinApiLicense lic;
    if (!pinlib_getApiKeys(lic)) {
        cerr << "You need to put Pinata API keys in ~/.quickBlocks/blockScrape.toml" << endl;
        return "";
    }

    string_q result;
    CURL* curl;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, ("https://api.pinata.cloud/pinning/unpin/" + hash).c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("pinata_api_key: " + lic.apiKey).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + lic.secretKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
        CURLcode res = curl_easy_perform(curl);
        if (res != 0) {
            result += curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    if (contains(result, "error"))
        LOG_WARN("Pinata returned an error: ", result);
    else
        LOG_INFO("Finishing unpin: ", result);
    return result;
}

//----------------------------------------------------------------
bool pinlib_getApiKeys(CPinApiLicense& lic) {
    lic.apiKey = getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_api_key", "<notset>");
    lic.secretKey = getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_secret_api_key", "<notset>");
    if (isTestMode())
        lic.apiKey = lic.secretKey = "--license_codes--";
    return (lic.apiKey != "<notset>" && lic.secretKey != "<notset>");
}

//---------------------------------------------------------------------------
// Assumes the license is valid
bool pinlib_getPinList(const CPinApiLicense& lic, string& result) {
    result = "";
    result.clear();

    bool ret = true;  // assume success

    CURL* curl;
    curl = curl_easy_init();  // TODO(tjayrush): Creating and destroying curl context is slow
    if (!curl) {
        curl_easy_cleanup(curl);
        result = "Could not open curl context";
        ret = false;
    } else {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        // TODO(tjayrush): This has pagination, I just didn't notice it
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/data/pinList?status=pinned");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("pinata_api_key: " + lic.apiKey).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + lic.secretKey).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
        CURLcode res = curl_easy_perform(curl);
        if (res != 0) {
            curl_easy_cleanup(curl);
            result = curl_easy_strerror(res);
            ret = false;
        }
    }
    curl_easy_cleanup(curl);
    if (!ret)  // if we already have an error
        return ret;

    if (result.empty()) {
        result = "No result";
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------
bool pinlib_getFileByHash(const hash_t& hash,
                          const string_q& outFilename) {  // also unzips if the file is zipped
    string_q cmd = "curl -s ";
    cmd += "\"https://ipfs.io/ipfs/" + hash + "\" ";
    cmd += "--output " + getCachePath("tmp/") + outFilename + " ; ";
    if (system(cmd.c_str())) {  // NOLINT
    }                           // Don't remove cruft. Silences compiler warnings
    if (endsWith(outFilename, ".gz")) {
        cmd = "cd " + getCachePath("tmp/") + " ; gunzip *.gz";
        if (system(cmd.c_str())) {  // NOLINT
        }                           // Don't remove cruft. Silences compiler warnings
    }

    return fileExists(substitute(outFilename, ".gz", ""));
}

}  // namespace qblocks
