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
#include "transaction.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTransfer : public CBaseNode {
  public:
    blknum_t blockNumber;
    blknum_t transactionIndex;
    blknum_t logIndex;
    hash_t transactionHash;
    timestamp_t timestamp;
    time_q date;
    address_t sender;
    address_t recipient;
    address_t assetAddr;
    string_q assetSymbol;
    uint64_t decimals;
    biguint_t amount;
    double spotPrice;
    string_q priceSource;
    string_q topic0;
    string_q topic1;
    string_q topic2;
    string_q topic3;
    string_q data;
    string_q encoding;

  public:
    CTransfer(void);
    CTransfer(const CTransfer& tr);
    virtual ~CTransfer(void);
    CTransfer& operator=(const CTransfer& tr);

    DECLARE_NODE(CTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransfer& it) const;
    bool operator!=(const CTransfer& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTransfer& v1, const CTransfer& v2);
    friend ostream& operator<<(ostream& os, const CTransfer& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransfer& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransfer::CTransfer(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransfer::CTransfer(const CTransfer& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransfer::~CTransfer(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransfer::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransfer::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    transactionIndex = 0;
    logIndex = 0;
    transactionHash = "";
    timestamp = 0;
    date = earliestDate;
    sender = "";
    recipient = "";
    assetAddr = "";
    assetSymbol = "";
    decimals = 18;
    amount = 0;
    spotPrice = 1.0;
    priceSource = "";
    topic0 = "";
    topic1 = "";
    topic2 = "";
    topic3 = "";
    data = "";
    encoding = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransfer::duplicate(const CTransfer& tr) {
    clear();
    CBaseNode::duplicate(tr);

    blockNumber = tr.blockNumber;
    transactionIndex = tr.transactionIndex;
    logIndex = tr.logIndex;
    transactionHash = tr.transactionHash;
    timestamp = tr.timestamp;
    date = tr.date;
    sender = tr.sender;
    recipient = tr.recipient;
    assetAddr = tr.assetAddr;
    assetSymbol = tr.assetSymbol;
    decimals = tr.decimals;
    amount = tr.amount;
    spotPrice = tr.spotPrice;
    priceSource = tr.priceSource;
    topic0 = tr.topic0;
    topic1 = tr.topic1;
    topic2 = tr.topic2;
    topic3 = tr.topic3;
    data = tr.data;
    encoding = tr.encoding;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransfer& CTransfer::operator=(const CTransfer& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransfer::operator==(const CTransfer& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransfer& v1, const CTransfer& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransfer> CTransferArray;
extern CArchive& operator>>(CArchive& archive, CTransferArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTransfer& tra);
extern CArchive& operator>>(CArchive& archive, CTransfer& tra);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRANSFER;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
