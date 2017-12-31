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
int main(int argc, const char * argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        while (!options.fileName.empty()) {
            SFString fileName = nextTokenClear(options.fileName, '|');
            cout << fileName << "\n";
            SFString contents = asciiFileToString(fileName);
            while (!contents.empty()) {
                SFString line = nextTokenClear(contents, '\n');
                cout << line << "\n";
            }
        }
    }

    return 0;
}
