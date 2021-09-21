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
class CCachedAccount : public CBaseNode {
  public:
    address_t addr;
    uint64_t latestPage;
    CTransaction latestTx;
    CTransactionArray transactions;

  public:
    CCachedAccount(void);
    CCachedAccount(const CCachedAccount& ca);
    virtual ~CCachedAccount(void);
    CCachedAccount& operator=(const CCachedAccount& ca);

    DECLARE_NODE(CCachedAccount);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    string_q displayString;
    bool handleCustomFormat(ostream& ctx, const string_q& fmtIn, void* data = NULL) const;
    void markLatest(const CTransaction& trans);
    // EXISTING_CODE
    bool operator==(const CCachedAccount& it) const;
    bool operator!=(const CCachedAccount& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CCachedAccount& v1, const CCachedAccount& v2);
    friend ostream& operator<<(ostream& os, const CCachedAccount& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCachedAccount& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCachedAccount::CCachedAccount(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCachedAccount::CCachedAccount(const CCachedAccount& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCachedAccount::~CCachedAccount(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCachedAccount::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCachedAccount::initialize(void) {
    CBaseNode::initialize();

    addr = "";
    latestPage = 1;
    latestTx = CTransaction();
    transactions.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCachedAccount::duplicate(const CCachedAccount& ca) {
    clear();
    CBaseNode::duplicate(ca);

    addr = ca.addr;
    latestPage = ca.latestPage;
    latestTx = ca.latestTx;
    transactions = ca.transactions;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCachedAccount& CCachedAccount::operator=(const CCachedAccount& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCachedAccount::operator==(const CCachedAccount& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCachedAccount& v1, const CCachedAccount& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCachedAccount> CCachedAccountArray;
extern CArchive& operator>>(CArchive& archive, CCachedAccountArray& array);
extern CArchive& operator<<(CArchive& archive, const CCachedAccountArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CCachedAccount& cac);
extern CArchive& operator>>(CArchive& archive, CCachedAccount& cac);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CACHEDACCOUNT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
