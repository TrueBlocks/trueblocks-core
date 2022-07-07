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
#include "bloom.h"

namespace qblocks {

#define BLOOM_WIDTH_IN_BYTES (1048576 / 8)
#define BLOOM_WIDTH_IN_BITS (BLOOM_WIDTH_IN_BYTES * 8)
#define MAX_ADDRS_IN_BLOOM 50000
#define BLOOM_K 5
//---------------------------------------------------------------------------
void bloom_t::init(void) {
    nInserted = 0;
    bits = new uint8_t[BLOOM_WIDTH_IN_BYTES];
    bzero(bits, BLOOM_WIDTH_IN_BYTES * sizeof(uint8_t));
}

//---------------------------------------------------------------------------
void bloom_t::copy(const bloom_t& b) {
    nInserted = b.nInserted;
    memcpy(bits, b.bits, BLOOM_WIDTH_IN_BYTES);
}

//---------------------------------------------------------------------------
bool bloom_t::operator==(const bloom_t& test) const {
    if (nInserted != test.nInserted)
        return false;
    for (size_t i = 0; i < BLOOM_WIDTH_IN_BYTES; i++) {
        if (bits[i] != test.bits[i])
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------
void bloom_t::lightBit(size_t bit) {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = BLOOM_WIDTH_IN_BYTES - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    bits[index] |= mask;
}

//---------------------------------------------------------------------------
bool isBitLit(size_t bit, uint8_t* bits) {
    size_t whence = (bit % 8);
    uint8_t mask = uint8_t(1 << whence);

    size_t which = (bit / 8);
    size_t index = BLOOM_WIDTH_IN_BYTES - which - 1;

    return (bits[index] & mask);
}

//---------------------------------------------------------------------------
bool bloom_t::isInBloom(const bloom_t& test) const {
    for (size_t b = 0; b < BLOOM_WIDTH_IN_BITS; b++)
        if (isBitLit(b, test.bits) && !isBitLit(b, bits))
            return false;
    return true;
}

//---------------------------------------------------------------------------
bool bloom_t::isInBloom(const CUintArray& bitsLit) const {
    for (auto bit : bitsLit) {
        if (!isBitLit(bit, bits))
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------
void getLitBits(const address_t& addrIn, CUintArray& litBitsOut) {
#define EXTRACT_WID 8
    for (size_t k = 0; k < BLOOM_K; k++) {
        string_q fourByte = ("0x" + extract(addrIn, 2 + (k * EXTRACT_WID), EXTRACT_WID));
        uint64_t bit = (str_2_Uint(fourByte) % BLOOM_WIDTH_IN_BITS);
        litBitsOut.push_back(bit);
    }
    return;
}

}  // namespace qblocks
