/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_status(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    LOG5("tool_flags: " + tool_flags);
    ostringstream os;
    os << "cacheStatus " << tool_flags;
    LOG_INFO("chifra calling: ", os.str());
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    EXIT_NOMSG4(true);
}
