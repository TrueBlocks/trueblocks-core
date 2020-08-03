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
#include "pinneditem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPinReport : public CBaseNode {
  public:
    string_q fileName;
    string_q indexFormat;
    string_q bloomFormat;
    string_q commitHash;
    hash_t prevHash;
    string_q newBlockRange;
    CPinnedItemArray newPins;
    string_q prevBlockRange;
    CPinnedItemArray prevPins;

  public:
    CPinReport(void);
    CPinReport(const CPinReport& pi);
    virtual ~CPinReport(void);
    CPinReport& operator=(const CPinReport& pi);

    DECLARE_NODE(CPinReport);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinReport& item) const;
    bool operator!=(const CPinReport& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CPinReport& v1, const CPinReport& v2);
    friend ostream& operator<<(ostream& os, const CPinReport& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinReport& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinReport::CPinReport(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinReport::CPinReport(const CPinReport& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinReport::~CPinReport(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinReport::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinReport::initialize(void) {
    CBaseNode::initialize();

    fileName = "";
    indexFormat = "";
    bloomFormat = "";
    commitHash = "";
    prevHash = "";
    newBlockRange = "";
    newPins.clear();
    prevBlockRange = "";
    prevPins.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinReport::duplicate(const CPinReport& pi) {
    clear();
    CBaseNode::duplicate(pi);

    fileName = pi.fileName;
    indexFormat = pi.indexFormat;
    bloomFormat = pi.bloomFormat;
    commitHash = pi.commitHash;
    prevHash = pi.prevHash;
    newBlockRange = pi.newBlockRange;
    newPins = pi.newPins;
    prevBlockRange = pi.prevBlockRange;
    prevPins = pi.prevPins;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinReport& CPinReport::operator=(const CPinReport& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinReport::operator==(const CPinReport& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinReport& v1, const CPinReport& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinReport> CPinReportArray;
extern CArchive& operator>>(CArchive& archive, CPinReportArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinReportArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinReport& pin);
extern CArchive& operator>>(CArchive& archive, CPinReport& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINREPORT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
