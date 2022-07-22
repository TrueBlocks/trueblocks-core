package main

import (
	"fmt"
	"io/fs"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
)

func main() {
	// Figure out where the finalized index chunks are...
	path := config.GetPathToIndex("mainnet")

	// Walk all the finalized chunks...
	filepath.Walk(path+"finalized/", func(indexPath string, info fs.FileInfo, err error) error {
		// We only want to view the index chunks...
		if strings.HasSuffix(indexPath, ".bin") {
			c, _ := index.NewChunk(indexPath)
			fmt.Println(c)
			fmt.Println("---------------------------------------------------------")
			fmt.Println()

			cd, _ := index.NewChunkData(indexPath)
			fmt.Println(cd)
			fmt.Println("---------------------------------------------------------")
			fmt.Println()

			b, _ := bloom.NewChunkBloom(bloom.ToBloomPath(indexPath))
			fmt.Println(b)
			fmt.Println("---------------------------------------------------------")
			fmt.Println()
		}
		return nil
	})
}

// /*-------------------------------------------------------------------------------------------
//  * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
//  * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
//  *
//  * This program is free software: you may redistribute it and/or modify it under the terms
//  * of the GNU General Public License as published by the Free Software Foundation, either
//  * version 3 of the License, or (at your option) any later version. This program is
//  * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//  * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//  * General Public License for more details. You should have received a copy of the GNU General
//  * Public License along with this program. If not, see http://www.gnu.org/licenses/.
//  *-------------------------------------------------------------------------------------------*/
// #include "etherlib.h"
// #include "bloomread.h"

// extern void alterAddress(uint8_t bytes[20], int inc);
// //----------------------------------------------------------------
// bool visitIndexChunk(CIndexArchive& chunk, void* data) {
//     CIndexChunkVisitor* v = (CIndexChunkVisitor*)data;

//     string_q indexPath = chunk.getFilename();
//     string_q bloomPath = substitute(substitute(indexPath, ".bin", ".bloom"), "finalized", "blooms");

//     CBloomFilterRead bloomFilter;
//     if (!bloomFilter.readBloomFilter(bloomPath, true)) {
//         LOG_WARN("Could not read bloom filter ", bloomPath);
//         return false;
//     }

//     CIndexedAddress* addrs = new CIndexedAddress[chunk.header.nAddrs];
//     if (!addrs) {
//         LOG_WARN("Could not allocation memory to read addresses from ", indexPath);
//         return false;
//     }

//     chunk.Seek(sizeof(CIndexHeader), SEEK_SET);
//     size_t nRead = chunk.Read(addrs, sizeof(CIndexedAddress) * chunk.header.nAddrs, sizeof(char));
//     if (nRead != (sizeof(CIndexedAddress) * chunk.header.nAddrs)) {
//         LOG_WARN("Could not read addresses from ", indexPath);
//         return false;
//     }

//     CBloomFilterRead testBloom;
//     size_t missed = 0, nFp = 0;  // number of false positives
//     for (size_t i = 0; i < chunk.header.nAddrs; i++) {
//         bool hit = bloomFilter.IsMemberOf(addrs[i].bytes);
//         if (!hit) {
//             LOG_WARN("Address should be in bloom: ", bytes_2_Addr(addrs[i].bytes));
//             missed++;
//         } else {
//             if (!(i % 57)) {
//                 LOG_INFO(v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ", missed, "\r");
//             }
//         }
//         testBloom.addToSet(bytes_2_Addr(addrs[i].bytes));

//         alterAddress(addrs[i].bytes, int(i));
//         hit = bloomFilter.IsMemberOf(addrs[i].bytes);
//         if (hit) {
//             nFp++;
//         } else {
//             if (!(i % 57)) {
//                 LOG_INFO("Pass 1 ", v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ", nFp,
//                          "\r");
//             }
//         }
//     }

//     if (testBloom != bloomFilter) {
//         LOG_WARN("Bloom created is different from the bloom read from disc ", bloomPath);
//     }

//     chunk.Seek(sizeof(CIndexHeader), SEEK_SET);
//     nRead = chunk.Read(addrs, sizeof(CIndexedAddress) * chunk.header.nAddrs, sizeof(char));
//     if (nRead != (sizeof(CIndexedAddress) * chunk.header.nAddrs)) {
//         LOG_WARN("Could not read addresses from ", indexPath);
//         return false;
//     }
//     size_t missed2 = 0;  // number of false positives
//     for (size_t i = 0; i < chunk.header.nAddrs; i++) {
//         bool hit = testBloom.IsMemberOf(addrs[i].bytes);
//         if (!hit) {
//             LOG_WARN("Address should be in bloom: ", bytes_2_Addr(addrs[i].bytes));
//             missed2++;
//         } else {
//             if (!(i % 57)) {
//                 LOG_INFO("Pass 2 ", v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ",
//                          missed2, "    \r");
//             }
//         }
//     }

//     blkrange_t* overall = (blkrange_t*)v->callData;
//     overall->first += nFp;
//     overall->second += chunk.header.nAddrs;

//     double pct = (double(nFp) / double(chunk.header.nAddrs)) * 100.;
//     double oPct = (double(overall->first) / double(overall->second)) * 100.;

//     cout << "range: " << padNum9(v->range.first) << "-" << padNum9(v->range.second);
//     cout << " tested: " << padNum7T(uint64_t(chunk.header.nAddrs));
//     cout << " missed: " << missed << " fp: " << padNum4T(uint64_t(nFp));
//     cout << " pct: " << double_2_Str(pct, 2) << "%";
//     cout << " overall: " << double_2_Str(oPct, 2) << "%" << endl;

//     delete[] addrs;
//     return true;
// }

// //----------------------------------------------------------------
// int main(int argc, const char* argv[]) {
//     loadEnvironmentPaths("mainnet", "/Users/jrush/Development/trueblocks-core/build/shit/unchained/", "");

//     etherlib_init(quickQuitHandler);

//     blkrange_t overall;
//     overall.first = 218419;
//     overall.second = 26844586;
//     forEveryIndexChunk(visitIndexChunk, &overall);

//     etherlib_cleanup();

//     return 0;
// }

// //----------------------------------------------------------------
// void alterAddress(uint8_t bytes[20], int inc) {
//     for (size_t i = 0; i < 20; i++) {
//         bytes[i] += uint8_t(i);
//     }
// }


/*
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
#include "bloomread.h"

namespace qblocks {

#define BLOOM_WIDTH_IN_BYTES (1048576 / 8)
#define BLOOM_WIDTH_IN_BITS (BLOOM_WIDTH_IN_BYTES * 8)
#define MAX_ADDRS_IN_BLOOM 50000
#define BLOOM_K 5

//----------------------------------------------------------------------
bool CBloomFilterRead::isMemberOf(const address_t& addr) {
    CUintArray bitsLit;
    getLitBits(addr, bitsLit);
    for (auto bloom : array) {
        if (bloom.isInBloom(bitsLit))
            return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CBloomFilterRead::isMemberOf(uint8_t const bytes[20]) {
    return isMemberOf(bytes_2_Addr(bytes));
}

//----------------------------------------------------------------------
bool CBloomFilterRead::addToSet(const address_t& addr) {
    if (array.size() == 0) {
        array.push_back(bloom_t());  // so we have something to add to
    }

    CUintArray bitsLit;
    getLitBits(addr, bitsLit);
    for (auto bit : bitsLit) {
        array[array.size() - 1].lightBit(bit);
    }
    array[array.size() - 1].nInserted++;

    if (array[array.size() - 1].nInserted > MAX_ADDRS_IN_BLOOM)
        array.push_back(bloom_t());

    return true;
}

//----------------------------------------------------------------------------------
bool CBloomFilterRead::readBloomFilter(const string_q& fileName, bool readBits) {
    array.clear();
    CArchive bloomCache(READING_ARCHIVE);
    if (bloomCache.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        uint32_t nBlooms;
        bloomCache.Read(nBlooms);
        for (size_t i = 0; i < nBlooms; i++) {
            bloom_t bloom;
            bloomCache.Read(bloom.nInserted);
            if (readBits) {
                bloomCache.Read(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
            } else {
                bloomCache.Seek(BLOOM_WIDTH_IN_BYTES, SEEK_CUR);
            }
            array.push_back(bloom);
        }
        bloomCache.Close();
        return true;
    }
    return false;
}

ostream& operator<<(ostream& os, const CBloomFilterRead& bloomFilter) {
    CUintArray bitsLit;
    uint64_t nBlooms, nInserted, nBitsLit, nBitsNotLit, sz;
    nBlooms = nInserted = nBitsLit = nBitsNotLit = sz = 0;
    nBlooms = bloomFilter.array.size();
    for (const auto& b : bloomFilter.array) {
        nInserted += b.nInserted;
        for (size_t i = 0; i < BLOOM_WIDTH_IN_BITS; i++) {
            if (isBitLit(i, b.bits)) {
                nBitsLit++;
                bitsLit.push_back(i);
            } else {
                nBitsNotLit++;
            }
        }
    }
    os << "nBlooms:     " << nBlooms << " nInserted:   " << nInserted << " nBitsLit:    " << nBitsLit
       << " nBitsNotLit: " << nBitsNotLit << endl;
    os << "bitsLit:" << endl;
    for (auto b : bitsLit) {
        os << b << ",";
    }
    return os;
}

bool CBloomFilterRead::operator==(const CBloomFilterRead& it) const {
    if (array.size() != it.array.size()) {
        cerr << endl << "Differs in size " << array.size() << "," << it.array.size() << endl;
        getchar();
        return false;
    }
    for (size_t i = 0; i < array.size(); i++) {
        for (size_t j = 0; j < BLOOM_WIDTH_IN_BYTES; j++) {
            if (array[i].bits[j] != it.array[i].bits[j]) {
                cerr << endl;
                cerr << "Differs at ";
                cerr << array[i].bits[j];
                cerr << " ";
                cerr << it.array[i].bits[j];
                cerr << endl;
                getchar();
                return false;
            }
        }
    }
    return true;
}

}  // namespace qblocks
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
#pragma once

#include "utillib.h"
#include "bloom.h"

namespace qblocks {

//---------------------------------------------------------------------------
class CBloomFilterRead {
    typedef vector<bloom_t> CBloomArray;

  public:
    CBloomArray array;
    bool readBloomFilter(const string_q& fileName, bool readBits);
    bool addToSet(const address_t& addr);
    bool isMemberOf(uint8_t const bytes[20]);
    bool isMemberOf(const address_t& addr);
    bool operator==(const CBloomFilterRead& it) const;
    bool operator!=(const CBloomFilterRead& it) const {
        return !operator==(it);
    }

    friend ostream& operator<<(ostream& os, const CBloomFilterRead& bloomFilter);
};

}  // namespace qblocks
*/