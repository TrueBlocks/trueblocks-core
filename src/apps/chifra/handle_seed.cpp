/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_seed(void) {

    string_q source = configPath("chifra/ipfs_get.sh");
    string_q dest = indexFolder_prod;
//    cout << "source: " << source << endl;
//    cout << "dest: " << dest << endl;
    establishFolder(dest);

    string_q contents;
    asciiFileToString(source, contents);

    CStringArray cmds;
    explode(cmds, contents, '\n');

    for (auto cmd : cmds) {
        CStringArray parts;
        explode(parts, cmd, ' ');

        ostringstream os;
        os << "cd \"" << dest << "\" ; ";
        os << "rm -f " << substitute(parts[4], ".txt.gz", "*") << " ; ";
        os << parts[0] << " " << parts[1] << " " << parts[2] << " ";
        os << parts[3] << " " << parts[4] << " ; ";
        os << "gunzip " << parts[4] << " ; cd -";
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    }
    return true;
}
