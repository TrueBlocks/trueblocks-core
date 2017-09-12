#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CTreeNode;
typedef SFArrayBase<CTreeNode>         CTreeNodeArray;
typedef SFList<CTreeNode*>             CTreeNodeList;
typedef SFUniqueList<CTreeNode*>       CTreeNodeListU;

// EXISTING_CODE
using ACCTVISITOR = bool (*)(const CTreeNode *v, void *data);
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTreeNode : public CBaseNode {
public:
    SFUint32 index;
    SFString m_prefix;

public:
    CTreeNode(void);
    CTreeNode(const CTreeNode& tr);
    virtual ~CTreeNode(void);
    CTreeNode& operator=(const CTreeNode& tr);

    DECLARE_NODE(CTreeNode);

    // EXISTING_CODE
    virtual SFString at(const SFString& _key) const { return ""; }
    virtual CTreeNode* insert(const SFString& _key, const SFString& _value) { return NULL; }
    virtual CTreeNode* remove(const SFString& _key) { return NULL; }
    virtual bool visitItems(ACCTVISITOR func, void *data) const { return true; }
    CTreeNode* newBranch(const SFString& _k1, const SFString& _v1, const SFString& _k2, const SFString& _v2);
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTreeNode& tr);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTreeNode::CTreeNode(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTreeNode::CTreeNode(const CTreeNode& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTreeNode::~CTreeNode(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::Init(void) {
    CBaseNode::Init();

    index = 0;
    m_prefix = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::Copy(const CTreeNode& tr) {
    Clear();
    CBaseNode::Copy(tr);

    index = tr.index;
    m_prefix = tr.m_prefix;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTreeNode& CTreeNode::operator=(const CTreeNode& tr) {
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTreeNodeArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTreeNodeArray);
IMPLEMENT_ARCHIVE_LIST(CTreeNodeList);

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
inline unsigned commonPrefix(const SFString& _t, const SFString& _u) {
    int32_t s = (int32_t)min(_t.length(), _u.length());
    for (int32_t i = 0 ; ; ++i)
        if (i == s || (_t[i] != _u[i]))
            return (unsigned)i;
    return (unsigned)s;
}
// EXISTING_CODE
#define NEW_CODE
}  // namespace qblocks

