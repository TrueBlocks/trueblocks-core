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

//----------------------------------------------------------------------
#define BLOOM_WIDTH_IN_BYTES (1048576 / 8)
#define BLOOM_WIDTH_IN_BITS (BLOOM_WIDTH_IN_BYTES * 8)
#define MAX_ADDRS_IN_BLOOM 50000
#define NIBBLE_WID 8
#define K 5

//---------------------------------------------------------------------------
void bloom_t::init(void) {
    nInserted = 0;
    bits = new uint8_t[BLOOM_WIDTH_IN_BYTES];
    bzero(bits, BLOOM_WIDTH_IN_BYTES * sizeof(uint8_t));
}

//---------------------------------------------------------------------------
void bloom_t::copy(const bloom_t& b) {
    nInserted = b.nInserted;
    for (size_t i = 0; i < BLOOM_WIDTH_IN_BYTES; i++) {
        bits[i] = b.bits[i];
    }
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
void bloom_t::showBloom(ostream& os) const {
    for (size_t i = 0; i < BLOOM_WIDTH_IN_BYTES; i++)
        os << byte_2_Bits(bits[i]) << " ";
    os << endl;
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
void bloom_t::unlightBit(size_t bit) {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = BLOOM_WIDTH_IN_BYTES - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    bits[index] &= ~(mask);
}

//---------------------------------------------------------------------------
bool bloom_t::isBitLit(size_t bit) const {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = BLOOM_WIDTH_IN_BYTES - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    return (bits[index] & mask);
}

//---------------------------------------------------------------------------
void bloom_t::toggleBit(size_t bit) {
    if (isBitLit(bit))
        unlightBit(bit);
    else
        lightBit(bit);
}

//---------------------------------------------------------------------------
size_t bloom_t::nBitsHit(void) const {
    size_t cnt = 0;
    for (size_t b = 0; b < BLOOM_WIDTH_IN_BYTES; b++) {
        string_q bitStr = byte_2_Bits(bits[b]);
        for (auto ch : bitStr) {
            if (ch == '1')
                cnt++;
        }
    }
    return cnt;
}

//---------------------------------------------------------------------------
bool bloom_t::isInBloom(const bloom_t& test) const {
    for (size_t b = 0; b < BLOOM_WIDTH_IN_BITS; b++)
        if (test.isBitLit(b) && !isBitLit(b))
            return false;
    return true;
}

//---------------------------------------------------------------------------
bloom_t addr_2_Bloom(const address_t& addrIn, CUintArray& litBits) {
    bloom_t ret;
    if (isAddress(addrIn)) {
        string_q sha = addrIn;
        for (size_t k = 0; k < K; k++) {
            string_q dbl_byte = ("0x" + extract(sha, 2 + (k * NIBBLE_WID), NIBBLE_WID));
            uint64_t bit = (str_2_Uint(dbl_byte) % BLOOM_WIDTH_IN_BITS);
            ret.lightBit(bit);
            litBits.push_back(bit);
        }
    }
    return ret;
}

//----------------------------------------------------------------------
bool addToSet(CBloomArray& blooms, const address_t& addr) {
    CUintArray litBits;
    bloom_t zeroBloom = addr_2_Bloom("0x0", litBits);
    if (blooms.size() == 0)
        blooms.push_back(zeroBloom);  // so we have something to add to

    addr_2_Bloom(addr, litBits);
    for (auto bit : litBits)
        blooms[blooms.size() - 1].lightBit(bit);
    blooms[blooms.size() - 1].nInserted++;

    if (blooms[blooms.size() - 1].nInserted > MAX_ADDRS_IN_BLOOM)
        blooms.push_back(zeroBloom);

    return true;
}

//----------------------------------------------------------------------
bool isMember(const CBloomArray& blooms, const bloom_t& bloomIn) {
    for (auto bloom : blooms) {
        if (bloom.isInBloom(bloomIn))
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------
bool readBloomFromBinary(const string_q& fileName, CBloomArray& blooms) {
    blooms.clear();
    CArchive bloomCache(READING_ARCHIVE);
    if (bloomCache.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        uint32_t nBlooms;
        bloomCache.Read(nBlooms);
        for (size_t i = 0; i < nBlooms; i++) {
            bloom_t bloom;
            bloomCache.Read(bloom.nInserted);
            bloomCache.Read(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
            blooms.push_back(bloom);
        }
        bloomCache.Close();
        return true;
    }
    return false;
}

//----------------------------------------------------------------------
bool writeBloomToBinary(const string_q& fileName, const CBloomArray& blooms) {
    lockSection();
    CArchive output(WRITING_ARCHIVE);
    if (!output.Lock(fileName, modeWriteCreate, LOCK_NOWAIT)) {
        unlockSection();
        return false;
    }
    output.Write((uint32_t)blooms.size());
    for (auto bloom : blooms) {
        output.Write(bloom.nInserted);
        output.Write(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
    }
    output.Release();
    unlockSection();
    return true;
}

}  // namespace qblocks
