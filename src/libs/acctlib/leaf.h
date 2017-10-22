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
class CLeaf;
typedef SFArrayBase<CLeaf>         CLeafArray;
typedef SFList<CLeaf*>             CLeafList;
typedef SFUniqueList<CLeaf*>       CLeafListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLeaf : public CTreeNode {
public:
    SFBlockArray blocks;
    uint64_t cnt;

public:
    CLeaf(void);
    CLeaf(const CLeaf& le);
    virtual ~CLeaf(void);
    CLeaf& operator=(const CLeaf& le);

    DECLARE_NODE(CLeaf);

    const SFString getStringAt(const SFString& name, uint32_t i) const override;

    // EXISTING_CODE
    CLeaf(const SFString& _key, const SFString& _value);
    SFString at(const SFString& _key) const override;
    CTreeNode* insert(const SFString& _key, const SFString& _value) override;
    CTreeNode* remove(const SFString& _key) override;
    bool visitItems(ACCTVISITOR func, void *data) const override;

private:
    bool contains(const SFString& _key) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CLeaf& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CLeaf& le);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLeaf::CLeaf(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLeaf::CLeaf(const CLeaf& le) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(le);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLeaf::~CLeaf(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLeaf::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLeaf::Init(void) {
    CTreeNode::Init();

    blocks.Clear();
    cnt = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLeaf::Copy(const CLeaf& le) {
    Clear();
    CTreeNode::Copy(le);

    blocks = le.blocks;
    cnt = le.cnt;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CLeaf& CLeaf::operator=(const CLeaf& le) {
    Copy(le);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CLeafArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CLeafArray);
IMPLEMENT_ARCHIVE_LIST(CLeafList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFString nextTreenodeChunk(const SFString& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

