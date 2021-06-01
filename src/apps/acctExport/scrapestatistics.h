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
class CScrapeStatistics : public CBaseNode {
  public:
    uint64_t nFiles;
    uint64_t nSkipped;
    uint64_t nChecked;
    uint64_t nBloomMisses;
    uint64_t nBloomHits;
    uint64_t nFalsePositive;
    uint64_t nPositive;
    uint64_t nStageChecked;
    uint64_t nStageHits;
    uint64_t nTotalHits;
    uint64_t nFileRecords;

  public:
    CScrapeStatistics(void);
    CScrapeStatistics(const CScrapeStatistics& sc);
    virtual ~CScrapeStatistics(void);
    CScrapeStatistics& operator=(const CScrapeStatistics& sc);

    DECLARE_NODE(CScrapeStatistics);

    // EXISTING_CODE
    string_q Header(const string_q& fmt) const;
    // EXISTING_CODE
    bool operator==(const CScrapeStatistics& it) const;
    bool operator!=(const CScrapeStatistics& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CScrapeStatistics& v1, const CScrapeStatistics& v2);
    friend ostream& operator<<(ostream& os, const CScrapeStatistics& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CScrapeStatistics& sc);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CScrapeStatistics::CScrapeStatistics(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CScrapeStatistics::CScrapeStatistics(const CScrapeStatistics& sc) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(sc);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CScrapeStatistics::~CScrapeStatistics(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CScrapeStatistics::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CScrapeStatistics::initialize(void) {
    CBaseNode::initialize();

    nFiles = 0;
    nSkipped = 0;
    nChecked = 0;
    nBloomMisses = 0;
    nBloomHits = 0;
    nFalsePositive = 0;
    nPositive = 0;
    nStageChecked = 0;
    nStageHits = 0;
    nTotalHits = 0;
    nFileRecords = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CScrapeStatistics::duplicate(const CScrapeStatistics& sc) {
    clear();
    CBaseNode::duplicate(sc);

    nFiles = sc.nFiles;
    nSkipped = sc.nSkipped;
    nChecked = sc.nChecked;
    nBloomMisses = sc.nBloomMisses;
    nBloomHits = sc.nBloomHits;
    nFalsePositive = sc.nFalsePositive;
    nPositive = sc.nPositive;
    nStageChecked = sc.nStageChecked;
    nStageHits = sc.nStageHits;
    nTotalHits = sc.nTotalHits;
    nFileRecords = sc.nFileRecords;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CScrapeStatistics& CScrapeStatistics::operator=(const CScrapeStatistics& sc) {
    duplicate(sc);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CScrapeStatistics::operator==(const CScrapeStatistics& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CScrapeStatistics& v1, const CScrapeStatistics& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CScrapeStatistics> CScrapeStatisticsArray;
extern CArchive& operator>>(CArchive& archive, CScrapeStatisticsArray& array);
extern CArchive& operator<<(CArchive& archive, const CScrapeStatisticsArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_SCRAPESTATISTICS;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
