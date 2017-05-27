/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//-----------------------------------------------------------------------------
acctTree_Ext::acctTree_Ext(const SFString& _bytes) : m_prefix(_bytes) {
}

//-----------------------------------------------------------------------------
acctTree_Leaf::acctTree_Leaf(const SFString& _key, const SFString& _value) : acctTree_Ext(_key), m_leafValue(_value) {
    if (verbose == 2) cerr << "\t\tCreating leaf " << _key << endl;
}
