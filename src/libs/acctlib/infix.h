#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
    CInfix(const string_q& _key, CTreeNode* _next) : next(_next) {
        prefixS = _key;
    }
    string_q at(const string_q& _key) const override;
    CTreeNode* insert(const string_q& _key, const string_q& _value) override;
    CTreeNode* remove(const string_q& _key) override;
    bool visitItems(ACCTVISITOR func, void *data) const override;
    bool contains(const string_q& _key) const;
    // EXISTING_CODE
    bool operator==(const CInfix& item) const;
    bool operator!=(const CInfix& item) const { return !operator==(item); }
    friend bool operator<(const CInfix& v1, const CInfix& v2);
    friend ostream& operator<<(ostream& os, const CInfix& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CInfix& in);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CInfix::CInfix(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CInfix::CInfix(const CInfix& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CInfix::~CInfix(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInfix::clear(void) {
    if (next)
        delete next;
    next = NULL;
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInfix::initialize(void) {
    CTreeNode::initialize();

    next = NULL;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CInfix::duplicate(const CInfix& in) {
    clear();
    CTreeNode::duplicate(in);

    if (in.next) {
        next = new CTreeNode;
        *next = *in.next;
    }

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CInfix& CInfix::operator=(const CInfix& in) {
    duplicate(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CInfix::operator==(const CInfix& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CInfix& v1, const CInfix& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CInfix> CInfixArray;
extern CArchive& operator>>(CArchive& archive, CInfixArray& array);
extern CArchive& operator<<(CArchive& archive, const CInfixArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_INFIX;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern string_q nextTreenodeChunk(const string_q& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

