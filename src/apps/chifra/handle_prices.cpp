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
    LOG_CALL(os.str());
    // clang-format off
    if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on
    EXIT_NOMSG8(true);
}
