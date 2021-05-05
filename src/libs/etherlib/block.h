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
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "transaction.h"

namespace qblocks {

// EXISTING_CODE
typedef bool (*TRANSFUNC)(const CTransaction* trans, void* data);
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBlock : public CBaseNode {
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
    string_q name;
    bool light;

  public:
    CBlock(void);
    CBlock(const CBlock& bl);
    virtual ~CBlock(void);
    CBlock& operator=(const CBlock& bl);

    DECLARE_NODE(CBlock);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    bool forEveryAppearanceInBlock(APPEARANCEFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool forEveryUniqueAppearanceInBlock(APPEARANCEFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool forEveryUniqueAppearanceInBlockPerTx(APPEARANCEFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    // EXISTING_CODE
    bool operator==(const CBlock& it) const;
    bool operator!=(const CBlock& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CBlock& v1, const CBlock& v2);
    friend ostream& operator<<(ostream& os, const CBlock& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBlock& bl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBlock::CBlock(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlock::CBlock(const CBlock& bl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBlock::~CBlock(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::initialize(void) {
    CBaseNode::initialize();

    gasLimit = 0;
    gasUsed = 0;
    hash = "";
    blockNumber = 0;
    parentHash = "";
    miner = "";
    difficulty = 0;
    price = 0.0;
    finalized = false;
    timestamp = 0;
    transactions.clear();
    name = "";
    light = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::duplicate(const CBlock& bl) {
    clear();
    CBaseNode::duplicate(bl);

    gasLimit = bl.gasLimit;
    gasUsed = bl.gasUsed;
    hash = bl.hash;
    blockNumber = bl.blockNumber;
    parentHash = bl.parentHash;
    miner = bl.miner;
    difficulty = bl.difficulty;
    price = bl.price;
    finalized = bl.finalized;
    timestamp = bl.timestamp;
    transactions = bl.transactions;
    name = bl.name;
    light = bl.light;

    // EXISTING_CODE
    finishParse();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlock& CBlock::operator=(const CBlock& bl) {
    duplicate(bl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBlock::operator==(const CBlock& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBlock& v1, const CBlock& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBlock> CBlockArray;
extern CArchive& operator>>(CArchive& archive, CBlockArray& array);
extern CArchive& operator<<(CArchive& archive, const CBlockArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CBlock& blo);
extern CArchive& operator>>(CArchive& archive, CBlock& blo);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_BLOCK;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<blknum_t, CBlock> CBlockMap;
//---------------------------------------------------------------------------
extern blknum_t bnFromPath(const string_q& path, blknum_t& endOut, timestamp_t& ts);
inline blknum_t bnFromPath(const string_q& path) {
    blknum_t unused = NOPOS;
    timestamp_t unused_ts;
    return bnFromPath(path, unused, unused_ts);
}
// EXISTING_CODE
}  // namespace qblocks
