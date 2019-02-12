/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_export(void) {
    string_q monitor = toLower(nextTokenClear(remainder, '|'));
    ostringstream os;
    os << "cd " << blockCachePath("monitors/" + monitor + "/") << " ; ";
    os << "acctExport" << " " << substitute(remainder, "|", " ");
    system (os.str().c_str());
    return true;
}
