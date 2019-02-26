/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_list(void) {

    ostringstream cmd;

    cout << cGreen << "Monitor path: " << cWhite << blockCachePath("monitors/") << endl;
    cmd << "cd " << blockCachePath("monitors/") << " ; ls";

    string_q result = doCommand(cmd.str());

    CStringArray files;
    explode(files, result, '\n');
    sort(files.begin(), files.end());

    size_t widest = 0;
    for (auto file : files)
        if (file.length() > widest)
            widest = file.length();
    size_t nWide = (90 / widest);

    cout << widest << endl;
    cout << cGreen << "Current monitors:" << cOff << endl << "    ";
    cout << cTeal;
    size_t cnt = 0;
    for (auto file : files) {
        if (file != "file") {
            cout << padRight(file, widest + 1);
            if (!(++cnt % nWide))
                cout << endl << "    ";
        }
    }
    cout << endl;

    return true;
}
