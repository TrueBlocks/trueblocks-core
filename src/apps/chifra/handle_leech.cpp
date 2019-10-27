/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_leech(void) {

#if 0
    // leech mode does not require a running node
    nodeNotRequired();
    // TODO(tjayrush): We should check that IPFS is running here (in much the same way we do for the node)

    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_unripe);
    establishFolder(indexFolder_zips);

    string_q contents;
    string_q source = configPath("chifra/ipf s_get.sh");
    asciiFileToString(source, contents);

    CStringArray cmds;
    explode(cmds, contents, '\n');

    for (auto cmd : cmds) {

        // format: ipfs get <ipfs_hash> -o <first-last>.txt.gz
        // part:    0    1       2       3         4
        CStringArray parts;
        explode(parts, cmd, ' ');
        ostringstream ipfs_cmd;
        string_q filename;
        if (parts[0] == "curl") {
            ipfs_cmd << parts[0] << " " << parts[1] << " " << parts[2] << " ";
            filename = parts[3];
        } else {
            ipfs_cmd << parts[0] << " " << parts[1] << " " << parts[2] << " " << parts[3] << " ";
            filename = parts[4];
        }

        string_q zipFile = indexFolder_zips + filename;
        string_q textFile = substitute(indexFolder_sorted + filename, ".gz", "");

        if (!fileExists(zipFile) || isTestMode()) {
            ostringstream os;

            // go to the zips folder
            os << "cd \"" << indexFolder_zips << "\" && ";

            // get the zip file from the IPFS cache
            os << ipfs_cmd.str() << " \"" << zipFile << "\"";
            if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            usleep(500000); // so Ctrl+C works

        } else {
            if (verbose)
                cout << "File " << cTeal << zipFile << cOff << " " << greenCheck << endl;
        }

        if (!fileExists(textFile) || isTestMode()) {
            ostringstream os;

            // go to the production folder
            os << "cd \"" << indexFolder_sorted << "\" && ";

            // copy the new zip locally and unzip it
            os << "cp -p \"" << zipFile << "\" \"" << indexFolder_sorted << filename << "\" && ";
            os << "gunzip \"" << filename << "\" && cd - >/dev/null";

            cerr << "Leeching " << cTeal << textFile << cOff << endl;
            if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            usleep(500000); // so Ctrl+C works

        } else {
            if (verbose)
                cout << "File " << cTeal << textFile << cOff << " " << greenCheck << endl;
        }
    }
    cerr << cGreen << "The index cache has been leeched." << cOff << endl;
#endif
    return true;
}
