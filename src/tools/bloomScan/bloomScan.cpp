/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

// TokenCard
blknum_t tokenStart = 3700000;
blknum_t tokenEnd   = 3900000;

// gnosis
//blknum_t tokenStart = 3593108;
//blknum_t tokenEnd   = 3593475;

SFUint32 skip = 1; //10;
blknum_t pad = 500;

//-------------------------------------------------------------
int main(int argc, const char *argv[])
{
	etherlib_init();

    forEveryNonEmptyBlockOnDisc(displayBloom, null, tokenStart - pad, tokenEnd - tokenStart + (pad*2), skip);

    return 0;
}

inline SFString toSpaces2(const SFBloom a) {
    SFString ret = bBlue+fromBloom(a).Substitute("0",cOff+"."+bBlue);
    SFString colors[] = {
        bRed, bGreen, bYellow, bBlue, bMagenta, bTeal, bWhite,
    };
//    ret.ReplaceAll(asString(1),colors[0]+asString(1)+cOff);
//    ret.ReplaceAll(asString(2),colors[1]+asString(2)+cOff);
//    ret.ReplaceAll(asString(4),colors[2]+asString(4)+cOff);
//    ret.ReplaceAll(asString(8),colors[3]+asString(8)+cOff);
    return ret;
}

//-------------------------------------------------------------
bool displayBloom(CBlock& block, void *data)
{
//    if (tokenStart - block.blockNumber < skip)
    if (!(block.blockNumber % 100))
        cout << bGreen << SFString('-',522) << "\n" << SFString('-',522) << "\n" << SFString('-',522) << "\n" << cOff;

    cout << block.blockNumber << " " << toSpaces2(block.logsBloom) << "\n";

    if (tokenEnd - block.blockNumber < skip)
        cout << bGreen << SFString('-',522) << "\n" << SFString('-',522) << "\n" << SFString('-',522) << "\n" << cOff;

    return true;
}

#if 0
//-------------------------------------------------------------
    if (green && block.blockNumber == 3593273) cout << bGreen << SFString('-',108) << cOff << "\n";
    SFString s = fromBloom(block.logsBloom).substr(2).c_str();
    uint32_t cnt=0;
    for (int i=0;i<s.length();i++)
    {
        char ss[4];
        ss[0] = '0';
        ss[1] = 'x';
        ss[2] = s[i];
        ss[3] = '\0';
        unsigned char x = (unsigned char)strtoul((const char*)&ss,NULL,16);
//        if (std::bitset<8>(x).count() > 2)
//            cout << s[i] << " : " << std::bitset<8>(x).count() << "          \n";
        cnt += std::bitset<8>(x).count();
    }
//    cout << cnt << "\n";
//    cout << cnt * 10000 << "\n";
//    cout << (cnt * 10000 / 2048) << "\n";
    float x = (2 * ((cnt * 10000 / 2048) / 100.) + 0.5);
//    cout << x << "\n";
//uint32_t pct = (uint32_t)((cnt * 100) / 2048. / 100.);
    cout << block.blockNumber << " " << SFString('x',x).Substitute("x","░") << SFString('.',200-x) << "\n"; //\r";
    if (green && block.blockNumber == 3593309) cout << bGreen << SFString('-',108) << cOff << "\n";
    cout.flush();
#endif
