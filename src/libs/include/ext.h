#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "base.h"

//-----------------------------------------------------------------------------
class acctTree_Ext: public acctTree_Node {
public:
    explicit acctTree_Ext(const SFString& _bytes);

    SFString m_prefix;
};
