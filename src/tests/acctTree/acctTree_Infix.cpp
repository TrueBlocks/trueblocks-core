/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//-----------------------------------------------------------------------------
acctTree_Infix::acctTree_Infix(const SFString& _key, acctTree_Node* _next) : acctTree_Ext(_key), m_next(_next) {
}

//-----------------------------------------------------------------------------
acctTree_Infix::~acctTree_Infix(void) {
    if (m_next)
        delete m_next;
}
