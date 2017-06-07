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
class CLeaf: public CTreeNode {
public:
    CLeaf(const SFString& _key, const SFString& _value);

    SFString at(const SFString& _key) const override;
    CTreeNode* insert(const SFString& _key, const SFString& _value) override;
    CTreeNode* remove(const SFString& _key) override;
    bool visitItems(ACCTVISITOR func, void *data) const override;

private:
    bool contains(const SFString& _key) const;

    SFString m_leafValue;
};

}  // namespace qblocks
