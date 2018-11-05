#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include <vector>
#include <map>
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CEraseNode : public CTransaction {
public:
    CStringArray labels;

public:
    CEraseNode(void);
    CEraseNode(const CEraseNode& er);
    virtual ~CEraseNode(void);
    CEraseNode& operator=(const CEraseNode& er);

    DECLARE_NODE(CEraseNode);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEraseNode& item) const;
    bool operator!=(const CEraseNode& item) const { return !operator==(item); }
    friend bool operator<(const CEraseNode& v1, const CEraseNode& v2);
    friend ostream& operator<<(ostream& os, const CEraseNode& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEraseNode& er);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEraseNode::CEraseNode(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEraseNode::CEraseNode(const CEraseNode& er) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(er);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEraseNode::~CEraseNode(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEraseNode::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEraseNode::initialize(void) {
    CTransaction::initialize();

    labels.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEraseNode::duplicate(const CEraseNode& er) {
    clear();
    CTransaction::duplicate(er);

    labels = er.labels;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CEraseNode& CEraseNode::operator=(const CEraseNode& er) {
    duplicate(er);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEraseNode::operator==(const CEraseNode& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEraseNode& v1, const CEraseNode& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEraseNode> CEraseNodeArray;
extern CArchive& operator>>(CArchive& archive, CEraseNodeArray& array);
extern CArchive& operator<<(CArchive& archive, const CEraseNodeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

