#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include <vector>
#include <map>
#include "etherlib.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CDailySummary : public CBaseNode {
public:
    timestamp_t timestamp;
    blknum_t nBlocks;
    blknum_t startBlock;
    blknum_t endBlock;
    blknum_t nTraces;
    blknum_t nTxErrors;
    blknum_t nTcErrors;
    blknum_t nSuccess;
    blknum_t nReward;
    blknum_t nTxCreates;
    blknum_t nTcCreates;
    blknum_t nTcFails;
    blknum_t nCalls;
    blknum_t nSelfDestruct;

public:
    CDailySummary(void);
    CDailySummary(const CDailySummary& da);
    virtual ~CDailySummary(void);
    CDailySummary& operator=(const CDailySummary& da);

    DECLARE_NODE(CDailySummary);

    // EXISTING_CODE
    explicit CDailySummary(const string_q& lineIn);
    // EXISTING_CODE
    bool operator==(const CDailySummary& item) const;
    bool operator!=(const CDailySummary& item) const { return !operator==(item); }
    friend bool operator<(const CDailySummary& v1, const CDailySummary& v2);
    friend ostream& operator<<(ostream& os, const CDailySummary& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDailySummary& da);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDailySummary::CDailySummary(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDailySummary::CDailySummary(const CDailySummary& da) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(da);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDailySummary::~CDailySummary(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDailySummary::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDailySummary::initialize(void) {
    CBaseNode::initialize();

    timestamp = 0;
    nBlocks = 0;
    startBlock = 0;
    endBlock = 0;
    nTraces = 0;
    nTxErrors = 0;
    nTcErrors = 0;
    nSuccess = 0;
    nReward = 0;
    nTxCreates = 0;
    nTcCreates = 0;
    nTcFails = 0;
    nCalls = 0;
    nSelfDestruct = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDailySummary::duplicate(const CDailySummary& da) {
    clear();
    CBaseNode::duplicate(da);

    timestamp = da.timestamp;
    nBlocks = da.nBlocks;
    startBlock = da.startBlock;
    endBlock = da.endBlock;
    nTraces = da.nTraces;
    nTxErrors = da.nTxErrors;
    nTcErrors = da.nTcErrors;
    nSuccess = da.nSuccess;
    nReward = da.nReward;
    nTxCreates = da.nTxCreates;
    nTcCreates = da.nTcCreates;
    nTcFails = da.nTcFails;
    nCalls = da.nCalls;
    nSelfDestruct = da.nSelfDestruct;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDailySummary& CDailySummary::operator=(const CDailySummary& da) {
    duplicate(da);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDailySummary::operator==(const CDailySummary& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDailySummary& v1, const CDailySummary& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDailySummary> CDailySummaryArray;
extern SFArchive& operator>>(SFArchive& archive, CDailySummaryArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CDailySummaryArray& array);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CDailySummary& dai);
extern SFArchive& operator>>(SFArchive& archive, CDailySummary& dai);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

