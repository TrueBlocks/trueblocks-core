#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class acctTree_Node;
using ACCTVISITOR = bool (*)(const acctTree_Node *v, void *data);

//-----------------------------------------------------------------------------
class acctTree_Node {
public:
    acctTree_Node(void);
    virtual ~acctTree_Node(void);

    virtual SFString at(const SFString& _key) const = 0;
    virtual acctTree_Node* insert(const SFString& _key, const SFString& _value) = 0;
    virtual acctTree_Node* remove(const SFString& _key) = 0;
    virtual SFString debugPrintBody(const SFString& _indent) const = 0;
    virtual bool visitItems(ACCTVISITOR func, void *data) const = 0;

protected:
    blknum_t index;
    static acctTree_Node* newBranch(const SFString& _k1, const SFString& _v1, const SFString& _k2, const SFString& _v2);
};

//-----------------------------------------------------------------------------
inline unsigned commonPrefix(const SFString& _t, const SFString& _u) {
    unsigned s = min((unsigned)_t.length(), (unsigned)_u.length());
    for (unsigned i = 0 ; ; ++i)
        if (i == s || (_t[i] != _u[i]))
            return i;
    return s;
}
