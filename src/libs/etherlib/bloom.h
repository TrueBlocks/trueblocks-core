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

namespace qblocks {

//---------------------------------------------------------------------------
class bloom_t {
  public:
    uint32_t nInserted;
    uint8_t* bits;

  public:
    bloom_t(void);
    ~bloom_t(void);
    bloom_t(const bloom_t& b);
    bloom_t& operator=(const bloom_t& b);
    bool operator==(const bloom_t& item) const;
    bool operator!=(const bloom_t& item) const {
        return !operator==(item);
    }

    void lightBit(size_t bit);
    void unlightBit(size_t bit);
    size_t nBitsHit(void) const;

    void showBloom(ostream& os, size_t first, size_t cnt) const;
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
extern bloom_t addr_2_Bloom(const address_t& addrIn, CUintArray& litBits);

//---------------------------------------------------------------------------
inline bloom_t addr_2_Bloom(const address_t& addrIn) {
    CUintArray litBits;
    return addr_2_Bloom(addrIn, litBits);
}

//----------------------------------------------------------------------
extern bool addToSet(CBloomArray& blooms, const address_t& addr);
extern bool isMember(const CBloomArray& blooms, const bloom_t& bloom);
extern bool writeBloomToBinary(const string_q& fileName, const CBloomArray& blooms);
extern bool readBloomFromBinary(const string_q& fileName, CBloomArray& blooms);
extern size_t getBloomWidthInBytes(void);
extern size_t getBloomWidthInBits(void);
extern size_t getMaxAddrsInBloom(void);
extern size_t getNibbleWid(void);
extern size_t getK(void);

}  // namespace qblocks
