#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "utillib.h"

//-----------------------------------------------------------------------------
class CBlockOptions : public COptionsBase {
public:
    COptionsBlockList blocks;

    CBlockOptions(void);
    SFString getBlockNumList(void) const;
};
