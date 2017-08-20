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

    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("fastest");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // Test 'zero' is an old bug that originally motivated this code. We used to print 10.2147483648 when we know
        // the value was 1.400001000001. Weird, but true
        if (options.testNum == 1) {
            CPriceQuote::registerClass();
            HIDE_FIELD(CPriceQuote, "timestamp"); HIDE_FIELD(CPriceQuote, "high"); HIDE_FIELD(CPriceQuote, "low");
            HIDE_FIELD(CPriceQuote, "close"); HIDE_FIELD(CPriceQuote, "quoteVolume"); HIDE_FIELD(CPriceQuote, "volume");
            HIDE_FIELD(CPriceQuote, "weightedAvg"); HIDE_FIELD(CPriceQuote, "date");

            CPriceQuote bug;
            bug.open = bug.high = bug.low = bug.close = 1.40000010000001;
            cerr << bug.Format() << "\n";

        } else if (options.testNum == 2) {
            cout << "Test " << options.testNum << "\n";

        } else {
            cout << "Unknown test: " << options.testNum << "\n";

        }
    }

    return 0;
}
