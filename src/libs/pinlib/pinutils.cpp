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
#include "pinutils.h"

namespace qblocks {

static string_q pinOneFile(const string_q& fileName, const string_q& type);
static string_q unpinOneFile(const string_q& hash);
static void cleanPinataStr(string_q& in);
static bool writeManifest(const CPinnedItemArray& array);
static bool readManifest(bool required = false);

//---------------------------------------------------------------------------
static CPinnedItemArray pinList;
static CPinReport pinReport;

//-------------------------------------------------------------------------
bool forEveryPin(PINFUNC func, void* data) {
    if (!func)
        return false;
    if (!readManifest(true))
        return false;
    for (auto pin : pinList) {
        if (!(*func)(pin, data))
            return false;
    }
    return true;
}

//----------------------------------------------------------------
bool addNewPin(CPinnedItem& pin, void* data) {
    CPinReport* reportPtr = (CPinReport*)data;
    reportPtr->newPins.push_back(pin);

    timestamp_t unused;
    blknum_t newEnd;
    blknum_t newStart = bnFromPath(pin.fileName, newEnd, unused);

    if (reportPtr->newBlockRange.empty()) {
        reportPtr->newBlockRange = padNum9(newStart) + "-" + padNum9(newEnd);
    } else {
        blknum_t oldEnd;
        blknum_t oldStart = bnFromPath(reportPtr->newBlockRange, oldEnd, unused);
        reportPtr->newBlockRange = padNum9(min(oldStart, newStart)) + "-" + padNum9(max(oldEnd, newEnd));
    }
    return true;
}

//-------------------------------------------------------------------------
bool getFileByHash(const hash_t& hash, const string_q& outFilename) {  // also unzips if the file is zipped
    string_q cmd = "curl -s ";
    cmd += "\"https://ipfs.io/ipfs/" + hash + "\" ";
    cmd += "--output " + getCachePath("tmp/") + outFilename + " ; ";
    if (system(cmd.c_str())) {
    }  // Don't remove cruft. Silences compiler warnings

    if (endsWith(outFilename, ".gz")) {
        cmd = "cd " + getCachePath("tmp/") + " ; gunzip *.gz";
        if (system(cmd.c_str())) {
        }  // Don't remove cruft. Silences compiler warnings
    }

    return fileExists(substitute(outFilename, ".gz", ""));
}

//-------------------------------------------------------------------------
string_q getFileContentsByHash(const hash_t& hash) {  // also unzips if the file is zipped
    string_q cmd = "curl -s ";
    cmd += "\"https://ipfs.io/ipfs/" + hash + "\" ";
    return doCommand(cmd);
}

//----------------------------------------------------------------
hash_t getCurrentManifest(void) {
    CAbi abi;
    abi.loadAbiFromFile(configPath("known_abis/unchained.json"), false);
    address_t contractAddr = unchainedIndex;
    return doEthCall(contractAddr, manifestHash, "", getLatestBlock_client(), abi);
}

//----------------------------------------------------------------
hash_t getLastManifest(void) {
    return asciiFileToString(configPath("ipfs-hashes/lastHash.txt"));
}

//----------------------------------------------------------------
bool checkOnDisc(CPinnedItem& pin, void* data) {
    pin.onDisc = fileExists(pin.fileName);
    return true;
}

//----------------------------------------------------------------
bool freshenBloomFilters(bool download) {
    string_q cur = getCurrentManifest();
    string_q prev = getLastManifest();
    if (cur != prev) {
        LOG_INFO("Manifest needs to be updated. Previous [", prev, "] Current [", cur, "]");
        stringToAsciiFile(configPath("ipfs-hashes/lastHash.txt"), cur);
        string_q contents = getFileContentsByHash(cur);
        if (contents != "empty file") {
            stringToAsciiFile(configPath("ipfs-hashes/pin-manifest.json"), contents);
            pinList.clear();
        }
    } else {
        LOG_INFO("Manifest is up to data at: ", cur);
    }

    readManifest();
    if (download) {
        forEveryPin(checkOnDisc, NULL);
    }

    return true;
}

//----------------------------------------------------------------
bool publishManifest(ostream& os) {
    pinReport.fileName = "pin-manifest.json";
    pinReport.indexFormat = hashToIndexFile;
    pinReport.bloomFormat = hashToBloomFilterFile;
    pinReport.prevHash = "";  //(prevHash == "" ? hashToEmptyFile : prevHash);

    forEveryPin(addNewPin, &pinReport);

    // pinReport.toJson(os);
    LOG_INFO(bRed, "  Pinned manifest and posted it to Ethereum address: ", unchainedIndex, cOff);

    return true;
}

//----------------------------------------------------------------
bool pinChunk(const string_q& fileName, CPinnedItem& item) {
    if (!readManifest()) {
        return false;
    }

    // If already pinned, no reason to pin it again...
    CPinnedItem copy;
    if (findChunk(fileName, copy)) {
        LOG_WARN("Pin for blocks ", fileName, " already exists.");
        item = copy;
        return true;
    }

    item.fileName = fileName;
    string_q indexStr = pinOneFile(fileName, "finalized");
    if (!contains(indexStr, "IpfsHash")) {
        LOG_ERR("Could not pin index for blocks ", fileName, " file to Pinata. Quitting...");
        return false;
    }

    cleanPinataStr(indexStr);
    CPinataPin index;
    index.parseJson3(indexStr);
    item.indexHash = index.ipfs_pin_hash;
    LOG_INFO(bRed, "  Pinned index to blocks ", substitute(fileName, "-", " "), " to: ", item.indexHash, cOff);

    string_q bloomStr = pinOneFile(fileName, "blooms");
    if (!contains(bloomStr, "IpfsHash")) {
        LOG_ERR("Could not pin bloom for blocks ", fileName, " file to Pinata. Quitting...");
        return false;
    }

    cleanPinataStr(bloomStr);
    CPinataPin bloom;
    bloom.parseJson3(bloomStr);
    item.bloomHash = bloom.ipfs_pin_hash;
    LOG_INFO(bRed, "  Pinned bloom for index to blocks ", substitute(fileName, "-", " "), " to: ", item.bloomHash,
             cOff);

    // add it to the array
    pinList.push_back(item);

    // write the array (after sorting it) to the database
    sort(pinList.begin(), pinList.end());
    return writeManifest(pinList);
}

//---------------------------------------------------------------------------
bool unpinChunkByHash(const hash_t& hash) {
    unpinOneFile(hash);
    usleep(1000000);
    return true;
}

//---------------------------------------------------------------------------
bool unpinChunk(const string_q& fileName, CPinnedItem& item) {
    if (!readManifest())
        return false;

    // If we don't think it's pinned, Pinata may, so proceed even if not found
    CPinnedItem copy;
    if (!findChunk(fileName, copy)) {
        item = copy;
        item.fileName = fileName;
        // return true;
    }

    CPinnedItemArray array;
    for (auto pin : pinList) {
        if (pin.fileName == fileName) {
            cout << "Unpinning: " << pin.fileName << endl;
            unpinOneFile(pin.indexHash);
            unpinOneFile(pin.bloomHash);
            item = pin;
        } else {
            cout << "Keeping " << pin.fileName << "\r";
            cout.flush();
            array.push_back(pin);
        }
    }
    cout << endl;

    pinList.clear();
    pinList = array;
    sort(pinList.begin(), pinList.end());
    return writeManifest(pinList);
}

//----------------------------------------------------------------
bool removeFromPinata(CPinnedItem& item, void* data) {
    cout << item << endl;
    unpinChunk(item.fileName, item);
    return true;
}

//---------------------------------------------------------------------------
bool findChunk(const string_q& fileName, CPinnedItem& item) {
    if (!readManifest())
        return false;

    CPinnedItem search;
    search.fileName = fileName;
    const vector<CPinnedItem>::iterator it = find(pinList.begin(), pinList.end(), search);
    if (it != pinList.end()) {
        item = *it;
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------
bool getChunkByHash(const string_q& fileName, CPinnedItem& item) {
    if (!readManifest(true))
        return false;

    // If we don't think it's pinned, Pinata may, so proceed even if not found
    if (!findChunk(fileName, item)) {
        // return true;
    }

    // clang-format off
    string_q fn = fileName + ".bin";
    getFileByHash(item.indexHash, fn + ".gz");

    string_q cmd = "mv " + getCachePath("tmp/" + fn) + " " + indexFolder_finalized + fn;
    if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    return true;
}

//-------------------------------------------------------------------------
bool ipfsExists(void) {
    ostringstream os;
    os << "ipfs --help";
    return (system(os.str().c_str()) == 0);
}

//-------------------------------------------------------------------------
static size_t curlCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string_q part;
    part.reserve(size * nmemb + 1);
    char* s = (char*)part.c_str();  // NOLINT
    strncpy(s, ptr, size * nmemb);
    s[size * nmemb] = '\0';
    string_q* str = (string_q*)userdata;
    ASSERT(str);
    *str += s;
    return size * nmemb;
}

//----------------------------------------------------------------
bool getPinataKeys(string_q& apiKey, string_q& secret) {
    apiKey = getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_api_key", "<notset>");
    if (apiKey == "<notset>")
        return false;

    secret = getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_secret_api_key", "<notset>");
    if (secret == "<notset>")
        return false;

    return true;
}

//----------------------------------------------------------------
static string_q pinOneFile(const string_q& fileName, const string_q& type) {
#if 0
    LOG4("Starting pin");

    string_q apiKey, secret;
    if (!getPinataKeys(apiKey, secret)) {
        cerr << "You need to put Pinata API keys in ~/.quickBlocks/blockScrape.toml" << endl;
        return "";
    }

    string_q fn = fileName + (type == "blooms" ? ".bloom" : ".bin");
    string_q source = getIndexPath(type + "/" + fn);
    string_q zip = source + ".gz";
    // clang-format off
    string_q cmd1 = "yes | gzip --keep " + source + " 2>/dev/null";
    if (system(cmd1.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    string_q result;
    CURL* curl;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/pinning/pinFileToIPFS");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("pinata_api_key: " + apiKey).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + secret).c_str());
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
        if (res != 0) {
            result += curl_easy_strerror(res);
        }
        curl_mime_free(mime);
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);

    // clang-format off
    string_q cmd2 = "yes | rm -f " + zip;
    if (system(cmd1.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    // LOG4("Finishing pin: ", result);
    LOG4("Finishing pin");
#else
    string_q result;
#endif
    return result;
}

//----------------------------------------------------------------
static string_q unpinOneFile(const string_q& hash) {
    LOG4("Starting unpin");

    string_q apiKey, secret;
    if (!getPinataKeys(apiKey, secret)) {
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
        headers = curl_slist_append(headers, "pinata_api_key: 4586b0cfbb404d87be6e");
        headers = curl_slist_append(
            headers, "pinata_secret_api_key: 9989d172ecb411ed57017501105ee2f8da832330470ae03cdc76157d165fc858");
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

//---------------------------------------------------------------------------
bool listPins(string& result) {
    LOG4("Starting list");

    string_q apiKey, secret;
    if (!getPinataKeys(apiKey, secret)) {
        cerr << "You need to put Pinata API keys in ~/.quickBlocks/blockScrape.toml" << endl;
        return false;
    }

    result.clear();
    CURL* curl;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        // TODO(tjayrush): this has pagination, I just didn't notice it
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/data/pinList?status=pinned");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "pinata_api_key: 4586b0cfbb404d87be6e");
        headers = curl_slist_append(
            headers, "pinata_secret_api_key: 9989d172ecb411ed57017501105ee2f8da832330470ae03cdc76157d165fc858");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
        CURLcode res = curl_easy_perform(curl);
        if (res != 0) {
            result += curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    LOG4("Finishing list: ", result);
    return !result.empty();
}

//----------------------------------------------------------------
static void cleanPinataStr(string_q& in) {
    replace(in, "IpfsHash", "ipfs_pin_hash");
    replace(in, "PinSize", "size");
    replace(in, "Timestamp", "date_pinned");
}

//---------------------------------------------------------------------------
static bool writeManifest(const CPinnedItemArray& array) {
    lockSection(true);  // disallow control+C until we write both files

    string_q binFile = getCachePath("tmp/pins.bin");
    establishFolder(binFile);
    CArchive pinFile(WRITING_ARCHIVE);
    if (!pinFile.Lock(binFile, modeWriteCreate, LOCK_WAIT)) {
        cerr << "Could not lock pin file for writing. Quitting..." << endl;
        return false;
    }
    pinFile << array;
    pinFile.Release();

    lockSection(false);  // enable control+C

    return true;
}

//---------------------------------------------------------------------------
static bool readManifest(bool required) {
    if (!pinList.empty())
        return true;

    string_q binFile = getCachePath("tmp/pins.bin");
    string_q textFile = configPath("ipfs-hashes/pin-manifest.json");

    time_q binDate = fileLastModifyDate(binFile);
    time_q textDate = fileLastModifyDate(textFile);

    // LOG_INFO("binDate: ", binDate.Format(FMT_JSON));
    // LOG_INFO("textDate: ", textDate.Format(FMT_JSON));

    if (binDate > textDate && fileExists(binFile)) {
        CArchive pinFile(READING_ARCHIVE);
        if (!pinFile.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_ERR("Could not open pin file for reading. Quitting...");
            return false;
        }
        pinFile >> pinList;
        pinFile.Release();

    } else if (!fileExists(textFile)) {
        if (required) {
            LOG_ERR("Pins file is required, but not found. Quitting...");
            return false;
        }
        return true;

    } else {
        pinList.clear();  // redundant, but fine
        string_q contents = asciiFileToString(textFile);
        CPinReport report;
        report.parseJson3(contents);
        for (auto pin : report.prevPins)
            pinList.push_back(pin);
        for (auto pin : report.newPins)
            pinList.push_back(pin);
        LOG4("Done Loading pins");
        sort(pinList.begin(), pinList.end());
        writeManifest(pinList);
    }
    return true;
}

//--------------------------------------------------------------------------------
void loadPinMaps(CIndexHashMap& bloomMap, CIndexHashMap& indexMap) {
    if (!readManifest())
        return;
    for (auto pin : pinList) {
        blknum_t num = str_2_Uint(pin.fileName);
        bloomMap[num] = pin.bloomHash;
        indexMap[num] = pin.indexHash;
    }
}

}  // namespace qblocks
