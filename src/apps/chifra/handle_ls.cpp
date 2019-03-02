/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_ls(void) {

    ostringstream os;
    os << endl << cGreen << "Monitor path: " << cWhite << monitorsPath << endl;

    CStringArray files;
    listFilesInFolder(files, monitorsPath + "*.*");

    CAccountNameArray accounts;
    for (auto file : files) {
        string_q type = nextTokenClear(file, '-');
        if (type == "f" && contains(file, ".acct.bin") && !contains(file, ".lck")) {
            CAccountName item;
            item.addr = nextTokenClear(file, '.');
            getNamedAccount(item, item.addr); item.name = substitute(substitute(item.name, "(", ""), ")", "");
            accounts.push_back(item);
        }
    }
    if (accounts.size() == 0) {
        CAccountName item;
        item.addr = "none";
        accounts.push_back(item);
    }
    sort(accounts.begin(), accounts.end());

#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    size_t ncols = ts.ts_cols;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    size_t ncols = ts.ws_col;
#endif /* TIOCGSIZE */

    size_t mx = 0;
    for (auto acct : accounts)
        mx = max(mx, acct.addr.length() + 20 + 3);

    uint64_t cnt = 0;
    os << "  " << cGreen << "Current monitors:" << cOff << endl;
    for (auto acct : accounts) {
        os << "    " << cTeal << acct.addr;
        string_q nm = acct.name.empty() ? "" : " (" + acct.name.substr(0,20) + ")";
        os << padRight(nm, 22);
        if (!(++cnt % (ncols / mx)))
            os << endl;
    }
    os << cOff << endl;

    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/");
    else
        cout << os.str();

    return true;
}
