/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_list(void) {
    ostringstream os;
    os << "cd " << blockCachePath("monitors/") << " ; ls";
    cout << cGreen << "Current monitors:" << endl;
    string_q result = substitute(doCommand(os.str().c_str()), "\n", "\t");
    CStringArray files;
    explode(files, result, '\t');
    for (auto file : files)
        if (file != "file")
            cout << cTeal << "\t" << file << cOff << endl;
    return true;
}
