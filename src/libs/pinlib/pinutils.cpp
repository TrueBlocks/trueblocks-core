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
static bool writePins(const CPinnedItemArray& array, bool writeAscii);
static bool readPins(void);

//---------------------------------------------------------------------------
static CPinnedItemArray pins;

//----------------------------------------------------------------
bool pinChunk(const string_q& fileName, CPinnedItem& item) {
    if (!readPins())
        return false;

    // If already pinned, no reason to pin it again...
    CPinnedItem copy;
    if (findChunk(fileName, copy)) {
        item = copy;
        return true;
    }

    item.fileName = fileName;
    string_q indexStr = pinOneFile(fileName, "finalized");
    if (!contains(indexStr, "IpfsHash")) {
        cerr << "Could not pin index file to Pinata. Quitting..." << endl;
        return false;
    }
    cleanPinataStr(indexStr);
    CPinataPin index;
    index.parseJson3(indexStr);
    item.indexHash = index.ipfs_pin_hash;
    item.uploadTs = date_2_Ts(index.date_pinned);

    string_q bloomStr = pinOneFile(fileName, "blooms");
    if (!contains(bloomStr, "IpfsHash")) {
        cerr << "Could not pin bloom file to Pinata. Quitting..." << endl;
        return false;
    }
    cleanPinataStr(bloomStr);
    CPinataPin bloom;
    bloom.parseJson3(bloomStr);
    item.bloomHash = bloom.ipfs_pin_hash;

    // add it to the array
    pins.push_back(item);

    // write the array (after sorting it) to the database
    sort(pins.begin(), pins.end());
    return writePins(pins, true);
}

//---------------------------------------------------------------------------
bool unpinChunkByHash(const string_q& hash) {
    unpinOneFile(hash);
    usleep(3000000);
    return true;
}

//---------------------------------------------------------------------------
bool unpinChunk(const string_q& fileName, CPinnedItem& item) {
    if (!readPins())
        return false;

    // If we don't think it's pinned, Pinata may, so proceed even if not found
    CPinnedItem copy;
    if (!findChunk(fileName, copy)) {
        item = copy;
        item.fileName = fileName;
        // return true;
    }

    CPinnedItemArray array;
    for (auto pin : pins) {
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

    pins.clear();
    pins = array;
    sort(pins.begin(), pins.end());
    return writePins(pins, true);
}

//---------------------------------------------------------------------------
bool findChunk(const string_q& fileName, CPinnedItem& item) {
    if (!readPins())
        return false;

    CPinnedItem search;
    search.fileName = fileName;
    const vector<CPinnedItem>::iterator it = find(pins.begin(), pins.end(), search);
    if (it != pins.end()) {
        item = *it;
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------
bool getChunk(const string_q& fileName, CPinnedItem& item) {
    if (!readPins())
        return false;

    // If we don't think it's pinned, Pinata may, so proceed even if not found
    if (!findChunk(fileName, item)) {
        // return true;
    }

    // clang-format off
    string_q cmd = "curl \"https://ipfs.io/ipfs/" + item.indexHash + "\" --output " + fileName + ".bin.gz ; ";
    if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    cmd = " gunzip *.gz";
    if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    cmd = "mv " + fileName + ".bin " + indexFolder_finalized + fileName + ".bin";
    if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    return true;
}

//-------------------------------------------------------------------------
bool forEveryPin(PINFUNC func, void* data) {
    if (!func)
        return false;
    if (!readPins())
        return false;
    for (auto pin : pins) {
        if (!(*func)(pin, data))
            return false;
    }
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
    string_q cmd1 = "yes | gzip --keep " + source;
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
    LOG4("Finishing unpin: ", result);
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
static bool writePins(const CPinnedItemArray& array, bool writeAscii) {
    string_q textFile = configPath("ipfs-hashes/pins.json");
    string_q binFile = getCachePath("tmp/pins.bin");

    ostringstream os;
    if (writeAscii)
        for (auto pin : pins)
            os << pin.Format(STR_DISPLAY_PINNEDITEM) << endl;

    lockSection(true);  // disallow control+C until we write both files

    if (writeAscii) {
        stringToAsciiFile(textFile, os.str());
        string_q now = Now().Format("%Y%m%d%H%M.00");
        string_q cmd = "touch -mt " + now + " " + textFile;
        cerr << cmd << endl;
        // clang-format off
        if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
    }

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
static bool readPins(void) {
    if (!pins.empty())
        return true;

    string_q binFile = getCachePath("tmp/pins.bin");
    time_q binDate = fileLastModifyDate(binFile);

    string_q textFile = configPath("ipfs-hashes/pins.json");
    time_q textDate = fileLastModifyDate(textFile);

    if (binDate > textDate && fileExists(binFile)) {
        CArchive pinFile(READING_ARCHIVE);
        if (!pinFile.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            cerr << "Could not open pin file for reading. Quitting..." << endl;
            return false;
        }
        pinFile >> pins;
        pinFile.Release();

    } else if (!fileExists(textFile)) {
        return true;

    } else {
        pins.clear();  // redundant, but fine
        string_q contents = asciiFileToString(textFile);
        if (contains(contents, "[")) {
            // slow JSON
            replace(contents, "[", "");
            replace(contents, "]", "");
            CPinnedItem pin;
            while (pin.parseJson3(contents)) {
                LOG4("Loading pin: ", pin.fileName, "\r");
                pins.push_back(pin);
                pin = CPinnedItem();
            }
        } else {
            CStringArray fields;
            string_q fieldStr = substitute(substitute(toLower(STR_DISPLAY_PINNEDITEM), "[{", ""), "}]", "");
            explode(fields, fieldStr, '\t');
            CStringArray lines;
            explode(lines, contents, '\n');
            CPinnedItem pin;
            for (auto line : lines) {
                pin.parseText(fields, line);
                pins.push_back(pin);
                pin = CPinnedItem();
            }
        }
        LOG4("Done lLoading pins");
        sort(pins.begin(), pins.end());
        writePins(pins, false);
    }
    return true;
}

//--------------------------------------------------------------------------------
void loadPins(CIndexHashMap& bloomMap, CIndexHashMap& indexMap) {
    if (!readPins())
        return;
    for (auto pin : pins) {
        blknum_t num = str_2_Uint(pin.fileName);
        bloomMap[num] = pin.bloomHash;
        indexMap[num] = pin.indexHash;
    }
}

}  // namespace qblocks
