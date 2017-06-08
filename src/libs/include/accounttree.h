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
class CAccountTree;
typedef SFArrayBase<CAccountTree>         CAccountTreeArray;
typedef SFList<CAccountTree*>             CAccountTreeList;
typedef SFUniqueList<CAccountTree*>       CAccountTreeListU;

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
    SFString strs;
    CVisitData(void) { type = 0; }
};
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountTree : public CBaseNode {
public:
    CTreeNode *m_root;

public:
    CAccountTree(void);
    CAccountTree(const CAccountTree& ac);
    ~CAccountTree(void);
    CAccountTree& operator=(const CAccountTree& ac);

    DECLARE_NODE(CAccountTree);

    // EXISTING_CODE
    SFString at(const SFString& _key) const;
    void insert(const SFString& _key, const SFString& _value);
    void insert(const char* _key, blknum_t _value) { insert(SFString(_key).Right(40), asString(_value)); }
    void remove(const SFString& _key);
    bool visitItems(ACCTVISITOR func, void *data) const;
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccountTree& ac);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountTree::CAccountTree(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountTree::CAccountTree(const CAccountTree& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountTree::~CAccountTree(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountTree::Clear(void) {
    // EXISTING_CODE
    if (m_root)
        delete m_root;
    m_root = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountTree::Init(void) {
    CBaseNode::Init();

    m_root = NULL;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountTree::Copy(const CAccountTree& ac) {
    Clear();
    CBaseNode::Copy(ac);

    if (ac.m_root)
        *m_root = *ac.m_root;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccountTree& CAccountTree::operator=(const CAccountTree& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CAccountTree::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountTreeArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountTreeArray);
IMPLEMENT_ARCHIVE_LIST(CAccountTreeList);

//---------------------------------------------------------------------------
// EXISTING_CODE
//----------------------------------------------------------
inline int nodeIndex(char c) {
    char l = tolower(c);
    int ret =  (l < 'a' ? l - '0' : l - 'a' + 10) % 16;
    return ret;
}

//------------------------------------------------------------------
inline SFString idex(uint32_t n) {
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
bool forEveryAccount(CAccountTree *trie, ACCTVISITOR func, void *data);
extern SFString idnt;
// EXISTING_CODE
}  // namespace qblocks

