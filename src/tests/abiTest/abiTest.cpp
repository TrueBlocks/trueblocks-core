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
    if (!fileExists("./input.txt"))
        return usage("./input.txt file not found.\n");

    SFString contents = asciiFileToString("./input.txt").Substitute("\t\t", "\t");
    nextTokenClear(contents, '\n');  // skip header row
    while (!contents.empty()) {
        SFString expected = nextTokenClear(contents, '\n');
        if (!expected.startsWith(';')) {
            SFString type   = nextTokenClear(expected, '\t');
            SFString text   = nextTokenClear(expected, '\t');
            SFString myHex = "0x"+string2Hex(text);
            SFString mySha = getSha3(myHex);
            if (type == "function")
                mySha = mySha.Left(10);
            cout << type << ": " << text << " "
                << "encoding: " << mySha << " "
                << (mySha == expected ? greenCheck : redX) << "\n";
        }
    }
    return 0;
}
