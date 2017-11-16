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
#include "leaf.h"
#include "branch.h"
#include "infix.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CTreeRoot;
typedef SFArrayBase<CTreeRoot>         CTreeRootArray;
typedef SFList<CTreeRoot*>             CTreeRootList;
typedef SFUniqueList<CTreeRoot*>       CTreeRootListU;

// EXISTING_CODE
//----------------------------------------------------------
#define T_TOP       (0)
#define T_LEAF      (1)
#define T_BRANCH    (2)
#define T_INFIX     (3)

//----------------------------------------------------------
class CVisitData {
public:
    uint32_t type;
    uint64_t cnt;
    SFString strs;
    CVisitData(void) : type(0), cnt(0) { }
};
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTreeRoot : public CBaseNode {
public:
    CTreeNode *m_root;

public:
    CTreeRoot(void);
    CTreeRoot(const CTreeRoot& tr);
    virtual ~CTreeRoot(void);
    CTreeRoot& operator=(const CTreeRoot& tr);

    DECLARE_NODE(CTreeRoot);

    // EXISTING_CODE
    SFString at(const SFString& _key) const;
    void insert(const SFString& _key, const SFString& _value);
    void remove(const SFString& _key);
    bool visitItems(ACCTVISITOR func, void *data) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CTreeRoot& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTreeRoot& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTreeRoot::CTreeRoot(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTreeRoot::CTreeRoot(const CTreeRoot& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTreeRoot::~CTreeRoot(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeRoot::Clear(void) {
    if (m_root)
        delete m_root;
    m_root = NULL;
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeRoot::Init(void) {
    CBaseNode::Init();

    m_root = NULL;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeRoot::Copy(const CTreeRoot& tr) {
    Clear();
    CBaseNode::Copy(tr);

    if (tr.m_root) {
        m_root = new CTreeNode;
        *m_root = *tr.m_root;
    }

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTreeRoot& CTreeRoot::operator=(const CTreeRoot& tr) {
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTreeRootArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTreeRootArray);
IMPLEMENT_ARCHIVE_LIST(CTreeRootList);

//---------------------------------------------------------------------------
// EXISTING_CODE
//----------------------------------------------------------
inline int nodeIndex(char c) {
    char l = (char)tolower(c);
    int ret =  (l < 'a' ? l - '0' : l - 'a' + 10) % 16;
    return ret;
}

//------------------------------------------------------------------
inline SFString idex(char n) {
    switch (n)     {
        case 0: return "0";
        case 1: return "1";
        case 2: return "2";
        case 3: return "3";
        case 4: return "4";
        case 5: return "5";
        case 6: return "6";
        case 7: return "7";
        case 8: return "8";
        case 9: return "9";
        case 10: return "a";
        case 11: return "b";
        case 12: return "c";
        case 13: return "d";
        case 14: return "e";
        case 15: return "f";
    }
    cerr << "should never happen";
    exit(0);
    return "XXX";
}

//------------------------------------------------------------------
bool forEveryAccount(CTreeRoot *trie, ACCTVISITOR func, void *data);
extern SFString idnt;
// EXISTING_CODE
}  // namespace qblocks

