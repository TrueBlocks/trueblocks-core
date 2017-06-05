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
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTreeNode : public CBaseNode {
public:
    SFUint32 index;

public:
    CTreeNode(void);
    CTreeNode(const CTreeNode& tr);
    ~CTreeNode(void);
    CTreeNode& operator=(const CTreeNode& tr);

    DECLARE_NODE(CTreeNode);

    // EXISTING_CODE
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

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::Copy(const CTreeNode& tr) {
    Clear();
    CBaseNode::Copy(tr);

    index = tr.index;

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
inline SFString CTreeNode::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTreeNodeArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTreeNodeArray);
IMPLEMENT_ARCHIVE_LIST(CTreeNodeList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CTreeNode& tre);
extern SFArchive& operator>>(SFArchive& archive, CTreeNode& tre);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

