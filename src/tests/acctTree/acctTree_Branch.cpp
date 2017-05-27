/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//-----------------------------------------------------------------------------
acctTree_Branch::acctTree_Branch(const SFString& _value) : m_branchValue(_value) {
    memset(m_nodes, 0, sizeof(acctTree_Node*) * 16);
}

//-----------------------------------------------------------------------------
acctTree_Branch::acctTree_Branch(char _i1, acctTree_Node* _n1, const SFString& _value) : m_branchValue(_value) {
    memset(m_nodes, 0, sizeof(acctTree_Node*) * 16);
    m_nodes[nodeIndex(_i1)] = _n1;
}

//-----------------------------------------------------------------------------
acctTree_Branch::acctTree_Branch(char _i1, acctTree_Node* _n1, char _i2, acctTree_Node* _n2) {
    memset(m_nodes, 0, sizeof(acctTree_Node*) * 16);
    m_nodes[nodeIndex(_i1)] = _n1;
    m_nodes[nodeIndex(_i2)] = _n2;
}

//-----------------------------------------------------------------------------
acctTree_Branch::~acctTree_Branch(void) {
    for (int i = 0 ; i < 16 ; i++)
        if (m_nodes[i])
            delete m_nodes[i];

    // unecessary, but okay
    memset(m_nodes, 0, sizeof(acctTree_Node*) * 16);
}
