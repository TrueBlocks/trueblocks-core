#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
    uint64_t index;
    SFString prefixS;

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
    friend ostream& operator<<(ostream& os, const CTreeNode& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTreeNode& tr);
    bool readBackLevel(SFArchive& archive) override;

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
    prefixS = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::Copy(const CTreeNode& tr) {
    Clear();
    CBaseNode::Copy(tr);

    index = tr.index;
    prefixS = tr.prefixS;

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
}  // namespace qblocks

