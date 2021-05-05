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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLogQuery : public CBaseNode {
  public:
    blknum_t fromBlock;
    blknum_t toBlock;
    hash_t blockHash;
    CAddressArray addresses;
    CTopicArray topics;

  public:
    CLogQuery(void);
    CLogQuery(const CLogQuery& lo);
    virtual ~CLogQuery(void);
    CLogQuery& operator=(const CLogQuery& lo);

    DECLARE_NODE(CLogQuery);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    string_q toRPC(void) const;
    // EXISTING_CODE
    bool operator==(const CLogQuery& it) const;
    bool operator!=(const CLogQuery& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CLogQuery& v1, const CLogQuery& v2);
    friend ostream& operator<<(ostream& os, const CLogQuery& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLogQuery& lo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogQuery::CLogQuery(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogQuery::CLogQuery(const CLogQuery& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogQuery::~CLogQuery(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogQuery::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogQuery::initialize(void) {
    CBaseNode::initialize();

    fromBlock = 0;
    toBlock = 0;
    blockHash = "";
    addresses.clear();
    topics.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogQuery::duplicate(const CLogQuery& lo) {
    clear();
    CBaseNode::duplicate(lo);

    fromBlock = lo.fromBlock;
    toBlock = lo.toBlock;
    blockHash = lo.blockHash;
    addresses = lo.addresses;
    topics = lo.topics;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogQuery& CLogQuery::operator=(const CLogQuery& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLogQuery::operator==(const CLogQuery& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLogQuery& v1, const CLogQuery& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLogQuery> CLogQueryArray;
extern CArchive& operator>>(CArchive& archive, CLogQueryArray& array);
extern CArchive& operator<<(CArchive& archive, const CLogQueryArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_LOGQUERY;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
