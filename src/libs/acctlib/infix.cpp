/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "tree.h"

//-----------------------------------------------------------------------------
acctTree_Infix::acctTree_Infix(const SFString& _key, acctTree_Node* _next) : acctTree_Ext(_key), m_next(_next) {
}

//-----------------------------------------------------------------------------
acctTree_Infix::~acctTree_Infix(void) {
    if (m_next)
        delete m_next;
}

//-----------------------------------------------------------------------------
SFString acctTree_Infix::at(const SFString& _key) const {
    ASSERT(m_next);
    return contains(_key) ? m_next->at(_key.substr(m_prefix.length())) : "";
}

//-----------------------------------------------------------------------------
bool acctTree_Infix::contains(const SFString& _key) const {
    size_t l1 = _key.length();
    size_t l2 = m_prefix.length();
    const char *s1 = (const char*)_key;
    const char *s2 = (const char*)m_prefix;
    bool found = !memcmp(s1, s2, l2);

    return
    l1 >= l2 && found;
}

//------------------------------------------------------------------
SFString acctTree_Infix::debugPrintBody(const SFString& _indent) const {
    CStringExportContext ctx;
    ctx << "\r";
    ctx << _indent;
    ctx << "infix: ";
    ctx << "m_prefix[" << m_prefix << "]";
    if (m_next)
        ctx << m_next->debugPrintBody(_indent + "--");
    return ctx.str;
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
