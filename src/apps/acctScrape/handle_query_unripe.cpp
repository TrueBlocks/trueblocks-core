/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitUnripeIndexFiles(const string_q& path, void *data) {
    return !shouldQuit();
}
