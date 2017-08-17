/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#define _DEBUG
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    // Note: we use a single block here instead of creating one in each loop
    // to see if there is any residual left over. The reads should reset the data
    CBlock block;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        switch (options.testNum) {
            case 0: {
                ASSERT(fileExists("./oldFmt.bin"));
                cout << "Read from old binary format and do nothing...\n";
                readOneBlock_fromBinary(block, "./oldFmt.bin");
                cout << block.Format() << "\n";
                break;
            }
            case 1: {
                ASSERT(fileExists("./oldFmt.bin"));
                cout << "Read from old binary format, write to JSON...\n";
                readOneBlock_fromBinary(block, "./oldFmt.bin");
                writeToJson(block, "./newFmt.json");
                ASSERT(fileExists("./newFmt.json"));
                cout << block.Format() << "\n";
                break;
            }
            case 2: {
                ASSERT(fileExists("./newFmt.json"));
                cout << "Read from JSON, write to new binary format...\n";
                readOneBlock_fromJson(block, "./newFmt.json");
                writeToBinary(block, "./newFmt.bin");
                ASSERT(fileExists("./newFmt.bin"));
                cout << block.Format() << "\n";
                break;
            }
            case 3: {
                ASSERT(fileExists("./newFmt.bin"));
                cout << "Read from new binary format, write to JSON...\n";
                readOneBlock_fromBinary(block, "./newFmt.bin");
                writeToJson(block, "./newFmt2.json");
                ASSERT(fileExists("./newFmt2.json"));
                cout << block.Format() << "\n";
                break;
            }
            case 4: {
                ASSERT(fileExists("./newFmt2.json"));
                cout << "Read from new JSON and we're done.\n";
                readOneBlock_fromJson(block, "./newFmt2.json");
                cout << block.Format() << "\n";

                // Clean up all but the original old format and make sure we did.
                ASSERT( fileExists("./newFmt.bin"));
                        removeFile("./newFmt.bin");
                ASSERT(!fileExists("./newFmt.bin"));

                ASSERT( fileExists("./newFmt.json"));
                        removeFile("./newFmt.json");
                ASSERT(!fileExists("./newFmt.json"));

                ASSERT( fileExists("./newFmt2.json"));
                        removeFile("./newFmt2.json");
                ASSERT(!fileExists("./newFmt2.json"));

                break;
            }
        }
    }

    return 0;
}
