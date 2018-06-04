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
#include "treenode.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CInfix;
typedef SFArrayBase<CInfix>         CInfixArray;
typedef SFList<CInfix*>             CInfixList;
typedef SFUniqueList<CInfix*>       CInfixListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CInfix : public CTreeNode {
public:
    CTreeNode *next;

public:
    CInfix(void);
    CInfix(const CInfix& in);
    virtual ~CInfix(void);
    CInfix& operator=(const CInfix& in);

    DECLARE_NODE(CInfix);

    // EXISTING_CODE
    CInfix(const SFString& _key, CTreeNode* _next) : next(_next) {
        prefixS = _key;
    }
    SFString at(const SFString& _key) const override;
    CTreeNode* insert(const SFString& _key, const SFString& _value) override;
    CTreeNode* remove(const SFString& _key) override;
    bool visitItems(ACCTVISITOR func, void *data) const override;
    bool contains(const SFString& _key) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CInfix& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CInfix& in);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CInfix::CInfix(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CInfix::CInfix(const CInfix& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CInfix::~CInfix(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInfix::Clear(void) {
    if (next)
        delete next;
    next = NULL;
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInfix::Init(void) {
    CTreeNode::Init();

    next = NULL;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInfix::Copy(const CInfix& in) {
    Clear();
    CTreeNode::Copy(in);

    if (in.next) {
        next = new CTreeNode;
        *next = *in.next;
    }

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CInfix& CInfix::operator=(const CInfix& in) {
    Copy(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CInfixArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CInfixArray);
IMPLEMENT_ARCHIVE_LIST(CInfixList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFString nextTreenodeChunk(const SFString& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

