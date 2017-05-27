/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern void doTests(void);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (isTesting || !options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (isTesting)
            doTests();
        else
            cout << getSha3(argv[1]) << "\n";
        isTesting = false;
    }
    return 0;
}

//--------------------------------------------------------------
void doTests(void) {
    SFString in;
    bool hasIn = false;
    SFString contents = asciiFileToString("./tests/input.txt");
    while (!contents.empty()) {
        SFString line = nextTokenClear(contents, '\n');
        if (!line.startsWith('#')) {
            if (!hasIn) {
                in = line;
                hasIn = true;
            } else {
                SFString hex = in.startsWith("0x") ? in : "0x" + string2Hex(in);
                SFString out = getSha3(hex);
                cout << "in:\t\t" << in << "\n"
                    << "hex:\t\t" << hex << "\n"
                    << "expected:\t" << line << "\n"
                    << "out:\t\t" << out << " " << (out == line ? greenCheck : redX) << "\n\n";
                hasIn = false;
            }
        }
    }
}
