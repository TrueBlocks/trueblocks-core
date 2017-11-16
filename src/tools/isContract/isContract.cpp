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
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        for (uint64_t i = 0 ; i < options.nAddrs ; i++) {
            SFString code1 = getCode(options.addrs[i]);
            bool hasCode = code1.length() > 2;

            if (options.diff) {
                SFString code2 = getCode(options.addrs[i+1]);
                cout << "Code at address '" << options.addrs[i] << ") and (" << "(" << options.addrs[i+1] + ") are "
                        << (code1 == code2 ? "identical" : "different") << "\n";
                break;

            } else if (options.showBytes) {

                cout << "Code at address: " << options.addrs[i] << ":\n";
                cout << code1 << "\n";

            } else if (options.asData) {
                cout << options.addrs[i] << "\t" << (hasCode ? "true" : "false") << "\n";

            } else {
                cout << "isContract(" << options.addrs[i] << "): " << (hasCode ? "true" : "false") << "\n";
                if (verbose)
                    cout << code1 << "\n";

            }
        }
    }
    return 0;
}
