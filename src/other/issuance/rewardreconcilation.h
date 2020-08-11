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
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CRewardReconcilation : public CBaseNode {
  public:
    blknum_t blockNumber;
    timestamp_t timestamp;
    wei_t baseReward;
    wei_t baseAddReward;
    wei_t uncleReward;

  public:
    CRewardReconcilation(void);
    CRewardReconcilation(const CRewardReconcilation& re);
    virtual ~CRewardReconcilation(void);
    CRewardReconcilation& operator=(const CRewardReconcilation& re);

    DECLARE_NODE(CRewardReconcilation);

    // EXISTING_CODE
    explicit CRewardReconcilation(blknum_t bn, const address_t& blockMiner);
    CRewardReconcilation& operator+=(const CRewardReconcilation& rec);
    // EXISTING_CODE
    bool operator==(const CRewardReconcilation& item) const;
    bool operator!=(const CRewardReconcilation& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CRewardReconcilation& v1, const CRewardReconcilation& v2);
    friend ostream& operator<<(ostream& os, const CRewardReconcilation& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRewardReconcilation& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRewardReconcilation::CRewardReconcilation(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRewardReconcilation::CRewardReconcilation(const CRewardReconcilation& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRewardReconcilation::~CRewardReconcilation(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRewardReconcilation::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRewardReconcilation::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    timestamp = 0;
    baseReward = 0;
    baseAddReward = 0;
    uncleReward = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRewardReconcilation::duplicate(const CRewardReconcilation& re) {
    clear();
    CBaseNode::duplicate(re);

    blockNumber = re.blockNumber;
    timestamp = re.timestamp;
    baseReward = re.baseReward;
    baseAddReward = re.baseAddReward;
    uncleReward = re.uncleReward;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRewardReconcilation& CRewardReconcilation::operator=(const CRewardReconcilation& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRewardReconcilation::operator==(const CRewardReconcilation& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRewardReconcilation& v1, const CRewardReconcilation& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRewardReconcilation> CRewardReconcilationArray;
extern CArchive& operator>>(CArchive& archive, CRewardReconcilationArray& array);
extern CArchive& operator<<(CArchive& archive, const CRewardReconcilationArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_REWARDRECONCILATION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
