#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "abilib.h"

namespace qblocks {

//---------------------------------------------------------------------------------------------------
class CWebAPI {
private:
    SFString key;
    SFString provider;
    SFString url;

public:
    CWebAPI(void);
    ~CWebAPI(void);

    bool checkKey(CToml& toml);
    SFString getKey(void) const;
};
}  // namespace qblocks
