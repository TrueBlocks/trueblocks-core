#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CNewBlock : public CBaseNode {
public:
    gas_t gasLimit;
    gas_t gasUsed;
    hash_t hash;
    blknum_t blockNumber;
    hash_t parentHash;
    address_t miner;
    uint64_t difficulty;
    double price;
    bool finalized;
    timestamp_t timestamp;
    CTransactionArray transactions;

public:
    CNewBlock(void);
    CNewBlock(const CNewBlock& ne);
    virtual ~CNewBlock(void);
    CNewBlock& operator=(const CNewBlock& ne);

    DECLARE_NODE(CNewBlock);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    explicit CNewBlock(const CBlock& block);
    // EXISTING_CODE
    bool operator==(const CNewBlock& item) const;
    bool operator!=(const CNewBlock& item) const { return !operator==(item); }
    friend bool operator<(const CNewBlock& v1, const CNewBlock& v2);
    friend ostream& operator<<(ostream& os, const CNewBlock& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewBlock& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewBlock::CNewBlock(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewBlock::CNewBlock(const CNewBlock& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewBlock::~CNewBlock(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBlock::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBlock::initialize(void) {
    CBaseNode::initialize();

    gasLimit = 0;
    gasUsed = 0;
    hash = "";
    blockNumber = 0;
    parentHash = "";
    miner = "";
    difficulty = 0;
    price = 0.0;
    finalized = 0;
    timestamp = 0;
    transactions.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBlock::duplicate(const CNewBlock& ne) {
    clear();
    CBaseNode::duplicate(ne);

    gasLimit = ne.gasLimit;
    gasUsed = ne.gasUsed;
    hash = ne.hash;
    blockNumber = ne.blockNumber;
    parentHash = ne.parentHash;
    miner = ne.miner;
    difficulty = ne.difficulty;
    price = ne.price;
    finalized = ne.finalized;
    timestamp = ne.timestamp;
    transactions = ne.transactions;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewBlock& CNewBlock::operator=(const CNewBlock& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewBlock::operator==(const CNewBlock& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewBlock& v1, const CNewBlock& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewBlock> CNewBlockArray;
extern CArchive& operator>>(CArchive& archive, CNewBlockArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewBlockArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CNewBlock& newp);
extern CArchive& operator>>(CArchive& archive, CNewBlock& newp);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_NEWBLOCK;

//---------------------------------------------------------------------------
// EXISTING_CODE
bool readOneNewBlock_fromBinary(CNewBlock& block, const string_q& fileName);
bool readOneNewBlock_fromJson(CNewBlock& block, const string_q& fileName);
// EXISTING_CODE

