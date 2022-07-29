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

//----------------------------------------------------------------------
inline bool addToSet(vector<bloom_t>& array, const address_t& addr) {
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

// TODO: BOGUS - TESTING SCRAPING
bool DebuggingOn = fileExists("./testing");

#if 1
size_t fff = 52;
char x = '-';
#define LOG_IN(a, f)                                                                                                   \
    string_q nm = (f);                                                                                                 \
    if ((a) != x) {                                                                                                    \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
    }                                                                                                                  \
    LOG_INFO(string_q(12, (a)), " in ", (nm), " ", string_q(fff, (a)));

#define LOG_OUT(a, b)                                                                                                  \
    LOG_INFO(string_q(12, (a)), " out ", (nm), " ", (b), " ", string_q(fff, (a)));                                     \
    if ((a) != x) {                                                                                                    \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
    }

#define LOG_FILE(a, b) LOG_INFO((a), substitute((b), "/Users/jrush/Data/trueblocks/unchained/", "./"))
#else
#define LOG_IN(a, f)
#define LOG_OUT(a, b)
#define LOG_FILE(a, b)
#endif
