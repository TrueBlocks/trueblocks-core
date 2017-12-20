/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init("");

    COptions opt;
    if (opt.parseArguments(argc, argv)) {
        forEveryBlock ( displayBloom, &opt, opt.start, opt.nBlocks, opt.skip );
    }

    return 0;
}

//-------------------------------------------------------------
bool displayBloom(CBlock& block, void *data)
{
    SFString s = fromBloom(block.logsBloom);
    COptions *opt = (COptions*)data;
    if (opt->mode == "short") {
        size_t len = s.length();
        s.Replace   ("0x",     "");
        s.ReplaceAny("0",      "");
        s.ReplaceAny("1248",   ".");
        s.ReplaceAny("3569ac", ".");
        s.ReplaceAny("7bde",   "+");
        s.ReplaceAny("f",      "@");
        size_t rem = len - s.length();
        s = s + SFString(' ', rem) + "|";
    } else {
        s.Replace("0x", cOff);
        s.ReplaceAll("0",  " ");
        s.ReplaceAll("1",  "[.");
        s.ReplaceAll("2",  "[.");
        s.ReplaceAll("4",  "[.");
        s.ReplaceAll("8",  "[.");
        s.ReplaceAll("3",  "+.");
        s.ReplaceAll("5",  "+.");
        s.ReplaceAll("6",  "+.");
        s.ReplaceAll("9",  "+.");
        s.ReplaceAll("a",  "+.");
        s.ReplaceAll("c",  "+.");
        s.ReplaceAll("7",  "-+");
        s.ReplaceAll("b",  "-+");
        s.ReplaceAll("d",  "-+");
        s.ReplaceAll("e",  "-+");
        s.ReplaceAll("f",  "%@");
//        s.ReplaceAll(" ",cOff);
        s.ReplaceAll("[",cOff+cWhite);
        s.ReplaceAll("+",cOff+cYellow);
        s.ReplaceAll("-",cOff+cMagenta);
        s.ReplaceAll("%",cOff+bBlue);
    }
    cout << cOff << block.blockNumber << s << "\n";
    cout.flush();
    return true;
}
