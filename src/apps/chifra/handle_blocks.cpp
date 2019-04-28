/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_blocks(void) {

    ENTER("handle_" + mode);
    LOG1("tool_flags: " + tool_flags);
    nodeNotRequired();

    if (!contains(tool_flags, "--edit")) // weird cleanup of --edit/edit confusion
        tool_flags = substitute(tool_flags, "edit", "--edit");

    ostringstream os;
    os << "whenBlock " << tool_flags << (api_mode ? " --fmt json" : "") << " ; "; // order matters, last in wins

    LOG1("command: " + os.str());
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    EXIT_OK("handle_" + mode);
}
