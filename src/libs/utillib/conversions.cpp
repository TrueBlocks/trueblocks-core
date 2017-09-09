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

    //------------------------------------------------------------------
    class SFBloomHex : public SFBigNumStore<unsigned char> {
    public:
        SFBloomHex(const SFUintBN& numIn);
        SFString str;
    };

    //------------------------------------------------------------------
    SFBloomHex::SFBloomHex(const SFUintBN& numIn) {
        len = 1024;
        allocate(1024);

        SFUintBN x2(numIn);
        unsigned int digitNum = 0;
        while (x2.len != 0)
        {
            SFUintBN lastDigit(x2);
            lastDigit.divide(16, x2);
            blk[digitNum] = (unsigned char)lastDigit.to_ushort();
            digitNum++;
        }
        len = digitNum;

        char s[1024+1];
		memset(s,'\0',sizeof(s));
        for (unsigned int p=0;p<len;p++) {
            unsigned short c = blk[len-1-p];
            s[p] = ((c < 10) ? char('0'+c) : char('A'+c-10));
        }
        str = s;
    }

    //------------------------------------------------------------------
    SFString fromBloom(SFBloom bl) {
        if (bl != 0) {
            SFBloomHex b2(bl);
            return ("0x"+padLeft(toLower(b2.str),512,'0'));
        }
        return "0x0";
    }

}  // namespace qblocks
