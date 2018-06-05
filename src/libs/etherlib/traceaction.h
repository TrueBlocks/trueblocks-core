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
    SFAddress address;
    SFWei balance;
    SFString callType;
    SFAddress from;
    SFGas gas;
    SFString init;
    SFString input;
    SFAddress refundAddress;
    SFAddress to;
    SFWei value;

public:
    CTraceAction(void);
    CTraceAction(const CTraceAction& tr);
    virtual ~CTraceAction(void);
    CTraceAction& operator=(const CTraceAction& tr);

    DECLARE_NODE(CTraceAction);

    // EXISTING_CODE
    friend class CTrace;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CTraceAction& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTraceAction& tr);
    bool readBackLevel(SFArchive& archive) override;

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

    address = "";
    balance = 0;
    callType = "";
    from = "";
    gas = 0;
    init = "";
    input = "";
    refundAddress = "";
    to = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceAction::Copy(const CTraceAction& tr) {
    Clear();
    CBaseNode::Copy(tr);

    address = tr.address;
    balance = tr.balance;
    callType = tr.callType;
    from = tr.from;
    gas = tr.gas;
    init = tr.init;
    input = tr.input;
    refundAddress = tr.refundAddress;
    to = tr.to;
    value = tr.value;

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
}  // namespace qblocks

