/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_daemon(void) {
    ostringstream os;
    os << "cd " << blockCachePath("monitors/") << " ; ";
    os << "chifraDaemon.py";
    system (os.str().c_str());
    return true;
}
