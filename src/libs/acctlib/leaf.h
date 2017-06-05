#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "ext.h"

//-----------------------------------------------------------------------------
class acctTree_Leaf: public acctTree_Ext {
public:
    acctTree_Leaf(const SFString& _key, const SFString& _value);

    SFString at(const SFString& _key) const override;
    acctTree_Node* insert(const SFString& _key, const SFString& _value) override;
    acctTree_Node* remove(const SFString& _key) override;
    SFString debugPrintBody(const SFString& _indent) const override;
    bool visitItems(ACCTVISITOR func, void *data) const override;

private:
    bool contains(const SFString& _key) const;

    SFString m_leafValue;
};
