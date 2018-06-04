#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

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

        // Test 'zero' is an old bug that originally motivated this code. We used to print 10.2147483648 when we know
        // the value was 1.400001000001. Weird, but true
        if (options.testNum == 1) {
            HIDE_FIELD(CPriceQuote, "timestamp");
            HIDE_FIELD(CPriceQuote, "date");

            CPriceQuote bug;
            bug.close = 1.40000010000001;
            cerr << bug.Format() << "\n";

        } else if (options.testNum == 2) {
            cout << "Test " << options.testNum << "\n";

        } else {
            return usage("Unknown test: " + asString(options.testNum) + "\n");

        }
    }

    return 0;
}

#if 0
// PRINTING HEX
int main(int argc, char *argv[]) {
    uint64_t x = NOPOS;
    char ss[100];
    sprintf(ss,"%016llx\n", x);
    SFString s = ss;
    cout << "0x" << toLower(s) << "\n";
    return 1;
}
#endif
