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
#include "indexarchive.h"

namespace qblocks {
//----------------------------------------------------------------
CIndexArchive::CIndexArchive(bool mode) : CArchive(mode) {
    rawData = NULL;
    header = NULL;
    addresses = NULL;
    appearances = NULL;
    nAddrs = nApps = 0;
}

//----------------------------------------------------------------
CIndexArchive::~CIndexArchive(void) {
    if (rawData) {
        delete[] rawData;
        header = NULL;
        addresses = NULL;
        appearances = NULL;
        rawData = NULL;
        nAddrs = nApps = 0;
    }
    if (reverseAppMap) {
        delete[] reverseAppMap;
        reverseAddrRanges.clear();
    }
    Release();
}

//----------------------------------------------------------------
bool CIndexArchive::ReadIndexFromBinary(const string_q& path) {
    if (!m_isReading)
        return false;
    if (!fileExists(path))
        return false;
    if (!Lock(path, modeReadOnly, LOCK_NOWAIT))
        return false;

    size_t sz = fileSize(path);
    rawData = reinterpret_cast<char*>(malloc(sz + (2 * asciiAppearanceSize)));
    if (!rawData) {
        LOG_ERR("Could not allocate memory for data.");
        Release();
        return false;
    }

    bzero(rawData, sz + (2 * asciiAppearanceSize));
    size_t nRead = Read(rawData, sz, sizeof(char));
    if (nRead != sz) {
        LOG_ERR("Could not read entire file.");
        Release();
        return false;
    }

    header = reinterpret_cast<CIndexHeader*>(rawData);
    ASSERT(header->magic == MAGIC_NUMBER);
    ASSERT(bytes_2_Hash(header->hash) == versionHash);
    nAddrs = header->nAddrs;
    nApps = header->nRows;
    addresses = (CIndexedAddress*)(rawData + sizeof(CIndexHeader));  // NOLINT
    size_t aRecSize = sizeof(CIndexedAddress);
    size_t sizeOfARecs = aRecSize * nAddrs;
    size_t sizeOfHeader = sizeof(CIndexHeader);
    size_t size = sizeOfHeader + sizeOfARecs;
    appearances = (CIndexedAppearance*)(rawData + size);  // NOLINT
    Release();
    return true;
}

//--------------------------------------------------------------
bool readIndexHeader(const string_q& path, CIndexHeader& header) {
    header.nRows = header.nAddrs = (uint32_t)-1;
    if (contains(path, "blooms")) {
        return false;
    }

    if (endsWith(path, ".txt")) {
        header.nRows = (uint32_t)fileSize(path) / (uint32_t)asciiAppearanceSize;
        CStringArray lines;
        asciiFileToLines(path, lines);
        CAddressBoolMap addrMap;
        for (auto line : lines)
            addrMap[nextTokenClear(line, '\t')] = true;
        header.nAddrs = (uint32_t)addrMap.size();
        return true;
    }

    CArchive archive(READING_ARCHIVE);
    if (!archive.Lock(path, modeReadOnly, LOCK_NOWAIT))
        return false;

    bzero(&header, sizeof(header));
    // size_t nRead =
    archive.Read(&header, sizeof(header), 1);
    // if (false) { //nRead != sizeof(header)) {
    //    cerr << "Could not read file: " << path << endl;
    //    return;
    //}
    ASSERT(header.magic == MAGIC_NUMBER);
    // ASSERT(bytes_2_Hash(h->hash) == versionHash);
    archive.Release();
    return true;
}
}  // namespace qblocks
