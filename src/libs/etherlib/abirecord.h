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

namespace qblocks {

//--------------------------------------------------------------------------
class CABIRecord;
typedef SFArrayBase<CABIRecord>         CABIRecordArray;
typedef SFList<CABIRecord*>             CABIRecordList;
typedef SFUniqueList<CABIRecord*>       CABIRecordListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CABIRecord : public CBaseNode {
public:
    SFString name;
    SFString type;
    bool constant;
    uint64_t inputcnt;
    SFString output0;
    SFString abiitem;
    uint64_t count;
    SFString hash;
    SFString signature;

public:
    CABIRecord(void);
    CABIRecord(const CABIRecord& ab);
    virtual ~CABIRecord(void);
    CABIRecord& operator=(const CABIRecord& ab);

    DECLARE_NODE(CABIRecord);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CABIRecord& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CABIRecord& ab);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CABIRecord::CABIRecord(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CABIRecord::CABIRecord(const CABIRecord& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CABIRecord::~CABIRecord(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CABIRecord::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CABIRecord::Init(void) {
    CBaseNode::Init();

    name = "";
    type = "";
    constant = 0;
    inputcnt = 0;
    output0 = "";
    abiitem = "";
    count = 0;
    hash = "";
    signature = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CABIRecord::Copy(const CABIRecord& ab) {
    Clear();
    CBaseNode::Copy(ab);

    name = ab.name;
    type = ab.type;
    constant = ab.constant;
    inputcnt = ab.inputcnt;
    output0 = ab.output0;
    abiitem = ab.abiitem;
    count = ab.count;
    hash = ab.hash;
    signature = ab.signature;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CABIRecord& CABIRecord::operator=(const CABIRecord& ab) {
    Copy(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CABIRecordArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CABIRecordArray);
IMPLEMENT_ARCHIVE_LIST(CABIRecordList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CABIRecord& abi);
extern SFArchive& operator>>(SFArchive& archive, CABIRecord& abi);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

