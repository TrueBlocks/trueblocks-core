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
// EXISTING_CODE

//--------------------------------------------------------------------------
class CDatedBlock : public CBaseNode {
  public:
    blknum_t blockNumber;
    timestamp_t timestamp;

  public:
    CDatedBlock(void);
    CDatedBlock(const CDatedBlock& da);
    virtual ~CDatedBlock(void);
    CDatedBlock& operator=(const CDatedBlock& da);

    DECLARE_NODE(CDatedBlock);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDatedBlock& it) const;
    bool operator!=(const CDatedBlock& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CDatedBlock& v1, const CDatedBlock& v2);
    friend ostream& operator<<(ostream& os, const CDatedBlock& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDatedBlock& da);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDatedBlock::CDatedBlock(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDatedBlock::CDatedBlock(const CDatedBlock& da) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(da);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDatedBlock::~CDatedBlock(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDatedBlock::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDatedBlock::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    timestamp = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDatedBlock::duplicate(const CDatedBlock& da) {
    clear();
    CBaseNode::duplicate(da);

    blockNumber = da.blockNumber;
    timestamp = da.timestamp;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDatedBlock& CDatedBlock::operator=(const CDatedBlock& da) {
    duplicate(da);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDatedBlock::operator==(const CDatedBlock& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDatedBlock& v1, const CDatedBlock& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDatedBlock> CDatedBlockArray;
extern CArchive& operator>>(CArchive& archive, CDatedBlockArray& array);
extern CArchive& operator<<(CArchive& archive, const CDatedBlockArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CDatedBlock& dat);
extern CArchive& operator>>(CArchive& archive, CDatedBlock& dat);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_DATEDBLOCK;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
