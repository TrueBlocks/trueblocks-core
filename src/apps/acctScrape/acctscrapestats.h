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
class CAcctScrapeStats : public CBaseNode {
  public:
    uint64_t nFiles;
    uint64_t nSkipped;
    uint64_t nChecked;
    uint64_t nBloomMisses;
    uint64_t nBloomHits;
    uint64_t nFalsePositive;
    uint64_t nPositive;
    uint64_t nRecords;

  public:
    CAcctScrapeStats(void);
    CAcctScrapeStats(const CAcctScrapeStats& ac);
    virtual ~CAcctScrapeStats(void);
    CAcctScrapeStats& operator=(const CAcctScrapeStats& ac);

    DECLARE_NODE(CAcctScrapeStats);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAcctScrapeStats& it) const;
    bool operator!=(const CAcctScrapeStats& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAcctScrapeStats& v1, const CAcctScrapeStats& v2);
    friend ostream& operator<<(ostream& os, const CAcctScrapeStats& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAcctScrapeStats& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcctScrapeStats::CAcctScrapeStats(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcctScrapeStats::CAcctScrapeStats(const CAcctScrapeStats& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcctScrapeStats::~CAcctScrapeStats(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctScrapeStats::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctScrapeStats::initialize(void) {
    CBaseNode::initialize();

    nFiles = 0;
    nSkipped = 0;
    nChecked = 0;
    nBloomMisses = 0;
    nBloomHits = 0;
    nFalsePositive = 0;
    nPositive = 0;
    nRecords = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctScrapeStats::duplicate(const CAcctScrapeStats& ac) {
    clear();
    CBaseNode::duplicate(ac);

    nFiles = ac.nFiles;
    nSkipped = ac.nSkipped;
    nChecked = ac.nChecked;
    nBloomMisses = ac.nBloomMisses;
    nBloomHits = ac.nBloomHits;
    nFalsePositive = ac.nFalsePositive;
    nPositive = ac.nPositive;
    nRecords = ac.nRecords;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcctScrapeStats& CAcctScrapeStats::operator=(const CAcctScrapeStats& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAcctScrapeStats::operator==(const CAcctScrapeStats& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAcctScrapeStats& v1, const CAcctScrapeStats& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAcctScrapeStats> CAcctScrapeStatsArray;
extern CArchive& operator>>(CArchive& archive, CAcctScrapeStatsArray& array);
extern CArchive& operator<<(CArchive& archive, const CAcctScrapeStatsArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ACCTSCRAPESTATS;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
