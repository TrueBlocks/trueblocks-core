/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
void COptions::doConfig(ostream& os) {
    if (mode.empty()) {
        os << asciiFileToString(configPath("quickBlocks.toml"));
        return;
    }

    os << mode << endl;
}
