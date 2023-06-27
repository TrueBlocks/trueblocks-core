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
// EXISTING_CODE

//--------------------------------------------------------------------------
class CEthCall : public CBaseNode {
  public:
    blknum_t blockNumber;
    address_t address;
    string_q signature;
    string_q encoding;
    string_q bytes;
    CAbi abi_spec;
    CFunction callResult;
    string_q compressedResult;
    blknum_t deployed;

  public:
    CEthCall(void);
    CEthCall(const CEthCall& et);
    virtual ~CEthCall(void);
    CEthCall& operator=(const CEthCall& et);

    DECLARE_NODE(CEthCall);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    string_q getCallResult(void) const;
    bool getCallResult(string_q& out) const;
    bool getCallResult(CStringArray& out) const;
    // EXISTING_CODE
    bool operator==(const CEthCall& it) const;
    bool operator!=(const CEthCall& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CEthCall& v1, const CEthCall& v2);
    friend ostream& operator<<(ostream& os, const CEthCall& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEthCall& et);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEthCall::CEthCall(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEthCall::CEthCall(const CEthCall& et) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(et);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEthCall::~CEthCall(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEthCall::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEthCall::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    address = "";
    signature = "";
    encoding = "";
    bytes = "";
    abi_spec = CAbi();
    callResult = CFunction();
    compressedResult = "";
    deployed = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEthCall::duplicate(const CEthCall& et) {
    clear();
    CBaseNode::duplicate(et);

    blockNumber = et.blockNumber;
    address = et.address;
    signature = et.signature;
    encoding = et.encoding;
    bytes = et.bytes;
    abi_spec = et.abi_spec;
    callResult = et.callResult;
    compressedResult = et.compressedResult;
    deployed = et.deployed;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEthCall& CEthCall::operator=(const CEthCall& et) {
    duplicate(et);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEthCall::operator==(const CEthCall& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return blockNumber == it.blockNumber;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEthCall& v1, const CEthCall& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.blockNumber < v2.blockNumber;
}

//---------------------------------------------------------------------------
typedef vector<CEthCall> CEthCallArray;
extern CArchive& operator>>(CArchive& archive, CEthCallArray& array);
extern CArchive& operator<<(CArchive& archive, const CEthCallArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CEthCall& eth);
extern CArchive& operator>>(CArchive& archive, CEthCall& eth);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ETHCALL;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool doEthCall(CEthCall& call, bool checkProxy);
extern bool getProxyContract(const address_t& contract, blknum_t blockNum, address_t& proxy);
// EXISTING_CODE
}  // namespace qblocks
