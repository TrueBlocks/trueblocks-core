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
#include "bloom.h"

namespace qblocks {

#define K 10
#define NIBBLE_WID 8

//---------------------------------------------------------------------------
size_t bloom_t::nBitsHit(void) const {
    size_t cnt = 0;
    for (size_t b = 0; b < bloom_t::BYTE_SIZE; b++) {
        string_q bitStr = byte_2_Bits(bits[b]);
        for (auto ch : bitStr) {
            if (ch == '1')
                cnt++;
        }
    }
    return cnt;
}

//---------------------------------------------------------------------------
bloom_t addr_2_Bloom(const address_t& addrIn, CUintArray& litBits) {
    bloom_t ret;
    if (isAddress(addrIn)) {
        string_q sha = addrIn;
        for (size_t k = 0; k < K; k++) {
            string_q dbl_byte = ("0x" + extract(sha, 2 + (k * NIBBLE_WID), NIBBLE_WID));
            uint64_t bit = (str_2_Uint(dbl_byte) % bloom_t::BIT_SIZE());
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

    if (blooms[blooms.size() - 1].nInserted > MAX_INSERTS)
        blooms.push_back(zeroBloom);

    return true;
}

//----------------------------------------------------------------------
bool isMember(const CBloomArray& blooms, const address_t& addr) {
    for (auto bloom : blooms) {
        if (bloom.isInBloom(addr_2_Bloom(addr)))
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------
bool readBloomFromBinary(const string_q& fileName, CBloomArray& blooms) {
    blooms.clear();
    CArchive bloomCache(READING_ARCHIVE);
    if (bloomCache.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        uint32_t n;
        bloomCache.Read(n);
        for (size_t i = 0; i < n; i++) {
            bloom_t bloom;
            bloomCache.Read(bloom.nInserted);
            bloomCache.Read(bloom.bits, sizeof(uint8_t), bloom_t::BYTE_SIZE);
            blooms.push_back(bloom);
        }
        bloomCache.Close();
        return true;
    }
    return false;
}

//----------------------------------------------------------------------
bool writeBloomToBinary(const string_q& fileName, const CBloomArray& blooms) {
    lockSection(true);
    CArchive output(WRITING_ARCHIVE);
    if (!output.Lock(fileName, modeWriteCreate, LOCK_NOWAIT)) {
        lockSection(false);
        return false;
    }
    output.Write((uint32_t)blooms.size());
    for (auto bloom : blooms) {
        output.Write(bloom.nInserted);
        output.Write(bloom.bits, sizeof(uint8_t), qblocks::bloom_t::BYTE_SIZE);
    }
    output.Release();
    lockSection(false);
    return true;
}

//----------------------------------------------------------------------
size_t bloom_t::BYTE_SIZE = N_BYTES;

}  // namespace qblocks
