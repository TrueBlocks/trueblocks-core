#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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

