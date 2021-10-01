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
#include "utillib.h"
#include "abi.h"

namespace qblocks {

// EXISTING_CODE
typedef enum {
    ST_NONE = 0,
    ST_BALANCE = (1 << 1),
    ST_NONCE = (1 << 2),
    ST_CODE = (1 << 3),
    ST_STORAGE = (1 << 4),
    ST_DEPLOYED = (1 << 5),
    ST_ACCTTYPE = (1 << 6),
    ST_SOME = (ST_BALANCE | ST_NONCE | ST_CODE | ST_ACCTTYPE),
    ST_ALL = (ST_BALANCE | ST_NONCE | ST_CODE | ST_STORAGE | ST_DEPLOYED | ST_ACCTTYPE)
} ethstate_t;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CEthState : public CBaseNode {
  public:
    blknum_t blockNumber;
    wei_t balance;
    uint64_t nonce;
    string_q code;
    string_q storage;
    address_t address;
    blknum_t deployed;
    string_q accttype;
    CFunction result;

  public:
    CEthState(void);
    CEthState(const CEthState& et);
    virtual ~CEthState(void);
    CEthState& operator=(const CEthState& et);

    DECLARE_NODE(CEthState);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEthState& it) const;
    bool operator!=(const CEthState& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CEthState& v1, const CEthState& v2);
    friend ostream& operator<<(ostream& os, const CEthState& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEthState& et);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEthState::CEthState(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEthState::CEthState(const CEthState& et) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(et);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEthState::~CEthState(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEthState::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEthState::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    balance = 0;
    nonce = 0;
    code = "";
    storage = "";
    address = "";
    deployed = 0;
    accttype = "";
    result = CFunction();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEthState::duplicate(const CEthState& et) {
    clear();
    CBaseNode::duplicate(et);

    blockNumber = et.blockNumber;
    balance = et.balance;
    nonce = et.nonce;
    code = et.code;
    storage = et.storage;
    address = et.address;
    deployed = et.deployed;
    accttype = et.accttype;
    result = et.result;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEthState& CEthState::operator=(const CEthState& et) {
    duplicate(et);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEthState::operator==(const CEthState& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return blockNumber == it.blockNumber;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEthState& v1, const CEthState& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.blockNumber < v2.blockNumber;
}

//---------------------------------------------------------------------------
typedef vector<CEthState> CEthStateArray;
extern CArchive& operator>>(CArchive& archive, CEthStateArray& array);
extern CArchive& operator<<(CArchive& archive, const CEthStateArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CEthState& eth);
extern CArchive& operator>>(CArchive& archive, CEthState& eth);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ETHSTATE;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<address_t, CEthState> CEthStateMap;  // NOLINT
//-------------------------------------------------------------------------
extern wei_t getBalanceAt(const address_t& addr, blknum_t blockNum);
extern bool isContractAt(const address_t& addr, blknum_t blockNum);
extern string_q getCodeAt(const address_t& addr, blknum_t blockNum);
extern string_q getStorageAt(const address_t& addr, uint64_t pos, blknum_t blockNum);
extern uint64_t getNonceAt(const address_t& addr, blknum_t num);
extern blknum_t getDeployBlock(const address_t& addr);
extern bool isArchiveNode(void);
// EXISTING_CODE
}  // namespace qblocks
