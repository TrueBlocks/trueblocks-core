#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "treenode.h"

namespace qblocks {

//-----------------------------------------------------------------------------
class CBranch: public CTreeNode {
public:
    explicit CBranch(const SFString& _value);
    CBranch(char _i1, CTreeNode* _n1, const SFString& _value = "");
    CBranch(char _i1, CTreeNode* _n1, char _i2, CTreeNode* _n2);
    virtual ~CBranch(void);

    SFString at(const SFString& _key) const override;
    CTreeNode* insert(const SFString& _key, const SFString& _value) override;
    CTreeNode* remove(const SFString& _key) override;
    bool visitItems(ACCTVISITOR func, void *data) const override;

private:
    /// @returns (byte)-1 when no active branches, 16 when multiple active and the index of the active branch otherwise.
    char activeBranch() const;

    CTreeNode *rejig();
    CTreeNode *m_nodes[16];
    SFString m_branchValue;
};

}  // namespace qblocks
