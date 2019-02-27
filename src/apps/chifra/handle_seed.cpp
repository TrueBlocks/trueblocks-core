/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_seed(void) {

    establishFolder(indexFolder);
    establishFolder(indexFolder_prod);
    establishFolder(indexFolder_stage);
    establishFolder(indexFolder_zips);

    string_q contents;
    string_q source = configPath("chifra/ipfs_get.sh");
    asciiFileToString(source, contents);

    CStringArray cmds;
    explode(cmds, contents, '\n');

    for (auto cmd : cmds) {

        // format: ipfs get <ipfs_hash> -o <first-last>.txt.gz
        // part:    0    1       2       3         4
        CStringArray parts;
        explode(parts, cmd, ' ');
        ostringstream ipfs_cmd;
        ipfs_cmd << parts[0] << " " << parts[1] << " " << parts[2] << " " << parts[3] << " ";

        string_q zipFile = indexFolder_zips + parts[4];
        string_q textFile = substitute(indexFolder_prod + parts[4], ".gz", "");

        if (isTestMode()) cout << endl;
        if (!fileExists(zipFile) || isTestMode()) {
            ostringstream os;

            // go to the zips folder
            os << "cd \"" << indexFolder_zips << "\" ; ";

            // get the zip file from the IPFS cache
            os << ipfs_cmd.str() << " \"" << zipFile << "\" ; ";
            if (isTestMode())
                cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/") << endl;
            else
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

        } else {
            if (verbose)
                cout << "Zip file " << cTeal << zipFile << cOff << " is up to date." << endl;
        }

        if (!fileExists(textFile) || isTestMode()) {
            ostringstream os;

            // go to the production folder
            os << "cd \"" << indexFolder_prod << "\" ; ";

            // copy the new zip locally and unzip it
            os << "cp -p \"" << zipFile << "\" \"" << indexFolder_prod << parts[4] << "\" ; ";
            os << "gunzip \"" << parts[4] << "\" ; cd -";
            cerr << "Seeding " << cTeal << textFile << cOff << endl;
            if (isTestMode())
                cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/") << endl;
            else
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        } else {
            if (verbose)
                cout << "Index file " << cTeal << textFile << cOff << " exists." << endl;
        }
    }
    cerr << cGreen << "Index cache has been seeded." << cOff << endl;
    return true;
}
