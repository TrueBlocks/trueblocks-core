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
#include "logentry.h"

//--------------------------------------------------------------------------
class CNewReceipt;
typedef SFArrayBase<CNewReceipt>         CNewReceiptArray;
typedef SFList<CNewReceipt*>             CNewReceiptList;
typedef SFUniqueList<CNewReceipt*>       CNewReceiptListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CNewReceipt : public CBaseNode {
public:
    SFAddress contractAddress;
    SFGas gasUsed;
    CLogEntryArray logs;
    SFBloom logsBloom;
    bool isError;

public:
    CNewReceipt(void);
    CNewReceipt(const CNewReceipt& ne);
    virtual ~CNewReceipt(void);
    CNewReceipt& operator=(const CNewReceipt& ne);

    DECLARE_NODE(CNewReceipt);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    friend class CTransaction;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CNewReceipt& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CNewReceipt& ne);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewReceipt::CNewReceipt(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewReceipt::CNewReceipt(const CNewReceipt& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewReceipt::~CNewReceipt(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewReceipt::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewReceipt::Init(void) {
    CBaseNode::Init();

    contractAddress = "";
    gasUsed = 0;
    logs.Clear();
    logsBloom = 0;
    isError = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewReceipt::Copy(const CNewReceipt& ne) {
    Clear();
    CBaseNode::Copy(ne);

    contractAddress = ne.contractAddress;
    gasUsed = ne.gasUsed;
    logs = ne.logs;
    logsBloom = ne.logsBloom;
    isError = ne.isError;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewReceipt& CNewReceipt::operator=(const CNewReceipt& ne) {
    Copy(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CNewReceiptArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CNewReceiptArray);
IMPLEMENT_ARCHIVE_LIST(CNewReceiptList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CNewReceipt& newp);
extern SFArchive& operator>>(SFArchive& archive, CNewReceipt& newp);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

