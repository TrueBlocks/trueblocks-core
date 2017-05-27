/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//----------------------------------------------------------
CAccountTree::CAccountTree(void) {
    m_root = NULL;
}

//-----------------------------------------------------------------------------
CAccountTree::~CAccountTree() {
    if (m_root)
        delete m_root;
    m_root = NULL;
}

//-----------------------------------------------------------------------------
SFString CAccountTree::at(const SFString& _key) const {
    if (!m_root)
        return "";
    return m_root->at(_key);
}

//-----------------------------------------------------------------------------
void CAccountTree::remove(const SFString& _key) {
    if (m_root)
        m_root = m_root->remove(_key);
}
