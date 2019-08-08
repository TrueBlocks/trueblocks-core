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
bool COptions::handle_status(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    ostringstream os;
    os << cGreen << "Monitor path: " << cWhite << getMonitorPath("") << endl;

    bool stats = contains(tool_flags, "-l") || contains(tool_flags, "--long");

    CStringArray files;
    if (isTestMode()) {
        files.push_back("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359.accts.bin");

    } else {
        if (stats && addrs.size()) {
            for (auto addr : addrs) {
                string_q fn = getMonitorPath(addr + ".acct.bin");
                if (fileExists(fn))
                    files.push_back(fn);
                else
                    LOG_WARN(fn, " not found.");
            }
        } else {
            listFilesInFolder(files, getMonitorPath("*.*"), false);
        }
    }

    CAccountNameArray accounts;
    for (auto file : files) {
        if (endsWith(file, ".acct.bin")) {
            replace(file, getMonitorPath(""), "");
            CAccountName item;
            item.addr = nextTokenClear(file, '.');
            getNamedAccount(item, item.addr);
            item.name = substitute(substitute(item.name, "(", ""), ")", "");
            item.fn = substitute(getMonitorPath(item.addr), getCachePath(""), "./");
            item.size = fileSize(getMonitorPath(item.addr));
            item.lb = str_2_Uint(asciiFileToString(getMonitorLast(item.addr)));
            item.le = str_2_Uint(asciiFileToString(getMonitorExpt(item.addr)));
            item.nrecs = fileSize(getMonitorPath(item.addr)) / sizeof(CAppearance_base);
            accounts.push_back(item);
        }
    }
    if (accounts.size() == 0) {
        if (api_mode) {
            CAccountName item;
            item.addr = "0x0";
            item.name = "none";
            accounts.push_back(item);
        } else {
            LOG_WARN("No monitors found.");
            EXIT_NOMSG4(true);
        }
    }
    sort(accounts.begin(), accounts.end());

    if (api_mode) {
        SHOW_FIELD(CAccountName, "fn");
        SHOW_FIELD(CAccountName, "size");
        SHOW_FIELD(CAccountName, "lb");
        SHOW_FIELD(CAccountName, "le");
        SHOW_FIELD(CAccountName, "nrecs");
        ostringstream oss;
        if (accounts.size() > 1)
            oss << "[";
        bool first = true;
        for (auto acct : accounts) {
            if (!first)
                oss << ",";
            oss << acct;
            first = false;
        }
        if (accounts.size() > 1)
            oss << "]";
        cout << substitute(substitute(oss.str(), "\n", ""), "\t", "") << endl;
        EXIT_NOMSG4(true);
    }

    if (stats) {
        for (auto acct : accounts) {
            string_q fmt =
                "[Address:  -c1-{ADDR}-off-\n]"
                "[\tName:        -c2-{NAME}-off-\n]"
                "[\tFile name:   -c2-{FN}-off-\n]"
                "[\tFile size:   -c2-{SIZE}-off-\n]"
                "[\tLast block:  -c2-{LB}-off-\n]"
                "[\tLast export: -c2-{LE}-off-\n]"
                "[\tnRecords:    -c2-{NRECS}-off-\n]";
            replaceAll(fmt, "-c1-", cTeal);
            replaceAll(fmt, "-c2-", cYellow);
            replaceAll(fmt, "-off-", cOff);
            os << acct.Format(fmt);
        }
    } else {
#ifdef TIOCGSIZE
        struct ttysize ts;
        ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
        size_t ncols = ts.ts_cols;
#elif defined(TIOCGWINSZ)
        struct winsize ts;
        ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
        size_t ncols = ts.ws_col;
#else
        size_t ncols = 1;
#endif /* TIOCGSIZE */
        ncols = max(ncols, (size_t)1);

        os << cGreen << "Current monitors:" << cOff << endl;

        // find the longest name (max 25 chars)
        size_t longest = 0;
        for (auto acct : accounts)
            longest = max(longest, acct.name.length() + 3); // two parens
        longest = min(longest, (size_t)23); // max 23
        ncols = max(((size_t)1), (ncols / (size_t(42) + longest)));

        uint64_t cnt = 0;
        for (auto acct : accounts) {
            string_q name = acct.name;
            if (!name.empty())
                name = "(" + name.substr(0,20) + ") ";
            os << " " << cTeal << acct.addr << " " << padRight(name, longest);
            if (!(++cnt % ncols))
                os << endl;
        }
        os << cOff << endl;
    }

    if (isTestMode())
        cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/");
    else
        cout << os.str();

    EXIT_NOMSG4(true);
}
