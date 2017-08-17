#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

namespace qblocks {
    //---------------------------------------------------------------------------
    inline bool isTestMode(void) {
        return (SFString(getenv("TEST_MODE")) == "true");
    }
}  // namespace qblocks
