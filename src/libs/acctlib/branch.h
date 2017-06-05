#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "base.h"

//-----------------------------------------------------------------------------
class acctTree_Branch: public acctTree_Node {
public:
    explicit acctTree_Branch(const SFString& _value);
    acctTree_Branch(char _i1, acctTree_Node* _n1, const SFString& _value = "");
    acctTree_Branch(char _i1, acctTree_Node* _n1, char _i2, acctTree_Node* _n2);
    virtual ~acctTree_Branch(void);

    SFString at(const SFString& _key) const override;
    acctTree_Node* insert(const SFString& _key, const SFString& _value) override;
    acctTree_Node* remove(const SFString& _key) override;
    SFString debugPrintBody(const SFString& _indent) const override;
    bool visitItems(ACCTVISITOR func, void *data) const override;

private:
    /// @returns (byte)-1 when no active branches, 16 when multiple active and the index of the active branch otherwise.
    char activeBranch() const;

    acctTree_Node *rejig();
    acctTree_Node *m_nodes[16];
    SFString m_branchValue;
};
