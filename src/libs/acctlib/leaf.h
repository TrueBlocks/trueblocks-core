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

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLeaf : public CTreeNode {
public:
    CBlockNumArray blocks;
    uint64_t counter;

public:
    CLeaf(void);
    CLeaf(const CLeaf& le);
    virtual ~CLeaf(void);
    CLeaf& operator=(const CLeaf& le);

    DECLARE_NODE(CLeaf);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    CLeaf(const string_q& _key, const string_q& _value);
    string_q at(const string_q& _key) const override;
    CTreeNode* insert(const string_q& _key, const string_q& _value) override;
    CTreeNode* remove(const string_q& _key) override;
    bool visitItems(ACCTVISITOR func, void *data) const override;
private:
    bool contains(const string_q& _key) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CLeaf& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLeaf& le);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLeaf::CLeaf(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLeaf::CLeaf(const CLeaf& le) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(le);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLeaf::~CLeaf(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLeaf::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLeaf::initialize(void) {
    CTreeNode::initialize();

    blocks.clear();
    counter = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLeaf::duplicate(const CLeaf& le) {
    clear();
    CTreeNode::duplicate(le);

    blocks = le.blocks;
    counter = le.counter;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CLeaf& CLeaf::operator=(const CLeaf& le) {
    duplicate(le);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
extern SFArchive& operator>>(SFArchive& archive, CLeafArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CLeafArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern string_q nextTreenodeChunk(const string_q& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

