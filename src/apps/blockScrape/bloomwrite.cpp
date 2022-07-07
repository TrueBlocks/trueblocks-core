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
#include "bloomwrite.h"

namespace qblocks {

#define BLOOM_WIDTH_IN_BYTES (1048576 / 8)
#define BLOOM_WIDTH_IN_BITS (BLOOM_WIDTH_IN_BYTES * 8)
#define MAX_ADDRS_IN_BLOOM 50000
#define BLOOM_K 5

//----------------------------------------------------------------------
bool CBloomFilterWrite::isMemberOf(const address_t& addr) {
    CUintArray bitsLit;
    getLitBits(addr, bitsLit);
    for (auto bloom : array) {
        if (bloom.isInBloom(bitsLit))
            return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CBloomFilterWrite::isMemberOf(uint8_t const bytes[20]) {
    return isMemberOf(bytes_2_Addr(bytes));
}

//----------------------------------------------------------------------
bool CBloomFilterWrite::addToSet(const address_t& addr) {
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

//----------------------------------------------------------------------
bool CBloomFilterWrite::writeBloomFilter(const string_q& fileName) {
    lockSection();
    CArchive output(WRITING_ARCHIVE);
    if (!output.Lock(fileName, modeWriteCreate, LOCK_NOWAIT)) {
        unlockSection();
        return false;
    }
    output.Write((uint32_t)array.size());
    for (auto bloom : array) {
        output.Write(bloom.nInserted);
        output.Write(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
    }
    output.Release();
    unlockSection();
    return true;
}

ostream& operator<<(ostream& os, const CBloomFilterWrite& bloomFilter) {
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

bool CBloomFilterWrite::operator==(const CBloomFilterWrite& it) const {
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
