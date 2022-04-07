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
    addresses2 = nullptr;
    appearances2 = nullptr;
    rawData = nullptr;
}

//----------------------------------------------------------------
CIndexArchive::~CIndexArchive(void) {
    clean();
}

//----------------------------------------------------------------
void CIndexArchive::clean(void) {
    if (rawData) {
        delete[] rawData;
        rawData = nullptr;
        header = CIndexHeader();
        addresses2 = nullptr;
        appearances2 = nullptr;
    }
    Release();
}

//----------------------------------------------------------------
bool CIndexArchive::ReadIndexFromBinary(const string_q& path, indexpart_t parts) {
    if (!m_isReading)
        return false;
    if (!fileExists(path))
        return false;
    if (parts == IP_NONE)
        return false;

    if (!Lock(path, modeReadOnly, LOCK_NOWAIT))
        return false;

    size_t readSize = sizeof(CIndexHeader);
    size_t memorySize = readSize;
    if ((parts & IP_ADDRS) || (parts & IP_APPS)) {
        readSize = fileSize(path);
        memorySize = readSize + (2 * asciiAppearanceSize);  // a bit of extra room for some reason...
    }

    rawData = reinterpret_cast<char*>(malloc(memorySize));
    if (!rawData) {
        LOG_ERR("Could not allocate memory for data.");
        Release();
        return false;
    }
    bzero(rawData, memorySize);

    size_t nRead = Read(rawData, readSize, sizeof(char));
    if (nRead != readSize) {
        LOG_ERR("Could not read entire file.");
        clean();
        return false;
    }

    header = *(reinterpret_cast<CIndexHeader*>(rawData));
    ASSERT(header.magic == MAGIC_NUMBER);
    ASSERT(bytes_2_Hash(header.hash) == versionHash);

    size_t startOfAddrTable = sizeof(CIndexHeader);
    size_t addrTableSize = sizeof(CIndexedAddress) * header.nAddrs;
    size_t startOfAppsTable = startOfAddrTable + addrTableSize;

    if (readSize == sizeof(CIndexHeader)) {
        addresses2 = nullptr;
        appearances2 = nullptr;
    } else {
        addresses2 = (CIndexedAddress*)(rawData + startOfAddrTable);       // NOLINT
        appearances2 = (CIndexedAppearance*)(rawData + startOfAppsTable);  // NOLINT
    }
    if (!(parts & IP_LEAVEOPEN)) {
        Release();
    }
    return true;
}

//----------------------------------------------------------------
bool visitOneChunk(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitOneChunk, data);

    } else {
        CIndexChunkVisitor* v = (CIndexChunkVisitor*)data;
        if (!v->indexFunc)
            return false;

        v->range.first = path_2_Bn(path, v->range.second);

        CIndexArchive index(READING_ARCHIVE);  // Releases when it goes out of scope
        if (!index.ReadIndexFromBinary(path, v->parts))
            return false;

        if (!(*v->indexFunc)(index, v))
            return false;
    }

    return true;
}

//----------------------------------------------------------------
bool forEveryIndexChunk(INDEXCHUNKFUNC func, void* data) {
    if (!func)
        return false;

    CIndexChunkVisitor v;
    v.indexFunc = func;
    v.callData = data;
    return forEveryFileInFolder(indexFolder_finalized, visitOneChunk, &v);
}
}  // namespace qblocks
