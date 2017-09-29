/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

   // Tell the system where the blocks are and which version to use
    etherlib_init("fastest");

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool loaded = options.loadNames();
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.isEdit) {
            editFile(options.namesFile.getFullPath());
            exit(0);
        }

        if (!loaded) {
            usage(options.namesFile.getFullPath() + " is empty. Use ethName -e to add some names. Quitting...");
            exit(0);
        }

        SFString fmt = (options.alone ? "[{ADDR}]" : "");
        if (options.list) {
            if (options.count)
                cout << options.namedAccounts.getCount() << " items\n";
            for (uint32_t i = 0 ; i < options.namedAccounts.getCount() ; i++)
                cout << options.namedAccounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") << "\n";
            exit(0);
        }

        SFString ret = options.showMatches();
        if (!ret.empty())
            cout << ret;
        else if (verbose)
            cout << "Address '" << options.addr << "' not found\n";
        cout.flush();
    }

    return 0;
}

//-----------------------------------------------------------------------
SFString COptions::showMatches(void) {
    SFString ret;
    uint32_t hits = 0;
    SFString fmt = (alone ? "[{ADDR}]" : "");
    for (uint32_t i = 0 ; i < namedAccounts.getCount() ; i++) {
        if (namedAccounts[i].Match(addr, name, source, matchCase, all)) {
            ret += (namedAccounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") + "\n");
            hits++;
        }
    }

    if (count)
        ret = asString(hits) + " match" + (hits==1?"":"es") + "\n" + (verbose ? ret : "");

    return ret;
}

