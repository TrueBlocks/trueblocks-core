/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

#define end "\r"
//------------------------------------------------------------------
SFString acctTree_Infix::debugPrintBody(const SFString& _indent) const {
    CStringExportContext ctx;
    ctx << end;
    ctx << _indent;
    ctx << "infix: ";
    ctx << "m_prefix[" << m_prefix << "]";
    if (m_next)
        ctx << m_next->debugPrintBody(_indent + "--");
    return ctx.str;
}

//------------------------------------------------------------------
SFString acctTree_Branch::debugPrintBody(const SFString& _indent) const {
    CStringExportContext ctx;
    ctx << end;
    ctx << _indent;
    ctx << "branch: ";

    if (m_branchValue.length())
        ctx << "(m_value: " << m_branchValue << ")";

    for (uint32_t i = 0; i < 16; ++i) {
        bool verbose = false;
        if (m_nodes[i])
            ctx << m_nodes[i]->debugPrintBody(_indent.Substitute("|-", "[" + SFString(idex(i)) + "]:") + "|-");
        if (verbose)
            ctx << "[" << idex(i) << "]:" << end;
    }
    return ctx.str;
}

//------------------------------------------------------------------
SFString acctTree_Leaf::debugPrintBody(const SFString& _indent) const {
    CStringExportContext ctx;
    ctx << end;
    ctx << _indent;
    ctx << "leaf: ";

    if (m_prefix.length() > 0)
        ctx << "(remain[" << m_prefix << "] <--> " << m_leafValue << ")";
    else
        ctx << "(leaf: " << m_leafValue << ")";

    return ctx.str;
}

extern SFString idnt;
//-----------------------------------------------------------------------------
SFString CAccountTree::debugPrint(void) const {
    CStringExportContext ctx;
    ctx << "accountTree: ";
    if (m_root)
        ctx << m_root->debugPrintBody(idnt + "|-");
    return ctx.str;
}
