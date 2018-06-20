#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "treenode.h"
#include "leaf.h"
#include "branch.h"
#include "infix.h"

namespace qblocks {

// EXISTING_CODE
//----------------------------------------------------------
#define T_TOP       (0)
#define T_LEAF      (1)
#define T_BRANCH    (2)
#define T_INFIX     (3)

//----------------------------------------------------------
class CVisitData {
public:
    uint64_t type;
    uint64_t level;
    uint64_t counter;
    string_q strs;
    CVisitData(void) : type(0), level(0), counter(0) { }
};
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTreeRoot : public CBaseNode {
public:
    CTreeNode *root;

public:
    CTreeRoot(void);
    CTreeRoot(const CTreeRoot& tr);
    virtual ~CTreeRoot(void);
    CTreeRoot& operator=(const CTreeRoot& tr);

    DECLARE_NODE(CTreeRoot);

    // EXISTING_CODE
    string_q at(const string_q& _key) const;
    void insert(const string_q& _key, const string_q& _value);
    void remove(const string_q& _key);
    bool visitItems(ACCTVISITOR func, void *data) const;
    // EXISTING_CODE
    friend bool operator<(const CTreeRoot& v1, const CTreeRoot& v2);
    friend ostream& operator<<(ostream& os, const CTreeRoot& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTreeRoot& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTreeRoot::CTreeRoot(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTreeRoot::CTreeRoot(const CTreeRoot& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTreeRoot::~CTreeRoot(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeRoot::clear(void) {
    if (root)
        delete root;
    root = NULL;
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeRoot::initialize(void) {
    CBaseNode::initialize();

    root = NULL;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeRoot::duplicate(const CTreeRoot& tr) {
    clear();
    CBaseNode::duplicate(tr);

    if (tr.root) {
        root = new CTreeNode;
        *root = *tr.root;
    }

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTreeRoot& CTreeRoot::operator=(const CTreeRoot& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTreeRoot& v1, const CTreeRoot& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef SFArrayBase<CTreeRoot> CTreeRootArray;
extern SFArchive& operator>>(SFArchive& archive, CTreeRootArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CTreeRootArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
//----------------------------------------------------------
inline int nodeIndex(char c) {
    char l = (char)tolower(c);
    int ret =  (l < 'a' ? l - '0' : l - 'a' + 10) % 16;
    return ret;
}

//------------------------------------------------------------------
inline string_q idex(char n) {
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
extern bool forEveryAccount(CTreeRoot *trie, ACCTVISITOR func, void *data);
extern string_q idnt;

//------------------------------------------------------------------
inline CTreeNode *createTreeNode(const string_q& type) {
    if (type == "CInfix")
        return new CInfix;
    else if (type == "CBranch")
        return new CBranch;
    else if (type == "CLeaf")
        return new CLeaf;
    return NULL;
}
// EXISTING_CODE
}  // namespace qblocks

