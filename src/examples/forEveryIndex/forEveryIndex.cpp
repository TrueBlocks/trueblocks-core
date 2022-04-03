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
#include "etherlib.h"

extern void alterAddress(uint8_t bytes[20], int inc);
//----------------------------------------------------------------
bool visitIndexChunk(CIndexArchive& chunk, void* data) {
    CIndexChunkVisitor* v = (CIndexChunkVisitor*)data;

    string_q indexPath = chunk.getFilename();
    string_q bloomPath = substitute(substitute(indexPath, ".bin", ".bloom"), "finalized", "blooms");

    CBloomFilter bloomFilter;
    if (!bloomFilter.readBloomFilter(bloomPath, true)) {
        LOG_WARN("Could not read bloom filter ", bloomPath);
        return false;
    }

    CIndexedAddress* addrs = new CIndexedAddress[chunk.header.nAddrs];
    if (!addrs) {
        LOG_WARN("Could not allocation memory to read addresses from ", indexPath);
        return false;
    }

    chunk.Seek(sizeof(CIndexHeader), SEEK_SET);
    size_t nRead = chunk.Read(addrs, sizeof(CIndexedAddress) * chunk.header.nAddrs, sizeof(char));
    if (nRead != (sizeof(CIndexedAddress) * chunk.header.nAddrs)) {
        LOG_WARN("Could not read addresses from ", indexPath);
        return false;
    }

    CBloomFilter testBloom;
    size_t missed = 0, nFp = 0;  // number of false positives
    for (size_t i = 0; i < chunk.header.nAddrs; i++) {
        bool hit = bloomFilter.isMemberOf(addrs[i].bytes);
        if (!hit) {
            LOG_WARN("Address should be in bloom: ", bytes_2_Addr(addrs[i].bytes));
            missed++;
        } else {
            if (!(i % 57)) {
                LOG_INFO(v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ", missed, "\r");
            }
        }
        testBloom.addToSet(bytes_2_Addr(addrs[i].bytes));

        alterAddress(addrs[i].bytes, i);
        hit = bloomFilter.isMemberOf(addrs[i].bytes);
        if (hit) {
            nFp++;
        } else {
            if (!(i % 57)) {
                LOG_INFO("Pass 1 ", v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ", nFp,
                         "\r");
            }
        }
    }

    if (testBloom != bloomFilter) {
        LOG_WARN("Bloom created is different from the bloom read from disc ", bloomPath);
    }

    chunk.Seek(sizeof(CIndexHeader), SEEK_SET);
    nRead = chunk.Read(addrs, sizeof(CIndexedAddress) * chunk.header.nAddrs, sizeof(char));
    if (nRead != (sizeof(CIndexedAddress) * chunk.header.nAddrs)) {
        LOG_WARN("Could not read addresses from ", indexPath);
        return false;
    }
    size_t missed2 = 0;  // number of false positives
    for (size_t i = 0; i < chunk.header.nAddrs; i++) {
        bool hit = testBloom.isMemberOf(addrs[i].bytes);
        if (!hit) {
            LOG_WARN("Address should be in bloom: ", bytes_2_Addr(addrs[i].bytes));
            missed2++;
        } else {
            if (!(i % 57)) {
                LOG_INFO("Pass 2 ", v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ",
                         missed2, "    \r");
            }
        }
    }

    blkrange_t* overall = (blkrange_t*)v->callData;
    overall->first += nFp;
    overall->second += chunk.header.nAddrs;

    double pct = (double(nFp) / double(chunk.header.nAddrs)) * 100.;
    double oPct = (double(overall->first) / double(overall->second)) * 100.;

    cout << "range: " << padNum9(v->range.first) << "-" << padNum9(v->range.second);
    cout << " tested: " << padNum7T(uint64_t(chunk.header.nAddrs));
    cout << " missed: " << missed << " fp: " << padNum4T(uint64_t(nFp));
    cout << " pct: " << double_2_Str(pct, 2) << "%";
    cout << " overall: " << double_2_Str(oPct, 2) << "%" << endl;

    delete[] addrs;
    return true;
}

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    loadEnvironmentPaths("mainnet", "/Users/jrush/Development/trueblocks-core/build/shit/unchained/", "");

    etherlib_init(quickQuitHandler);

    blkrange_t overall;
    overall.first = 218419;
    overall.second = 26844586;
    forEveryIndexChunk(visitIndexChunk, &overall);

    etherlib_cleanup();

    return 0;
}

//----------------------------------------------------------------
void alterAddress(uint8_t bytes[20], int inc) {
    for (size_t i = 0; i < 20; i++) {
        bytes[i] += uint8_t(i);
    }
}
