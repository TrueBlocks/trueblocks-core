#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
class CTreeNode;
using ACCTVISITOR = bool (*)(const CTreeNode* v, void* data);
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTreeNode : public CBaseNode {
  public:
    uint64_t index;
    string_q prefixS;

  public:
    CTreeNode(void);
    CTreeNode(const CTreeNode& tr);
    virtual ~CTreeNode(void);
    CTreeNode& operator=(const CTreeNode& tr);

    DECLARE_NODE(CTreeNode);

    // EXISTING_CODE
    virtual string_q at(const string_q& _key) const {
        return "";
    }
    virtual CTreeNode* insert(const string_q& _key, const string_q& _value) {
        return NULL;
    }
    virtual CTreeNode* remove(const string_q& _key) {
        return NULL;
    }
    virtual bool visitItems(ACCTVISITOR func, void* data) const {
        return true;
    }
    CTreeNode* newBranch(const string_q& _k1, const string_q& _v1, const string_q& _k2, const string_q& _v2);
    // EXISTING_CODE
    bool operator==(const CTreeNode& it) const;
    bool operator!=(const CTreeNode& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTreeNode& v1, const CTreeNode& v2);
    friend ostream& operator<<(ostream& os, const CTreeNode& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTreeNode& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTreeNode::CTreeNode(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTreeNode::CTreeNode(const CTreeNode& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTreeNode::~CTreeNode(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::initialize(void) {
    CBaseNode::initialize();

    index = 0;
    prefixS = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTreeNode::duplicate(const CTreeNode& tr) {
    clear();
    CBaseNode::duplicate(tr);

    index = tr.index;
    prefixS = tr.prefixS;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTreeNode& CTreeNode::operator=(const CTreeNode& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTreeNode::operator==(const CTreeNode& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTreeNode& v1, const CTreeNode& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTreeNode> CTreeNodeArray;
extern CArchive& operator>>(CArchive& archive, CTreeNodeArray& array);
extern CArchive& operator<<(CArchive& archive, const CTreeNodeArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTreeNode& tre);
extern CArchive& operator>>(CArchive& archive, CTreeNode& tre);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TREENODE;

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
inline unsigned commonPrefix(const string_q& _t, const string_q& _u) {
    size_t s = min(_t.length(), _u.length());
    for (size_t i = 0;; ++i)
        if (i == s || (_t[i] != _u[i]))
            return (unsigned)i;
    return (unsigned)s;
}
// EXISTING_CODE
}  // namespace qblocks
