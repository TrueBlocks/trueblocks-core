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
class CAddressPerBlock : public CBaseNode {
public:
    string_q id;
    string_q block_number;
    string_q tx_index;
    string_q trace_id;
    address_t address;
    string_q reason;

public:
    CAddressPerBlock(void);
    CAddressPerBlock(const CAddressPerBlock& ad);
    virtual ~CAddressPerBlock(void);
    CAddressPerBlock& operator=(const CAddressPerBlock& ad);

    DECLARE_NODE(CAddressPerBlock);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAddressPerBlock& item) const;
    bool operator!=(const CAddressPerBlock& item) const { return !operator==(item); }
    friend bool operator<(const CAddressPerBlock& v1, const CAddressPerBlock& v2);
    friend ostream& operator<<(ostream& os, const CAddressPerBlock& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAddressPerBlock& ad);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAddressPerBlock::CAddressPerBlock(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAddressPerBlock::CAddressPerBlock(const CAddressPerBlock& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAddressPerBlock::~CAddressPerBlock(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddressPerBlock::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddressPerBlock::initialize(void) {
    CBaseNode::initialize();

    id = "";
    block_number = "";
    tx_index = "";
    trace_id = "";
    address = "";
    reason = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddressPerBlock::duplicate(const CAddressPerBlock& ad) {
    clear();
    CBaseNode::duplicate(ad);

    id = ad.id;
    block_number = ad.block_number;
    tx_index = ad.tx_index;
    trace_id = ad.trace_id;
    address = ad.address;
    reason = ad.reason;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAddressPerBlock& CAddressPerBlock::operator=(const CAddressPerBlock& ad) {
    duplicate(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAddressPerBlock::operator==(const CAddressPerBlock& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAddressPerBlock& v1, const CAddressPerBlock& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAddressPerBlock> CAddressPerBlockArray;
extern CArchive& operator>>(CArchive& archive, CAddressPerBlockArray& array);
extern CArchive& operator<<(CArchive& archive, const CAddressPerBlockArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

