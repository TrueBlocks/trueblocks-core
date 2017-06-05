/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "tree.h"

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

//-----------------------------------------------------------------------------
SFString acctTree_Branch::at(const SFString& _key) const {
    if (_key.empty())
        return m_branchValue;

    int idx = nodeIndex(_key[0]);
    if (m_nodes[idx])
        return m_nodes[idx]->at(_key.substr(1));

    return "";
}


//------------------------------------------------------------------
SFString acctTree_Branch::debugPrintBody(const SFString& _indent) const {
    CStringExportContext ctx;
    ctx << "\r";
    ctx << _indent;
    ctx << "branch: ";

    if (m_branchValue.length())
        ctx << "(m_value: " << m_branchValue << ")";

    for (uint32_t i = 0; i < 16; ++i) {
        bool verbose = false;
        if (m_nodes[i])
            ctx << m_nodes[i]->debugPrintBody(_indent.Substitute("|-", "[" + SFString(idex(i)) + "]:") + "|-");
        if (verbose)
            ctx << "[" << idex(i) << "]:" << "\r";
    }
    return ctx.str;
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
