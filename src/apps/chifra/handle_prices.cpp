/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_quotes(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    LOG5("tool_flags: " + tool_flags);
    ostringstream os;
    os << "ethQuote " << tool_flags << " ; ";
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    EXIT_NOMSG4(true);
}
