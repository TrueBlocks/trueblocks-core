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
class CPinningRecord : public CBaseNode {
  public:
    timestamp_t pinDate;
    string_q fileName;
    string_q index_hash;
    uint64_t index_size;
    string_q bloom_hash;
    uint64_t bloom_size;

  public:
    CPinningRecord(void);
    CPinningRecord(const CPinningRecord& pi);
    virtual ~CPinningRecord(void);
    CPinningRecord& operator=(const CPinningRecord& pi);

    DECLARE_NODE(CPinningRecord);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinningRecord& item) const;
    bool operator!=(const CPinningRecord& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CPinningRecord& v1, const CPinningRecord& v2);
    friend ostream& operator<<(ostream& os, const CPinningRecord& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinningRecord& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinningRecord::CPinningRecord(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinningRecord::CPinningRecord(const CPinningRecord& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinningRecord::~CPinningRecord(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinningRecord::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinningRecord::initialize(void) {
    CBaseNode::initialize();

    pinDate = 0;
    fileName = "";
    index_hash = "";
    index_size = 0;
    bloom_hash = "";
    bloom_size = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinningRecord::duplicate(const CPinningRecord& pi) {
    clear();
    CBaseNode::duplicate(pi);

    pinDate = pi.pinDate;
    fileName = pi.fileName;
    index_hash = pi.index_hash;
    index_size = pi.index_size;
    bloom_hash = pi.bloom_hash;
    bloom_size = pi.bloom_size;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinningRecord& CPinningRecord::operator=(const CPinningRecord& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinningRecord::operator==(const CPinningRecord& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinningRecord& v1, const CPinningRecord& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinningRecord> CPinningRecordArray;
extern CArchive& operator>>(CArchive& archive, CPinningRecordArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinningRecordArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinningRecord& pin);
extern CArchive& operator>>(CArchive& archive, CPinningRecord& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINNINGRECORD;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
