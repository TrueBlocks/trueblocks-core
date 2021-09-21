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
#include "logentry.h"

namespace qblocks {

// EXISTING_CODE
class CTransaction;
#define NO_STATUS ((uint32_t)-1)
// EXISTING_CODE

//--------------------------------------------------------------------------
class CReceipt : public CBaseNode {
  public:
    address_t contractAddress;
    wei_t cumulativeGasUsed;
    gas_t gasUsed;
    gas_t effectiveGasPrice;
    CLogEntryArray logs;
    string_q root;
    uint32_t status;

  public:
    CReceipt(void);
    CReceipt(const CReceipt& re);
    virtual ~CReceipt(void);
    CReceipt& operator=(const CReceipt& re);

    DECLARE_NODE(CReceipt);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    explicit CReceipt(const CTransaction* pT) : pTransaction(pT) {
    }
    const CTransaction* pTransaction;
    friend class CTransaction;
    // EXISTING_CODE
    bool operator==(const CReceipt& it) const;
    bool operator!=(const CReceipt& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CReceipt& v1, const CReceipt& v2);
    friend ostream& operator<<(ostream& os, const CReceipt& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReceipt& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReceipt::CReceipt(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReceipt::CReceipt(const CReceipt& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReceipt::~CReceipt(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceipt::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceipt::initialize(void) {
    CBaseNode::initialize();

    contractAddress = "";
    cumulativeGasUsed = 0;
    gasUsed = 0;
    effectiveGasPrice = 0;
    logs.clear();
    root = "";
    status = NO_STATUS;

    // EXISTING_CODE
    pTransaction = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceipt::duplicate(const CReceipt& re) {
    clear();
    CBaseNode::duplicate(re);

    contractAddress = re.contractAddress;
    cumulativeGasUsed = re.cumulativeGasUsed;
    gasUsed = re.gasUsed;
    effectiveGasPrice = re.effectiveGasPrice;
    logs = re.logs;
    root = re.root;
    status = re.status;

    // EXISTING_CODE
    pTransaction = re.pTransaction;  // no deep copy becuase it's const
    finishParse();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReceipt& CReceipt::operator=(const CReceipt& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReceipt::operator==(const CReceipt& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (getValueByName("transactionHash") == it.getValueByName("transactionHash"));
}

//-------------------------------------------------------------------------
inline bool operator<(const CReceipt& v1, const CReceipt& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReceipt> CReceiptArray;
extern CArchive& operator>>(CArchive& archive, CReceiptArray& array);
extern CArchive& operator<<(CArchive& archive, const CReceiptArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CReceipt& rec);
extern CArchive& operator>>(CArchive& archive, CReceipt& rec);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_RECEIPT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
