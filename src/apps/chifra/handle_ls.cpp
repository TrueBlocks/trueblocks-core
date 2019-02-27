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
    os << cGreen << "Monitor path: " << cWhite << monitorsPath << endl;

    CStringArray files;
    listFilesInFolder(files, monitorsPath);
    sort(files.begin(), files.end());

    CAccountNameArray accounts;
    for (auto file : files) {
        if (file != "d-monitors" && startsWith(file, "d-")) {
            CAccountName item;
            item.addr = substitute(file.substr(0, file.find(".")), "d-", "");
            getNamedAccount(item, item.addr);
            accounts.push_back(item);
        }
    }
    if (accounts.size() == 0) {
        CAccountName item;
        item.addr = "none";
        accounts.push_back(item);
    }

    os << cGreen << "Current monitors:" << cOff << endl << "    ";
    os << cTeal;
    for (auto acct : accounts)
        os << acct.addr << "\t" << padLeft(acct.name, 20) << endl;
    os << cOff << endl;

    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/");
    else
        cout << os.str();

    return true;
}
