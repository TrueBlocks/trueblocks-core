/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

SFString idnt = "";

//-----------------------------------------------------------------------------
acctTree_Node* acctTree_Infix::remove(const SFString& _key) {
    if (verbose) {
        cerr << endl << endl<< endl
            << idnt << SFString('-', 80) << endl
            << idnt << SFString('-', 80) << endl
            << idnt << "remove infix [" << m_prefix << "] at [" << _key << "]: ";
        idnt+="\t";
    }

    if (contains(_key)) {
        SFString newKey = _key.substr(m_prefix.length());
        m_next = m_next->remove(newKey);
        if (auto p = dynamic_cast<acctTree_Ext*>(m_next)) {
            // merge with child...
            p->m_prefix = m_prefix + p->m_prefix;
            m_next = nullptr;
            delete this;
            if (verbose) cerr << idnt << "removed infix replaced with [" << p->m_prefix << "]";
            idnt.Replace("\t", "");
            return p;
        }

        if (!m_next) {
            // we've cleaned up all the children
            delete this;
            idnt.Replace("\t", "");
            return NULL;
        }
    }
    idnt.Replace("\t", "");
    return this;
}

//-----------------------------------------------------------------------------
acctTree_Node* acctTree_Branch::remove(const SFString& _key) {
    if (verbose) {
        cerr << endl<< endl<< endl
                << idnt << SFString('-', 80) << endl
                << idnt << SFString('-', 80) << endl
                << idnt << "remove branch at [" << _key << "]: ";
        idnt+="\t";
    }

    if (_key.empty()) {
        if (m_branchValue.length()) {
            if (verbose)
                cerr << "and non-empty value " << endl;
            m_branchValue = "";
            return rejig();
        }
        if (verbose)
            cerr << "and empty value" << endl;
        idnt.Replace("\t", "");
        return this;
    }

    int idx = nodeIndex(_key[0]);
    if (m_nodes[idx]) {
        const char *k = (const char*)_key;
        acctTree_Node *ret = m_nodes[idx]->remove(&k[1]);
        m_nodes[idx] = ret;
        acctTree_Node *n = rejig();
        if (verbose)
            idnt.Replace("\t", "");
        return n;
    }

    if (verbose) {
        cerr << endl;
        idnt.Replace("\t", "");
    }
    return this;
}

//-----------------------------------------------------------------------------
#define NO_BRANCHS   ((char)-1)
#define MULTI_BRANCH ((char)16)
acctTree_Node *acctTree_Branch::rejig() {
    char n = activeBranch();
    if (n == NO_BRANCHS && m_branchValue.length()) {
        if (verbose)
            cerr << "No branches, but we have a value, so we save it as a leaf" << endl;
        // revert back down to a leaf
        acctTree_Node *r = new acctTree_Leaf("", m_branchValue);
        delete this;
        return r;

    } else if (n != MULTI_BRANCH && m_branchValue.empty()) {
        if (verbose == 2)
            cerr << idnt << "This single (empty) branch has a single child at [" << idex(n) << "]" << endl;
        // only branching to n...
        int nn = static_cast<int>(n);
        if (auto b = dynamic_cast<acctTree_Branch*>(m_nodes[nn])) {
            // switch to infix
            m_nodes[nn] = NULL;
            delete this;
            SFString x = idex(n);
            return new acctTree_Infix(x, b);

        } else {
            auto x = dynamic_cast<acctTree_Ext*>(m_nodes[nn]);
            ASSERT(x);
            // include in child
            x->m_prefix = idex(n) + x->m_prefix;
            m_nodes[nn] = NULL;
            delete this;
            return x;
        }
    }

    return this;
}

//-----------------------------------------------------------------------------
/// @returns (byte)-1 when no active branches, 16 when multiple active and the index of the active branch otherwise.
char acctTree_Branch::activeBranch() const {
    char n = NO_BRANCHS;
    for (int i = 0 ; i < 16 ; i++) {
        if (m_nodes[i]) {
            if (n == NO_BRANCHS)
                n = static_cast<char>(i);
            else
                return MULTI_BRANCH;
        }
    }
    return n;
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
