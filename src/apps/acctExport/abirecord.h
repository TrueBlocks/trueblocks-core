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
#include "etherlib.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAbiRecord : public CBaseNode {
public:
    uint64_t index;
    string_q symbol;
    address_t addr;
    hash_t encoding;
    string_q type;
    bool constant;
    string_q name;
    string_q signature;
    string_q variables;

public:
    CAbiRecord(void);
    CAbiRecord(const CAbiRecord& ab);
    virtual ~CAbiRecord(void);
    CAbiRecord& operator=(const CAbiRecord& ab);

    DECLARE_NODE(CAbiRecord);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAbiRecord& item) const;
    bool operator!=(const CAbiRecord& item) const { return !operator==(item); }
    friend bool operator<(const CAbiRecord& v1, const CAbiRecord& v2);
    friend ostream& operator<<(ostream& os, const CAbiRecord& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAbiRecord& ab);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbiRecord::CAbiRecord(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbiRecord::CAbiRecord(const CAbiRecord& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbiRecord::~CAbiRecord(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiRecord::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiRecord::initialize(void) {
    CBaseNode::initialize();

    index = 0;
    symbol = "";
    addr = "";
    encoding = "";
    type = "";
    constant = 0;
    name = "";
    signature = "";
    variables = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbiRecord::duplicate(const CAbiRecord& ab) {
    clear();
    CBaseNode::duplicate(ab);

    index = ab.index;
    symbol = ab.symbol;
    addr = ab.addr;
    encoding = ab.encoding;
    type = ab.type;
    constant = ab.constant;
    name = ab.name;
    signature = ab.signature;
    variables = ab.variables;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAbiRecord& CAbiRecord::operator=(const CAbiRecord& ab) {
    duplicate(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAbiRecord::operator==(const CAbiRecord& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAbiRecord& v1, const CAbiRecord& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAbiRecord> CAbiRecordArray;
extern CArchive& operator>>(CArchive& archive, CAbiRecordArray& array);
extern CArchive& operator<<(CArchive& archive, const CAbiRecordArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

