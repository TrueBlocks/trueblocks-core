/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool handle_raw(blknum_t bn, void *data) {

    // TODO(tjayrush): This command requires the $PATH to be set properly
    cout << doCommand("getBloom " + uint_2_Str(bn) + " --raw") << "\n";
    return !shouldQuit();

}
