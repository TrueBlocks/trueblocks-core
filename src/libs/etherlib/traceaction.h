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
class CTraceAction : public CBaseNode {
  public:
    address_t selfDestructed;
    wei_t balance;
    string_q callType;
    address_t from;
    gas_t gas;
    string_q init;
    string_q input;
    address_t refundAddress;
    string_q rewardType;
    address_t to;
    wei_t value;
    address_t author;

  public:
    CTraceAction(void);
    CTraceAction(const CTraceAction& tr);
    virtual ~CTraceAction(void);
    CTraceAction& operator=(const CTraceAction& tr);

    DECLARE_NODE(CTraceAction);

    // EXISTING_CODE
    friend class CTrace;
    wei_t extraValue1;
    wei_t extraValue2;
    // EXISTING_CODE
    bool operator==(const CTraceAction& it) const;
    bool operator!=(const CTraceAction& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTraceAction& v1, const CTraceAction& v2);
    friend ostream& operator<<(ostream& os, const CTraceAction& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTraceAction& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTraceAction::CTraceAction(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceAction::CTraceAction(const CTraceAction& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTraceAction::~CTraceAction(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceAction::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceAction::initialize(void) {
    CBaseNode::initialize();

    selfDestructed = "";
    balance = 0;
    callType = "";
    from = "";
    gas = 0;
    init = "";
    input = "";
    refundAddress = "";
    rewardType = "";
    to = "";
    value = 0;
    author = "";

    // EXISTING_CODE
    extraValue1 = 0;
    extraValue2 = 0;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceAction::duplicate(const CTraceAction& tr) {
    clear();
    CBaseNode::duplicate(tr);

    selfDestructed = tr.selfDestructed;
    balance = tr.balance;
    callType = tr.callType;
    from = tr.from;
    gas = tr.gas;
    init = tr.init;
    input = tr.input;
    refundAddress = tr.refundAddress;
    rewardType = tr.rewardType;
    to = tr.to;
    value = tr.value;
    author = tr.author;

    // EXISTING_CODE
    extraValue1 = tr.extraValue1;
    extraValue2 = tr.extraValue2;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceAction& CTraceAction::operator=(const CTraceAction& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTraceAction::operator==(const CTraceAction& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return ((selfDestructed == it.selfDestructed) && (balance == it.balance) && (callType == it.callType) &&
            (from == it.from) && (gas == it.gas) && (init == it.init) && (input == it.input) &&
            (refundAddress == it.refundAddress) && (to == it.to) && (value == it.value));
}

//-------------------------------------------------------------------------
inline bool operator<(const CTraceAction& v1, const CTraceAction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTraceAction> CTraceActionArray;
extern CArchive& operator>>(CArchive& archive, CTraceActionArray& array);
extern CArchive& operator<<(CArchive& archive, const CTraceActionArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTraceAction& tra);
extern CArchive& operator>>(CArchive& archive, CTraceAction& tra);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRACEACTION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
