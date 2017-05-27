/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//-----------------------------------------------------------------------------
SFString acctTree_Infix::at(const SFString& _key) const {
    ASSERT(m_next);
    return contains(_key) ? m_next->at(_key.substr(m_prefix.length())) : "";
}

//-----------------------------------------------------------------------------
SFString acctTree_Branch::at(const SFString& _key) const {
    if (_key.empty())
        return m_branchValue;

    int idx = nodeIndex(_key[0]);
    if (m_nodes[idx])
        return m_nodes[idx]->at(_key.substr(1));

    return "";
}

//-----------------------------------------------------------------------------
SFString acctTree_Leaf::at(const SFString& _key) const {
    return contains(_key) ? m_leafValue : "";
}
