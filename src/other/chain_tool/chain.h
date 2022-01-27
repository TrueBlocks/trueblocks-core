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
class CChain : public CBaseNode {
  public:
    string_q chain_name;
    string_q chain_type;
    uint64_t chain_id;
    string_q rpc_provider;
    string_q symbol;
    string_q remote_explorer;
    string_q pin_gateway;

  public:
    CChain(void);
    CChain(const CChain& ch);
    virtual ~CChain(void);
    CChain& operator=(const CChain& ch);

    DECLARE_NODE(CChain);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChain& it) const;
    bool operator!=(const CChain& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CChain& v1, const CChain& v2);
    friend ostream& operator<<(ostream& os, const CChain& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChain& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChain::CChain(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChain::CChain(const CChain& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChain::~CChain(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChain::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChain::initialize(void) {
    CBaseNode::initialize();

    chain_name = "";
    chain_type = "";
    chain_id = 0;
    rpc_provider = "";
    symbol = "";
    remote_explorer = "";
    pin_gateway = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChain::duplicate(const CChain& ch) {
    clear();
    CBaseNode::duplicate(ch);

    chain_name = ch.chain_name;
    chain_type = ch.chain_type;
    chain_id = ch.chain_id;
    rpc_provider = ch.rpc_provider;
    symbol = ch.symbol;
    remote_explorer = ch.remote_explorer;
    pin_gateway = ch.pin_gateway;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChain& CChain::operator=(const CChain& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChain::operator==(const CChain& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChain& v1, const CChain& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChain> CChainArray;
extern CArchive& operator>>(CArchive& archive, CChainArray& array);
extern CArchive& operator<<(CArchive& archive, const CChainArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CChain& cha);
extern CArchive& operator>>(CArchive& archive, CChain& cha);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CHAIN;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
