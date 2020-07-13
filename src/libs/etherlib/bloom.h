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
#pragma once

#include "utillib.h"

namespace qblocks {

#define MAX_INSERTS 50000
#define N_BYTES (1048576 / 8)

//---------------------------------------------------------------------------
class bloom_t {
  public:
    static size_t BYTE_SIZE;
    static size_t BIT_SIZE(void) {
        return BYTE_SIZE * 8;
    }

  public:
    uint64_t nInserted;
    uint8_t* bits;

  public:
    bloom_t(void);
    ~bloom_t(void);
    bloom_t(const bloom_t& b);
    bloom_t& operator=(const bloom_t& b);

    void lightBit(size_t bit);
    void unlightBit(size_t bit);
    size_t nBitsHit(void) const;

    void showBloom(ostream& os) const;
    bool isBitLit(size_t bit) const;
    void toggleBit(size_t bit);
    bool isInBloom(const bloom_t& test) const;

  private:
    void copy(const bloom_t& b);
    void init(void);
};
typedef vector<bloom_t> CBloomArray;

//---------------------------------------------------------------------------
inline bloom_t::bloom_t(void) {
    init();
}

//---------------------------------------------------------------------------
inline bloom_t::~bloom_t(void) {
    if (bits)
        delete[] bits;
}

//---------------------------------------------------------------------------
inline bloom_t::bloom_t(const bloom_t& b) {
    init();
    copy(b);
}

//---------------------------------------------------------------------------
inline bloom_t& bloom_t::operator=(const bloom_t& b) {
    copy(b);
    return *this;
}

//---------------------------------------------------------------------------
inline void bloom_t::init(void) {
    nInserted = 0;
    bits = new uint8_t[bloom_t::BYTE_SIZE];
    bzero(bits, bloom_t::BYTE_SIZE * sizeof(uint8_t));
}

//---------------------------------------------------------------------------
inline void bloom_t::copy(const bloom_t& b) {
    nInserted = b.nInserted;
    for (size_t i = 0; i < bloom_t::BYTE_SIZE; i++) {
        bits[i] = b.bits[i];
    }
}

//---------------------------------------------------------------------------
inline void bloom_t::showBloom(ostream& os) const {
    for (size_t i = 0; i < bloom_t::BYTE_SIZE; i++)
        os << byte_2_Bits(bits[i]) << " ";
    os << endl;
}

//---------------------------------------------------------------------------
inline void bloom_t::lightBit(size_t bit) {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = bloom_t::BYTE_SIZE - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    bits[index] |= mask;
}

//---------------------------------------------------------------------------
inline void bloom_t::unlightBit(size_t bit) {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = bloom_t::BYTE_SIZE - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    bits[index] &= ~(mask);
}

//---------------------------------------------------------------------------
inline bool bloom_t::isBitLit(size_t bit) const {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = bloom_t::BYTE_SIZE - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    return (bits[index] & mask);
}

//---------------------------------------------------------------------------
inline void bloom_t::toggleBit(size_t bit) {
    if (isBitLit(bit))
        unlightBit(bit);
    else
        lightBit(bit);
}

//---------------------------------------------------------------------------
inline bool bloom_t::isInBloom(const bloom_t& test) const {
    for (size_t b = 0; b < bloom_t::BIT_SIZE(); b++)
        if (test.isBitLit(b) && !isBitLit(b))
            return false;
    return true;
}

//---------------------------------------------------------------------------
extern bloom_t addr_2_Bloom(const address_t& addrIn, CUintArray& litBits);

//---------------------------------------------------------------------------
inline bloom_t addr_2_Bloom(const address_t& addrIn) {
    CUintArray litBits;
    return addr_2_Bloom(addrIn, litBits);
}

//----------------------------------------------------------------------
extern bool addToSet(CBloomArray& blooms, const address_t& addr);
extern bool isMember(const CBloomArray& blooms, const address_t& addr);
extern bool writeBloomToBinary(const string_q& outFile, const CBloomArray& blooms);

}  // namespace qblocks
