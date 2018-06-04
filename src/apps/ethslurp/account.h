#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "transaction.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CAccount;
typedef SFArrayBase<CAccount>         CAccountArray;
typedef SFList<CAccount*>             CAccountList;
typedef SFUniqueList<CAccount*>       CAccountListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccount : public CBaseNode {
public:
    SFAddress addr;
    SFString header;
    SFString displayString;
    uint64_t pageSize;
    uint64_t lastPage;
    int64_t lastBlock;
    uint64_t nVisible;
    CTransactionArray transactions;

public:
    CAccount(void);
    CAccount(const CAccount& ac);
    virtual ~CAccount(void);
    CAccount& operator=(const CAccount& ac);

    DECLARE_NODE(CAccount);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    CAbi abi;
    uint32_t deleteNotShowing(void);
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CAccount& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccount& ac);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccount::CAccount(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccount::CAccount(const CAccount& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccount::~CAccount(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::Clear(void) {
    // EXISTING_CODE
    abi.abiByName.Clear();
    abi.abiByEncoding.Clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::Init(void) {
    CBaseNode::Init();

    addr = "";
    header = "";
    displayString = "";
    pageSize = 0;
    lastPage = 0;
    lastBlock = -1;
    nVisible = 0;
    transactions.Clear();

    // EXISTING_CODE
    abi.Init();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::Copy(const CAccount& ac) {
    Clear();
    CBaseNode::Copy(ac);

    addr = ac.addr;
    header = ac.header;
    displayString = ac.displayString;
    pageSize = ac.pageSize;
    lastPage = ac.lastPage;
    lastBlock = ac.lastBlock;
    nVisible = ac.nVisible;
    transactions = ac.transactions;

    // EXISTING_CODE
    abi = ac.abi;
    abi.abiByName = ac.abi.abiByName;
    abi.abiByEncoding = ac.abi.abiByEncoding;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccount& CAccount::operator=(const CAccount& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountArray);
IMPLEMENT_ARCHIVE_LIST(CAccountList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern uint64_t verbose;
#define REP_FREQ   11
#define REP_INFREQ 563
// EXISTING_CODE
}  // namespace qblocks

