/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_ls(void) {

    ostringstream os;
    os << endl << cGreen << "Monitor path: " << cWhite << monitorsPath << endl;

    CStringArray files;
    listFilesInFolder(files, monitorsPath + "*.*");
    sort(files.begin(), files.end());

    CAccountNameArray accounts;
    for (auto file : files) {
        string_q type = nextTokenClear(file, '-');
        if (type == "f" && contains(file, ".acct.bin")) {
            CAccountName item;
            item.addr = nextTokenClear(file, '.');
            getNamedAccount(item, item.addr);
            accounts.push_back(item);
        }
    }
    if (accounts.size() == 0) {
        CAccountName item;
        item.addr = "none";
        accounts.push_back(item);
    }

    os  << "    " << cGreen << "Current monitors:" << cOff << endl << "    ";
    for (auto acct : accounts) {
        os << cTeal << acct.addr;
        os << (acct.name.empty() ? "" : " (" + italic + acct.name + cTeal + ")") << endl;
        os  << "    ";
    }
    os << cOff << endl;

    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/");
    else
        cout << os.str();

    return true;
}
