#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "transaction.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccount : public CBaseNode {
  public:
    address_t addr;
    uint64_t latestPage;
    CTransaction latestTx;
    CTransactionArray transactions;

  public:
    CAccount(void);
    CAccount(const CAccount& ac);
    virtual ~CAccount(void);
    CAccount& operator=(const CAccount& ac);

    DECLARE_NODE(CAccount);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    string_q displayString;
    bool handleCustomFormat(ostream& ctx, const string_q& fmtIn, void* data = NULL) const;
    void markLatest(const CTransaction& trans);
    // EXISTING_CODE
    bool operator==(const CAccount& it) const;
    bool operator!=(const CAccount& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAccount& v1, const CAccount& v2);
    friend ostream& operator<<(ostream& os, const CAccount& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAccount& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccount::CAccount(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccount::CAccount(const CAccount& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccount::~CAccount(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::initialize(void) {
    CBaseNode::initialize();

    addr = "";
    latestPage = 1;
    latestTx = CTransaction();
    transactions.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::duplicate(const CAccount& ac) {
    clear();
    CBaseNode::duplicate(ac);

    addr = ac.addr;
    latestPage = ac.latestPage;
    latestTx = ac.latestTx;
    transactions = ac.transactions;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccount& CAccount::operator=(const CAccount& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAccount::operator==(const CAccount& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAccount& v1, const CAccount& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAccount> CAccountArray;
extern CArchive& operator>>(CArchive& archive, CAccountArray& array);
extern CArchive& operator<<(CArchive& archive, const CAccountArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ACCOUNT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
