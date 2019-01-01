/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool accumAddrs(const CAddressAppearance& item, void *data) {
    if (isZeroAddr(item.addr) || !data)
        return true;
    CAddressArray *array = (CAddressArray*)data;
    array->push_back(item.addr);
    return true;
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    // TODO: Use an option here for deep trace
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//--------------------------------------------------------------------------------
string_q bloom_2_BitStr(const bloom_t& bl) {
    ostringstream os;
    string_q s1 = substitute(bloom_2_Bits(bl), "0", ".");
    for (size_t i = 0 ; i < 16 ; i++) {
        os << "\t";
        for (size_t j = 0 ; j < 128 ; j = j + 10)
            os << extract(extract(s1, i * 128, 128), j, 10) + " ";
        os << endl;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q bloom_2_ByteStr(const bloom_t& bl) {
    ostringstream os;
    string_q s1 = substitute(substitute(bloom_2_Bytes(bl), "0x", ""), "0", ".");
    for (size_t i = 0 ; i < 4 ; i++) {
        os << "\t";
        for (size_t j = 0 ; j < 128 ; j = j + 10)
            os << extract(extract(s1, i * 128, 128), j, 10) + " ";
        os << endl;
    }
    return os.str();
}

//-------------------------------------------------------------------------
bool compareBlooms(const bloom_t& b1, const bloom_t& b2, string_q& str) {
    if (verbose > 2) {
        string_q s1 = substitute(bloom_2_Bits(b1), "0", ".");
        string_q s2 = substitute(bloom_2_Bits(b2), "0", ".");
        for (size_t i = 0 ; i < 16 ; i++) {
            string_q m1, m2;
            for (size_t j = 0 ; j < 128 ; j = j + 10) {
                m1 += extract(extract(s1, i*128, 128), j, 10) + " ";
                m2 += extract(extract(s2, i*128, 128), j, 10) + " ";
            }
            str += ("\n\t" + cRed + m1 + cOff + "\n\t" + m2);
        }
    } else if (verbose > 1) {
        string_q s1 = substitute(substitute(bloom_2_Bytes(b1), "0x", ""), "0", ".");
        string_q s2 = substitute(substitute(bloom_2_Bytes(b2), "0x", ""), "0", ".");
        for (size_t i = 0 ; i < 4 ; i++) {
            string_q m1, m2;
            for (size_t j = 0 ; j < 128 ; j = j + 10) {
                m1 += extract(extract(s1, i*128, 128), j, 10) + " ";
                m2 += extract(extract(s2, i*128, 128), j, 10) + " ";
            }
            str += ("\n\t" + cRed + m1 + cOff + "\n\t" + m2);
        }
    }
    return isBloomHit(b1, b2);
}

//-------------------------------------------------------------------------
string_q compareBlooms(const bloom_t& b1, const bloom_t& b2) {
    string_q ret; compareBlooms(b1, b2, ret); return ret;
}
