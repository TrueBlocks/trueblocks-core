/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "leaf.h"
#include "tree.h"

namespace qblocks {

    //-----------------------------------------------------------------------------
    CLeaf::CLeaf(const SFString& _key, const SFString& _value) : m_leafValue(_value) {
        m_prefix = _key;
        if (verbose == 2) cerr << "\t\tCreating leaf " << _key << endl;
    }

    //-----------------------------------------------------------------------------
    SFString CLeaf::at(const SFString& _key) const {
        return contains(_key) ? m_leafValue : "";
    }

    //-----------------------------------------------------------------------------
    bool CLeaf::contains(const SFString& _key) const {
        size_t l1 = _key.length();
        size_t l2 = m_prefix.length();
        const char *s1 = (const char*)_key;
        const char *s2 = (const char*)m_prefix;
        bool found = !memcmp(s1, s2, l1);

        return l1 == l2 && found;
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CLeaf::insert(const SFString& _key, const SFString& _value) {
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
            CTreeNode *n = CTreeNode::newBranch(_key, _value, m_prefix, m_leafValue);
            delete this;
            return n;
        }
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CLeaf::remove(const SFString& _key) {
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
    bool CLeaf::visitItems(ACCTVISITOR func, void *data) const {
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
    
}  // namespace qblocks
