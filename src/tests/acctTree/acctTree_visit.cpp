/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//------------------------------------------------------------------
SFString types[] = {
    SFString(cWhite)   + "T_TOPP" + cOff,
    SFString(cRed)     + "T_LEAF" + cOff,
    SFString(bYellow)  + "T_BRAN" + cOff,
    SFString(cGreen)   + "T_INFX" + cOff,
};

//------------------------------------------------------------------
bool acctTree_Infix::visitItems(ACCTVISITOR func, void *data) const {
    ASSERT(func);
    CVisitData *vd = reinterpret_cast<CVisitData*>(data);
    uint32_t save = vd->type;
    vd->type = T_INFIX;
    vd->strs = vd->strs + "+" + m_prefix;
    (*func)(this, data);
    if (m_next)
        m_next->visitItems(func, data);
    nextTokenClearReverse(vd->strs, '+');
    vd->type = save;
    return true;
}

//------------------------------------------------------------------
bool acctTree_Branch::visitItems(ACCTVISITOR func, void *data) const {
    ASSERT(func);
    CVisitData *vd = reinterpret_cast<CVisitData*>(data);
    uint32_t save = vd->type;
    vd->type = T_BRANCH;
    vd->strs = vd->strs + "+" + m_branchValue;
    (*func)(this, data);
    for (uint32_t i = 0; i < 16; ++i) {
        if (m_nodes[i]) {
            vd->strs = vd->strs + "-" + idex(i);
            m_nodes[i]->visitItems(func, data);
            nextTokenClearReverse(vd->strs, '-');

        } else {
//          vd->type = T_LEAF;
//          vd->strs = vd->strs + "-" + idex(i) + "+" + conMagenta + "" + conOff;
//          cerr << types[vd->type] << "(" << vd->type << ")" << vd->strs << endl;
//          nextTokenClearReverse(vd->strs,'-');
        }
    }
    nextTokenClearReverse(vd->strs, '+');
    vd->type = save;
    return true;
}

//------------------------------------------------------------------
bool acctTree_Leaf::visitItems(ACCTVISITOR func, void *data) const {
    ASSERT(func);
    CVisitData *vd = reinterpret_cast<CVisitData*>(data);
    uint32_t save = vd->type;
    vd->type = T_LEAF;
    vd->strs = vd->strs + "+" + (cMagenta+m_prefix + cOff + "|" + cBlue + m_leafValue + cOff);
    (*func)(this, data);
    nextTokenClearReverse(vd->strs, '+');
    vd->type = save;
    return true;
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
