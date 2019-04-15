/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_names(void) {

    // names mode does not require a running node
    nodeNotRequired();

    if (!contains(tool_flags, "--edit"))
        tool_flags = substitute(tool_flags, "edit", "--edit");

    ostringstream os;
    os << "ethName " << (tool_flags.empty() ? "--data" : tool_flags) << " ; ";
    if (isTestMode())
        cout << os.str() << endl;
    else
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    return true;
}
