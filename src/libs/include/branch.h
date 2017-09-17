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
#include "treenode.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CBranch;
typedef SFArrayBase<CBranch>         CBranchArray;
typedef SFList<CBranch*>             CBranchList;
typedef SFUniqueList<CBranch*>       CBranchListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBranch : public CTreeNode {
public:
    SFString m_branchValue;

public:
    CBranch(void);
    CBranch(const CBranch& br);
    virtual ~CBranch(void);
    CBranch& operator=(const CBranch& br);

    DECLARE_NODE(CBranch);

    // EXISTING_CODE
    CTreeNode *m_nodes[16];
    explicit CBranch(const SFString& _value);
    CBranch(char _i1, CTreeNode* _n1, const SFString& _value = "");
    CBranch(char _i1, CTreeNode* _n1, char _i2, CTreeNode* _n2);
    SFString at(const SFString& _key) const override;
    CTreeNode* insert(const SFString& _key, const SFString& _value) override;
    CTreeNode* remove(const SFString& _key) override;
    bool visitItems(ACCTVISITOR func, void *data) const override;

private:
    char activeBranch() const;
    CTreeNode *rejig();
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CBranch& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CBranch& br);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBranch::CBranch(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBranch::CBranch(const CBranch& br) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(br);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBranch::~CBranch(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBranch::Clear(void) {
    // EXISTING_CODE
    for (int i = 0 ; i < 16 ; i++)
        if (m_nodes[i])
            delete m_nodes[i];
    memset(m_nodes, 0, sizeof(CTreeNode*) * 16);
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBranch::Init(void) {
    CTreeNode::Init();

    m_branchValue = "";

    // EXISTING_CODE
    memset(m_nodes, 0, sizeof(CTreeNode*) * 16);
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBranch::Copy(const CBranch& br) {
    Clear();
    CTreeNode::Copy(br);

    m_branchValue = br.m_branchValue;

    // EXISTING_CODE
    for (int i = 0 ; i < 16 ; i++)
        if (br.m_nodes[i])
            *m_nodes[i] = *br.m_nodes[i];
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBranch& CBranch::operator=(const CBranch& br) {
    Copy(br);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CBranchArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CBranchArray);
IMPLEMENT_ARCHIVE_LIST(CBranchList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFString nextTreenodeChunk(const SFString& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

