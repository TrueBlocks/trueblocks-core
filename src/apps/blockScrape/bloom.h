#pragma once

#include "acctlib.h"

#define BLOOM_WIDTH_IN_BYTES (1048576 / 8)
#define BLOOM_WIDTH_IN_BITS (BLOOM_WIDTH_IN_BYTES * 8)
#define MAX_ADDRS_IN_BLOOM 50000
#define BLOOM_K 5

//---------------------------------------------------------------------------
class bloom_t {
  public:
    uint32_t nInserted;
    uint8_t* bits;

  public:
    bloom_t(void);
    ~bloom_t(void);
    bloom_t(const bloom_t& b);
    void lightBit(size_t bit);
};

//---------------------------------------------------------------------------
inline bloom_t::bloom_t(void) {
    nInserted = 0;
    bits = new uint8_t[BLOOM_WIDTH_IN_BYTES];
    bzero(bits, BLOOM_WIDTH_IN_BYTES * sizeof(uint8_t));
}

//---------------------------------------------------------------------------
inline bloom_t::~bloom_t(void) {
    if (bits)
        delete[] bits;
}

//---------------------------------------------------------------------------
inline bloom_t::bloom_t(const bloom_t& b) {
    nInserted = 0;
    bits = new uint8_t[BLOOM_WIDTH_IN_BYTES];
    bzero(bits, BLOOM_WIDTH_IN_BYTES * sizeof(uint8_t));
    nInserted = b.nInserted;
    memcpy(bits, b.bits, BLOOM_WIDTH_IN_BYTES);
}

//---------------------------------------------------------------------------
inline void bloom_t::lightBit(size_t bit) {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = BLOOM_WIDTH_IN_BYTES - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    bits[index] |= mask;
}

//---------------------------------------------------------------------------
inline void getLitBits(const address_t& addrIn, CUintArray& litBitsOut) {
#define EXTRACT_WID 8
    for (size_t k = 0; k < BLOOM_K; k++) {
        string_q fourByte = ("0x" + extract(addrIn, 2 + (k * EXTRACT_WID), EXTRACT_WID));
        uint64_t bit = (str_2_Uint(fourByte) % BLOOM_WIDTH_IN_BITS);
        litBitsOut.push_back(bit);
    }
    return;
}

// TODO: BOGUS - TESTING SCRAPING
bool DebuggingOn = fileExists("./testing");

#if 1
#define LOG_FILE(a, b) LOG_INFO((a), substitute((b), "/Users/jrush/Data/trueblocks/unchained/", "./"))
#else
#define LOG_FILE(a, b)
#endif
