#pragma once

#include "utillib.h"

namespace qblocks {

#define N_RAND_ADDR ((120000)-1)
#define MAX_INSERTS 50000
#define N_BYTES (1048576 / 8)

//---------------------------------------------------------------------------
class bloom_nt {
public:
    static size_t BYTE_SIZE;
    static size_t BIT_SIZE(void) { return BYTE_SIZE * 8; }

public:
    size_t nInserted;
    uint8_t *bits;

public:
              bloom_nt   (void);
             ~bloom_nt   (void);
              bloom_nt   (const bloom_nt& b);
    bloom_nt& operator=  (const bloom_nt& b);

    void      lightBit   (size_t bit);
    void      unlightBit (size_t bit);
    size_t    nBitsHit   (void) const;

    void      showBloom  (ostream& os) const;
    bool      isBitLit   (size_t bit) const;
    void      toggleBit  (size_t bit);
    bool      isInBloom  (const bloom_nt& test) const;

private:
    void      copy       (const bloom_nt& b);
    void      init       (void);
};
typedef vector<bloom_nt> CNewBloomArray;

//---------------------------------------------------------------------------
inline bloom_nt::bloom_nt(void) {
    init();
}

//---------------------------------------------------------------------------
inline bloom_nt::~bloom_nt(void) {
    if (bits)
        delete [] bits;
}

//---------------------------------------------------------------------------
inline bloom_nt::bloom_nt(const bloom_nt& b) {
    init();
    copy(b);
}

//---------------------------------------------------------------------------
inline bloom_nt& bloom_nt::operator=(const bloom_nt& b) {
    copy(b);
    return *this;
}

//---------------------------------------------------------------------------
inline void bloom_nt::init(void) {
    nInserted = 0;
    bits = new uint8_t[bloom_nt::BYTE_SIZE];
    bzero(bits, bloom_nt::BYTE_SIZE*sizeof(uint8_t));
}

//---------------------------------------------------------------------------
inline void bloom_nt::copy(const bloom_nt& b) {
    nInserted = b.nInserted;
    for (size_t i = 0 ; i < bloom_nt::BYTE_SIZE ; i++) {
        bits[i] = b.bits[i];
    }
}

//---------------------------------------------------------------------------
inline void bloom_nt::showBloom(ostream& os) const {
    for (size_t i = 0 ; i < bloom_nt::BYTE_SIZE ; i++)
        os << byte_2_Bits(bits[i]) << " ";
    os << endl;
}

//---------------------------------------------------------------------------
inline void bloom_nt::lightBit(size_t bit) {
    size_t which  = (bit / 8);
    size_t whence = (bit % 8);
    size_t index  = bloom_nt::BYTE_SIZE - which - 1;
    uint8_t mask  = uint8_t(1 << whence);
    bits[index] |= mask;
}

//---------------------------------------------------------------------------
inline void bloom_nt::unlightBit(size_t bit) {
    size_t which  = (bit / 8);
    size_t whence = (bit % 8);
    size_t index  = bloom_nt::BYTE_SIZE - which - 1;
    uint8_t mask  = uint8_t(1 << whence);
    bits[index] &= ~(mask);
}

//---------------------------------------------------------------------------
inline bool bloom_nt::isBitLit(size_t bit) const {
    size_t which  = (bit / 8);
    size_t whence = (bit % 8);
    size_t index  = bloom_nt::BYTE_SIZE - which - 1;
    uint8_t mask  = uint8_t(1 << whence);
    return (bits[index] & mask);
}

//---------------------------------------------------------------------------
inline void bloom_nt::toggleBit(size_t bit) {
    if (isBitLit(bit))
        unlightBit(bit);
    else
        lightBit(bit);
}

//---------------------------------------------------------------------------
inline bool bloom_nt::isInBloom(const bloom_nt& test) const {
    for (size_t b = 0 ; b < bloom_nt::BIT_SIZE() ; b++)
        if (test.isBitLit(b) && !isBitLit(b))
            return false;
    return true;
}

//---------------------------------------------------------------------------
extern bloom_nt addr_2_Bloom(const address_t& addrIn, CUintArray& litBits);

//---------------------------------------------------------------------------
inline bloom_nt addr_2_Bloom(const address_t& addrIn) {
    CUintArray litBits;
    return addr_2_Bloom(addrIn, litBits);
}

//----------------------------------------------------------------------
extern bool addToSet(CNewBloomArray& blooms, const address_t& addr);
extern bool isMember(const CNewBloomArray& blooms, const address_t& addr);
extern bool writeNewBloom(const string_q& outFile, const CNewBloomArray& blooms);

}  // namespace
