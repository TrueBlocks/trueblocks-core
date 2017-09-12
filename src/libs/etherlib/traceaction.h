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
class CTraceAction;
typedef SFArrayBase<CTraceAction>         CTraceActionArray;
typedef SFList<CTraceAction*>             CTraceActionList;
typedef SFUniqueList<CTraceAction*>       CTraceActionListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTraceAction : public CBaseNode {
public:
    SFString callType;
    SFAddress from;
    SFGas gas;
    SFString input;
    SFAddress to;
    SFWei value;
    SFAddress address;
    SFWei balance;
    SFAddress refundAddress;

public:
    CTraceAction(void);
    CTraceAction(const CTraceAction& tr);
    ~CTraceAction(void);
    CTraceAction& operator=(const CTraceAction& tr);

    DECLARE_NODE(CTraceAction);

    // EXISTING_CODE
    friend class CTrace;
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTraceAction& tr);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTraceAction::CTraceAction(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceAction::CTraceAction(const CTraceAction& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTraceAction::~CTraceAction(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceAction::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceAction::Init(void) {
    CBaseNode::Init();

    callType = "";
    from = "";
    gas = 0;
    input = "";
    to = "";
    value = 0;
    address = "";
    balance = 0;
    refundAddress = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceAction::Copy(const CTraceAction& tr) {
    Clear();
    CBaseNode::Copy(tr);

    callType = tr.callType;
    from = tr.from;
    gas = tr.gas;
    input = tr.input;
    to = tr.to;
    value = tr.value;
    address = tr.address;
    balance = tr.balance;
    refundAddress = tr.refundAddress;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTraceAction& CTraceAction::operator=(const CTraceAction& tr) {
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTraceActionArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTraceActionArray);
IMPLEMENT_ARCHIVE_LIST(CTraceActionList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CTraceAction& tra);
extern SFArchive& operator>>(SFArchive& archive, CTraceAction& tra);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE
}  // namespace qblocks

