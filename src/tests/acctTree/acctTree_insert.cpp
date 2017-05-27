/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//-----------------------------------------------------------------------------
void CAccountTree::insert(const SFString& _key, const SFString& _value) {
    if (_value.empty())
        remove(_key);
    m_root = m_root ? m_root->insert(_key, _value) : new acctTree_Leaf(_key, _value);
}

//-----------------------------------------------------------------------------
acctTree_Node* acctTree_Infix::insert(const SFString& _key, const SFString& _value) {
    if (verbose == 2) cerr << "\tInserting infix " << _key << "\n";
    ASSERT(_value.length());
    if (contains(_key)) {
        m_next = m_next->insert(_key.substr(m_prefix.length()), _value);
        return this;

    } else {
        unsigned prefix = commonPrefix(_key, m_prefix);
        if (prefix) {
            // one infix becomes two infixes, then insert into the second
            // instead of pop_front()...
            m_prefix = m_prefix.substr(prefix);
            return new acctTree_Infix(_key.substr(0, prefix), insert(_key.substr(prefix), _value));

        } else {
            // split here.
            auto f = m_prefix[0];
            m_prefix = m_prefix.substr(1);
            acctTree_Node* n = m_prefix.empty() ? m_next : this;
            if (n != this) {
                m_next = NULL;
                delete this;
            }
            acctTree_Branch* ret = new acctTree_Branch(f, n);
            ret->insert(_key, _value);
            return ret;
        }
    }
}

//-----------------------------------------------------------------------------
acctTree_Node* acctTree_Branch::insert(const SFString& _key, const SFString& _value) {
    if (verbose == 2) cerr << "\tInserting branch " << _key << "\n";
    if (_key.empty()) {
        // We've reached the end of the key, so store the value here
        if (m_branchValue.empty()) {
            // store the first encountered block
            m_branchValue = _value;

        } else {
            // preserve the most recent block encountered
            m_branchValue = nextTokenClear(m_branchValue, '|');
            m_branchValue += "|" + _value;
        }

    } else {
        // Figure out which bucket to store the value in by the next character in the key
        int idx = nodeIndex(_key[0]);
        if (m_nodes[nodeIndex(_key[0])]) {
            // There is already something stored here, so we need to find room for it
            m_nodes[idx] = m_nodes[idx]->insert(_key.substr(1), _value);

        } else {
            // we've reached a leaf
            m_nodes[idx] = new acctTree_Leaf(_key.substr(1), _value);
        }
    }
    return this;
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
