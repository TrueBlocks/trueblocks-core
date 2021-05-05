/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------
bool visitUnripeIndexFiles(const string_q& path, void* data) {
    return !shouldQuit();
}
