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
#include "ethtypes.h"
#include "abilib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CLogEntry;
typedef SFArrayBase<CLogEntry>         CLogEntryArray;
typedef SFList<CLogEntry*>             CLogEntryList;
typedef SFUniqueList<CLogEntry*>       CLogEntryListU;

// EXISTING_CODE
class CReceipt;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLogEntry : public CBaseNode {
public:
    SFAddress address;
    SFString data;
    SFUint32 logIndex;
    SFBigUintArray topics;

public:
    CLogEntry(void);
    CLogEntry(const CLogEntry& lo);
    ~CLogEntry(void);
    CLogEntry& operator=(const CLogEntry& lo);

    DECLARE_NODE(CLogEntry);

    // EXISTING_CODE
    const CReceipt *pReceipt;
#if 0
    SFHash blockHash;
    uint32_t blockNumber;
    SFHash transactionHash;
    uint32_t transactionIndex;
#endif
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CLogEntry& lo);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogEntry::CLogEntry(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogEntry::CLogEntry(const CLogEntry& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogEntry::~CLogEntry(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::Init(void) {
    CBaseNode::Init();

    address = "";
    data = "";
    logIndex = 0;
    topics.Clear();

    // EXISTING_CODE
#if 0
    blockHash = "";
    blockNumber = 0;
    transactionHash = "";
    transactionIndex = 0;
#endif
    pReceipt = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::Copy(const CLogEntry& lo) {
    Clear();
    CBaseNode::Copy(lo);

    address = lo.address;
    data = lo.data;
    logIndex = lo.logIndex;
    topics = lo.topics;

    // EXISTING_CODE
#if 0
    blockHash = lo.blockHash;
    blockNumber = lo.blockNumber;
    transactionHash = lo.transactionHash;
    transactionIndex = lo.transactionIndex;
#endif
    // no deep copy because it's const
    pReceipt = lo.pReceipt;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CLogEntry& CLogEntry::operator=(const CLogEntry& lo) {
    Copy(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CLogEntry::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CLogEntryArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CLogEntryArray);
IMPLEMENT_ARCHIVE_LIST(CLogEntryList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CLogEntry& log);
extern SFArchive& operator>>(SFArchive& archive, CLogEntry& log);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFString nextReceiptChunk(const SFString& fieldIn, bool& force, const void *data);
extern SFString nextLogentryChunk(const SFString& fieldIn, bool& force, const void *data);
// EXISTING_CODE
}  // namespace qblocks

