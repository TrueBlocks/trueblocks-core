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

// Gnosis
//blknum_t tokenStart = 3593108;
//blknum_t tokenEnd   = 3593475;

SFUint32 skip = 1; //10;
blknum_t pad = 500;

//-------------------------------------------------------------
int main(int argc, const char *argv[])
{
    etherlib_init("binary");

    forEveryNonEmptyBlockOnDisc(displayBloom, null, tokenStart - pad, tokenEnd - tokenStart + (pad*2), skip);

    return 0;
}

//-------------------------------------------------------------
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
