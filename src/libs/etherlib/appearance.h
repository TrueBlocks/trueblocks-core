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
/*

This is the width of an ascii version of an appearance stored in flat files:

  42 character 0x-prefixed address                  42 bytes
  \t                                                 1
  nine character 0-prefixed block number             9
  \t                                                 1
  five character 0-prefixed transaction id           5
  \n                                                 1
                                                -------
                                                    59
*/

#define asciiAppearanceSize uint32_t(59)
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAppearance : public CBaseNode {
  public:
    address_t address;
    blknum_t blockNumber;
    blknum_t transactionIndex;
    blknum_t traceIndex;
    string_q reason;
    string_q name;
    timestamp_t timestamp;
    string_q date;

  public:
    CAppearance(void);
    CAppearance(const CAppearance& ap);
    virtual ~CAppearance(void);
    CAppearance& operator=(const CAppearance& ap);

    DECLARE_NODE(CAppearance);

    // EXISTING_CODE
    CAppearance(blknum_t b, blknum_t x, blknum_t c, const address_t& a, const string_q r)
        : address(a), blockNumber(b), transactionIndex(x), traceIndex(c), reason(r) {
    }
    CAppearance(const address_t& a, const string_q& n, blknum_t b, blknum_t t)
        : address(a), blockNumber(b), transactionIndex(t), name(n) {
    }
    // string_q Format(const string_q& fmt) const;
    // EXISTING_CODE
    bool operator==(const CAppearance& it) const;
    bool operator!=(const CAppearance& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAppearance& v1, const CAppearance& v2);
    friend ostream& operator<<(ostream& os, const CAppearance& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAppearance& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAppearance::CAppearance(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearance::CAppearance(const CAppearance& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAppearance::~CAppearance(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearance::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearance::initialize(void) {
    CBaseNode::initialize();

    address = "";
    blockNumber = 0;
    transactionIndex = 0;
    traceIndex = 0;
    reason = "";
    name = "";
    timestamp = 0;
    date = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearance::duplicate(const CAppearance& ap) {
    clear();
    CBaseNode::duplicate(ap);

    address = ap.address;
    blockNumber = ap.blockNumber;
    transactionIndex = ap.transactionIndex;
    traceIndex = ap.traceIndex;
    reason = ap.reason;
    name = ap.name;
    timestamp = ap.timestamp;
    date = ap.date;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearance& CAppearance::operator=(const CAppearance& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAppearance::operator==(const CAppearance& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAppearance& v1, const CAppearance& v2) {
    // EXISTING_CODE
    return ((v1.blockNumber != v2.blockNumber) ? v1.blockNumber < v2.blockNumber
                                               : v1.transactionIndex < v2.transactionIndex);
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAppearance> CAppearanceArray;
extern CArchive& operator>>(CArchive& archive, CAppearanceArray& array);
extern CArchive& operator<<(CArchive& archive, const CAppearanceArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAppearance& app);
extern CArchive& operator>>(CArchive& archive, CAppearance& app);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_APPEARANCE;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef bool (*ADDRESSFUNC)(const address_t& addr, void* data);

//---------------------------------------------------------------------------
extern int findAddresses(const void* v1, const void* v2);
// EXISTING_CODE
}  // namespace qblocks
