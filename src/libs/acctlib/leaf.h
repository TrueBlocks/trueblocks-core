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
    uint32_t counter;

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
    counter = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLeaf::Copy(const CLeaf& le) {
    Clear();
    CTreeNode::Copy(le);

    blocks = le.blocks;
    counter = le.counter;

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

