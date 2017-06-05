/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "tree.h"

//-----------------------------------------------------------------------------
acctTree_Ext::acctTree_Ext(const SFString& _bytes) : m_prefix(_bytes) {
}

//-----------------------------------------------------------------------------
acctTree_Leaf::acctTree_Leaf(const SFString& _key, const SFString& _value) : acctTree_Ext(_key), m_leafValue(_value) {
    if (verbose == 2) cerr << "\t\tCreating leaf " << _key << endl;
}

//-----------------------------------------------------------------------------
SFString acctTree_Leaf::at(const SFString& _key) const {
    return contains(_key) ? m_leafValue : "";
}

//-----------------------------------------------------------------------------
bool acctTree_Leaf::contains(const SFString& _key) const {
    size_t l1 = _key.length();
    size_t l2 = m_prefix.length();
    const char *s1 = (const char*)_key;
    const char *s2 = (const char*)m_prefix;
    bool found = !memcmp(s1, s2, l1);

    return l1 == l2 && found;
}

//------------------------------------------------------------------
SFString acctTree_Leaf::debugPrintBody(const SFString& _indent) const {
    CStringExportContext ctx;
    ctx << "\r";
    ctx << _indent;
    ctx << "leaf: ";

    if (m_prefix.length() > 0)
        ctx << "(remain[" << m_prefix << "] <--> " << m_leafValue << ")";
    else
        ctx << "(leaf: " << m_leafValue << ")";

    return ctx.str;
}

//-----------------------------------------------------------------------------
acctTree_Node* acctTree_Leaf::insert(const SFString& _key, const SFString& _value) {
    if (contains(_key)) {
        // If the leaf exists, we reset the value
        if (verbose) cerr << "\t\tReplacing leaf contents " << _key << "\n";
        // We've reached the end of the key, so store the value here
        if (m_leafValue.empty()) {
            // store the first encountered block
            m_leafValue = _value;

        } else {
            // preserve the most recent block encountered
            m_leafValue = nextTokenClear(m_leafValue, '|');
            m_leafValue += "|" + _value;
        }
        return this;

    } else {
        // If the leaf is not the key, delete convert to a branch
        if (verbose == 2) cerr << "\t\tBranching " << _key << "\n";
        acctTree_Node *n = acctTree_Node::newBranch(_key, _value, m_prefix, m_leafValue);
        delete this;
        return n;
    }
}

//-----------------------------------------------------------------------------
acctTree_Node* acctTree_Leaf::remove(const SFString& _key) {
    if (verbose)
        cerr << endl<< endl<< endl
        << idnt << SFString('-', 80) << endl
        << idnt << SFString('-', 80) << endl
        << idnt << "remove infix at [" << _key << "]: ";

    if (contains(_key)) {
        if (verbose)
            cerr << endl << idnt << "removed leaf node at" << _key << endl;
        delete this;
        return NULL;
    }
    if (verbose)
        cerr << endl << idnt << "no node removed at" << _key << endl;
    return this;
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

