/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "tree.h"

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

//-----------------------------------------------------------------------------
SFString CAccountTree::debugPrint(void) const {
    CStringExportContext ctx;
    ctx << "accountTree: ";
    if (m_root)
        ctx << m_root->debugPrintBody(idnt + "|-");
    return ctx.str;
}

//-----------------------------------------------------------------------------
void CAccountTree::insert(const SFString& _key, const SFString& _value) {
    if (_value.empty())
        remove(_key);
    m_root = m_root ? m_root->insert(_key, _value) : new acctTree_Leaf(_key, _value);
}

//-----------------------------------------------------------------------------
bool CAccountTree::visitItems(ACCTVISITOR func, void *data) const {
    if (m_root)
        return m_root->visitItems(func, data);
    return true;
}

//------------------------------------------------------------------
bool forEveryAccount(CAccountTree *trie, ACCTVISITOR func, void *data) {
    ASSERT(trie);
    return trie->visitItems(func, data);
}
