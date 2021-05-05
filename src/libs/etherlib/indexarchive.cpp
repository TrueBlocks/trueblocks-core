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
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
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
    Release();
}

//----------------------------------------------------------------
bool CIndexArchive::ReadIndexFromBinary(const string_q& path) {
    if (m_isReading && !fileExists(path))
        return false;
    if (!m_isReading && fileExists(path))
        return false;
    if (!Lock(path, (m_isReading ? modeReadOnly : modeWriteCreate), (m_isReading ? LOCK_NOWAIT : LOCK_WAIT)))
        return false;

    size_t sz = fileSize(path);
    rawData = reinterpret_cast<char*>(malloc(sz + (2 * 59)));
    if (!rawData) {
        LOG_ERR("Could not allocate memory for data.");
        Release();
        return false;
    }

    bzero(rawData, sz + (2 * 59));
    size_t nRead = Read(rawData, sz, sizeof(char));
    if (nRead != sz) {
        LOG_ERR("Could not read entire file.");
        Release();
        return false;
    }

    header = reinterpret_cast<CHeaderRecord_base*>(rawData);
    ASSERT(h->magic == MAGIC_NUMBER);
    ASSERT(bytes_2_Hash(h->hash) == versionHash);
    nAddrs = header->nAddrs;
    nApps = header->nRows;
    addresses = (CAddressRecord_base*)(rawData + sizeof(CHeaderRecord_base));  // NOLINT
    size_t aRecSize = sizeof(CAddressRecord_base);
    size_t sizeOfARecs = aRecSize * nAddrs;
    size_t sizeOfHeader = sizeof(CHeaderRecord_base);
    size_t size = sizeOfHeader + sizeOfARecs;
    appearances = (CAppearance_base*)(rawData + size);  // NOLINT
    Release();
    return true;
}
// EXISTING_CODE
}  // namespace qblocks
