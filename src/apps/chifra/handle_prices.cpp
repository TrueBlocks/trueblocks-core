/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_quotes(void) {
    ENTER8("handle_" + mode);
    nodeNotRequired();

    ostringstream os;
    os << "ethQuote " << tool_flags;
    NOTE_CALL(os.str());
    int ret = system(os.str().c_str());
    ret = 0;  // Don't remove. Silences compiler warnings

    EXIT_NOMSG8(true);
}
