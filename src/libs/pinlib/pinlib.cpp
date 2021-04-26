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
#include "classes/pinatalist.h"
#include "classes/pinatapin.h"

namespace qblocks {

extern string_q pinlib_unpinByHash(const string_q& hash);
extern bool parseOneLine(const char* line, void* data);
#define hashToEmptyFile "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm"

//-------------------------------------------------------------------------
void pinlib_init(QUITHANDLER qh) {
    acctlib_init(qh);
    CPinnedChunk::registerClass();
    CPinataPin::registerClass();
    CPinataPinlist::registerClass();
    CPinataMetadata::registerClass();
    CPinataRegion::registerClass();
    CPinManifest::registerClass();
}

//-------------------------------------------------------------------------
void pinlib_cleanup(void) {
    acctlib_cleanup();
}

//----------------------------------------------------------------
bool pinlib_downloadManifest(void) {
    CEthCall call;
    call.address = unchainedIndexAddr;
    call.encoding = manifestHashEncoding;
    call.blockNumber = getBlockProgress(BP_CLIENT).client;
    call.abi_spec.loadAbiFromEtherscan(call.address, false /* raw */);
    if (doEthCall(call)) {
        ipfshash_t ipfshash = call.result.outputs[0].value;
        LOG_INFO("Found manifest hash at ", ipfshash);
        string_q remoteData = doCommand("curl -s \"http://gateway.ipfs.io/ipfs/" + ipfshash + "\"");
        string fn = configPath("manifest/manifest.txt");
        stringToAsciiFile(fn, remoteData);
        return fileExists(fn);
    }
    return false;
}

//---------------------------------------------------------------------------
bool pinlib_readManifest(CPinnedChunkArray& pinArray) {
    if (!pinArray.empty())
        return true;

    string_q binFile = getCachePath("tmp/pins.bin");
    string_q textFile = configPath("manifest/manifest.txt");

    time_q binDate = fileLastModifyDate(binFile);
    time_q textDate = fileLastModifyDate(textFile);

    if (binDate > textDate && fileExists(binFile)) {
        CArchive pinFile(READING_ARCHIVE);
        if (!pinFile.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_ERR("Could not open pin file for reading.");
            return false;
        }
        pinFile >> pinArray;
        pinFile.Release();

    } else if (!fileExists(textFile)) {
        LOG_ERR("Pins file is required, but not found.");
        return false;

    } else {
        pinArray.clear();  // redundant, but fine
        forEveryLineInAsciiFile(textFile, parseOneLine, &pinArray);
        LOG4("Done loading pins");
        sort(pinArray.begin(), pinArray.end());
        if (!isTestMode())
            pinlib_updateManifest(pinArray);
    }
    return true;
}

//---------------------------------------------------------------------------
bool pinlib_updateManifest(CPinnedChunkArray& pList) {
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

extern string_q pinOneChunk(const string_q& fileName, const string_q& type);
//----------------------------------------------------------------
bool pinlib_pinChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item) {
    // If already pinned, no reason to pin it again...
    CPinnedChunk copy;
    if (pinlib_findChunk(pList, fileName, copy)) {
        LOG_WARN("Pin for blocks ", fileName, " already exists.");
        item = copy;
        return true;
    }

    item.fileName = fileName;
    string_q indexStr = pinOneChunk(fileName, "finalized");
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

    string_q bloomStr = pinOneChunk(fileName, "blooms");
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
    return pinlib_updateManifest(pList);
}

//---------------------------------------------------------------------------
bool pinlib_unpinChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item) {
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
            pinlib_unpinByHash(pin.indexHash);
            pinlib_unpinByHash(pin.bloomHash);
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
    return pinlib_updateManifest(pList);
}

//---------------------------------------------------------------------------
bool pinlib_getChunkFromRemote(CPinnedChunk& pin, ipfsdown_t which, double sleep) {
    string_q outFile = "blooms/" + pin.fileName + ".bloom";
    ipfshash_t ipfshash = pin.bloomHash;
    if (which != BLOOM_TYPE) {
        outFile = "finalized/" + pin.fileName + ".bin";
        ipfshash = pin.indexHash;
    }

    if (!fileExists(getIndexPath(outFile))) {
        string_q zipFile = outFile + ".gz";
        if (!fileExists(getIndexPath(zipFile))) {
            // download from ipfs gateway
            ostringstream cmd;
            cmd << "curl --silent -o ";
            cmd << "\"" << getIndexPath(zipFile) << "\" ";
            cmd << "\"http://gateway.ipfs.io/ipfs/" << ipfshash << "\"";
            LOG_INFO(bBlue, "Unchaining bloom ", ipfshash, " to ", pin.fileName, cOff);
            int ret = system(cmd.str().c_str());
            // cerr << "result: " << ret << endl;
            if (ret != 0) {
                // clean up on failure
                if (ret == 2)
                    defaultQuitHandler(-1);  // user hit control+c, let ourselves know
                LOG_WARN("Could not download zip file ", zipFile);
                ::remove(getIndexPath(zipFile).c_str());
            } else {
                usleep((useconds_t)(sleep * 1000000));  // do not remove cruft - stays responsive to control+C
            }
        }

        if (fileExists(getIndexPath(zipFile))) {
            ostringstream cmd;
            cmd << "cd " << getIndexPath("") << " && gunzip --keep " << zipFile;
            int ret = system(cmd.str().c_str());
            // cerr << "result: " << ret << endl;
            if (ret != 0) {
                // clean up on failure
                if (ret == 2)
                    defaultQuitHandler(-1);  // user hit control+c, let ourselves know
                LOG_WARN("Could not download file ", outFile);
                ::remove(getIndexPath(zipFile).c_str());
                ::remove(getIndexPath(outFile).c_str());
            }
        } else {
            LOG_INFO("File ", outFile, " does not exist.");
        }

    } else {
        LOG_INFO("File ", outFile, " exists.");
    }
    return true;
}

//---------------------------------------------------------------------------
bool pinlib_findChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item) {
    if (!pinlib_readManifest(pList))
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

    if (!pinlib_readManifest(pList))
        return false;

    for (auto pin : pList) {
        if (!(*func)(pin, data))
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool parseOneLine(const char* line, void* data) {
    if (isTestMode() && line > string_q("005000000"))
        return true;

    CPinnedChunkArray* pins = (CPinnedChunkArray*)data;
    static CStringArray fields;
    if (fields.empty()) {
        string_q flds = "fileName,bloomHash,indexHash";
        explode(fields, flds, ',');
    }

    CPinnedChunk pin;
    string_q ln(line);
    pin.parseCSV(fields, ln);
    pins->push_back(pin);
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
string_q pinOneChunk(const string_q& fileName, const string_q& type) {
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
        CApiKey lic;
        if (!getApiKey(lic)) {
            cerr << "You need to put Pinata API keys in ~/.quickBlocks/blockScrape.toml" << endl;
            return "";
        }
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
    return result;
}

//----------------------------------------------------------------
string_q pinlib_unpinByHash(const string_q& hash) {
    CApiKey lic;
    if (!getApiKey(lic)) {
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
        headers = curl_slist_append(headers, ("pinata_api_key: " + lic.key).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + lic.secret).c_str());
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

}  // namespace qblocks
