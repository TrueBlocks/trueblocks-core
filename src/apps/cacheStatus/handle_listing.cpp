/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
bool COptions::handle_listing(ostream& os) {
    cout << "List mode" << endl;
    ENTER8("handle_" + mode);
    nodeNotRequired();

    os << cGreen << "Monitor path: " << cWhite << getMonitorPath("") << endl;

    string_q tool_flags = (details ? "--long" : "");
    bool stats = contains(tool_flags, "-l") || contains(tool_flags, "--long");

    CStringArray files;
    CStringArray addrs;
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

    CAccountNameArray accts;
    for (auto file : files) {
        if (endsWith(file, ".acct.bin")) {
            replace(file, getMonitorPath(""), "");
            CAccountName item;
            item.address = nextTokenClear(file, '.');
            getNamedAccount2(item, item.address);
            item.name = substitute(substitute(item.name, "(", ""), ")", "");
            item.path = substitute(getMonitorPath(item.address), getCachePath(""), "./");
            item.sizeInBytes = fileSize(getMonitorPath(item.address));
            item.latestAppearance = str_2_Uint(asciiFileToString(getMonitorLast(item.address)));
            item.lastExport = str_2_Uint(asciiFileToString(getMonitorExpt(item.address)));
            item.m_deleted = fileExists(getMonitorPath(item.address + ".deleted"));
            //            item.lastBalance = 0;
            item.nAppearances = fileSize(getMonitorPath(item.address)) / sizeof(CAppearance_base);
            accts.push_back(item);
        }
    }
    if (accts.size() == 0) {
        if (isApiMode()) {
            CAccountName item;
            item.address = "0x0";
            item.name = "none";
            accts.push_back(item);
        } else {
            LOG_WARN("No monitors found.");
            EXIT_NOMSG8(true);
        }
    }
    sort(accts.begin(), accts.end());

    if (isApiMode()) {
        SHOW_FIELD(CAccountName, "path");
        SHOW_FIELD(CAccountName, "sizeInBytes");
        SHOW_FIELD(CAccountName, "latestAppearance");
        SHOW_FIELD(CAccountName, "lastExport");
        SHOW_FIELD(CAccountName, "nAppearances");
        ostringstream oss;
        if (accts.size() > 1)
            oss << "[";
        bool first = true;
        for (auto acct : accts) {
            if (!first)
                oss << ",";
            oss << acct.Format();
            first = false;
        }
        if (accts.size() > 1)
            oss << "]";
        cout << substitute(substitute(oss.str(), "\n", ""), "\t", "") << endl;
        EXIT_NOMSG8(true);
    }

    if (stats) {
        for (auto acct : accts) {
            string_q fmt =
                "[Address:   -c1-{ADDRESS}-off-\n]"
                "[\tName:         -c2-{NAME}-off-\n]"
                "[\tFile name:    -c2-{PATH}-off-\n]"
                "[\tFile size:    -c2-{SIZEINBYTES}-off-\n]"
                "[\tLast block:   -c2-{LATESTAPPEARANCE}-off-\n]"
                "[\tLast export:  -c2-{LASTEXPORT}-off-\n]"
                "[\tnAppearances: -c2-{NAPPEARANCES}-off-\n]";
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
        for (auto acct : accts)
            longest = max(longest, acct.name.length() + 3);  // two parens
        longest = min(longest, (size_t)23);                  // max 23
        ncols = max(((size_t)1), (ncols / (size_t(42) + longest)));

        uint64_t cnt = 0;
        for (auto acct : accts) {
            string_q name = acct.name;
            if (!name.empty())
                name = "(" + name.substr(0, 20) + ") ";
            os << " " << cTeal << acct.address << " " << setw(static_cast<int>(longest)) << name;
            if (!(++cnt % ncols))
                os << endl;
        }
        os << cOff << endl;
    }

    //    if (isTestMode())
    //        cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/");
    //    else
    //        cout << os.str();

    EXIT_NOMSG8(true);
}
