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
#include "abilib.h"
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
    explicit CReceipt(const CTransaction* pT) : pTrans(pT) {
    }
    const CTransaction* pTrans;
    friend class CTransaction;
    // EXISTING_CODE
    bool operator==(const CReceipt& item) const;
    bool operator!=(const CReceipt& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CReceipt& v1, const CReceipt& v2);
    friend ostream& operator<<(ostream& os, const CReceipt& item);

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
    logs.clear();
    root = "";
    status = NO_STATUS;

    // EXISTING_CODE
    pTrans = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceipt::duplicate(const CReceipt& re) {
    clear();
    CBaseNode::duplicate(re);

    contractAddress = re.contractAddress;
    cumulativeGasUsed = re.cumulativeGasUsed;
    gasUsed = re.gasUsed;
    logs = re.logs;
    root = re.root;
    status = re.status;

    // EXISTING_CODE
    pTrans = re.pTrans;  // no deep copy becuase it's const
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
inline bool CReceipt::operator==(const CReceipt& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
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
extern string_q nextTransactionChunk(const string_q& fieldIn, const void* data);
// EXISTING_CODE
}  // namespace qblocks
