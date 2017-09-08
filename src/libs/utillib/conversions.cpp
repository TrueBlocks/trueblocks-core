/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "biglib.h"
#include "conversions.h"

namespace qblocks {

#ifndef NEW_CODE
#define NEW_CODE
#endif
#ifdef NEW_CODE
    class BigUnsignedInABase2 : public SFBigNumStore<unsigned short>
    {
    public:
        unsigned short base;
        BigUnsignedInABase2(const SFUintBN &x);
        operator string() const;
    };

    unsigned int bitLen2(unsigned int x) {
        unsigned int len = 0;
        while (x > 0) {
            x >>= 1;
            len++;
        }
        return len;
    }

    //------------------------------------------------------------------
    BigUnsignedInABase2::BigUnsignedInABase2(const SFUintBN &x)
    {
        this->base = 16;

        // Get an upper bound on how much space we need
        int maxBitLenOfX    = (int)(x.len * SFUintBN::N);
        int minBitsPerDigit = (int)bitLen2(16) - 1;
        int maxDigitLenOfX  = (maxBitLenOfX + minBitsPerDigit - 1) / minBitsPerDigit;

        len = (unsigned int)maxDigitLenOfX; // Another change to comply with `staying in bounds'.
        allocate(len); // Get the space

        SFUintBN x2(x), buBase(16);
        unsigned int digitNum = 0;

        while (x2.len!=0)
        {
            // Get last digit.  This is like `lastDigit = x2 % buBase, x2 /= buBase'.
            SFUintBN lastDigit(x2);
            lastDigit.divide(buBase, x2);
            // Save the digit.
            blk[digitNum] = lastDigit.to_ushort();
            // Move on.  We can't run out of room: we figured it out above.
            digitNum++;
        }

        // Save the actual length.
        len = digitNum;
    }

    //------------------------------------------------------------------
    BigUnsignedInABase2::operator string(void) const
    {
        if (len==0)
            return string("0");

        char *s = new char[len+1];
        memset(s,'\0',len+1);
        for (unsigned int p=0;p<len;p++)
        {
            unsigned short c = blk[len-1-p];
            s[p] = ((c < 10) ? char('0'+c) : char('A'+c-10));
        }

        string ret = s;
        delete [] s;
        return ret;
    }

    SFString fromBloom(SFBloom bl) {
        if (bl == 0)
            return "0x0";
        BigUnsignedInABase2 b2(bl);
        SFString ret = b2.operator::string().c_str();
        return ("0x"+padLeft(toLower(ret),512,'0'));
    }
#else
    int unusedToAvoidWarnings = 0;
#endif

}  // namespace qblocks
