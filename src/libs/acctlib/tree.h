#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#include "base.h"
#include "leaf.h"
#include "branch.h"
#include "infix.h"

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

//----------------------------------------------------------
extern SFString types[];

//----------------------------------------------------------
class CAccountTree {
public:
    CAccountTree(void);
    ~CAccountTree(void);

    SFString at(const SFString& _key) const;
    void insert(const SFString& _key, const SFString& _value);
    void insert(const char* _key, blknum_t _value) { insert(SFString(_key).Right(40), asString(_value)); }
    void remove(const SFString& _key);
    SFString debugPrint(void) const;
    bool visitItems(ACCTVISITOR func, void *data) const;

private:
    acctTree_Node* m_root;
};

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
