#if 0
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
#include "utillib.h"
#include "silkworm/db/lmdbval.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLmdbRecord : public CBaseNode {
  public:
    CLmdbVal key;
    CLmdbVal value;

  public:
    CLmdbRecord(void);
    CLmdbRecord(const CLmdbRecord& lm);
    virtual ~CLmdbRecord(void);
    CLmdbRecord& operator=(const CLmdbRecord& lm);

    DECLARE_NODE(CLmdbRecord);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    CLmdbRecord(const MDB_val& key, const MDB_val& value);
    // EXISTING_CODE
    bool operator==(const CLmdbRecord& it) const;
    bool operator!=(const CLmdbRecord& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CLmdbRecord& v1, const CLmdbRecord& v2);
    friend ostream& operator<<(ostream& os, const CLmdbRecord& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLmdbRecord& lm);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLmdbRecord::CLmdbRecord(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLmdbRecord::CLmdbRecord(const CLmdbRecord& lm) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lm);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLmdbRecord::~CLmdbRecord(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLmdbRecord::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLmdbRecord::initialize(void) {
    CBaseNode::initialize();

    key = CLmdbVal();
    value = CLmdbVal();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLmdbRecord::duplicate(const CLmdbRecord& lm) {
    clear();
    CBaseNode::duplicate(lm);

    key = lm.key;
    value = lm.value;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLmdbRecord& CLmdbRecord::operator=(const CLmdbRecord& lm) {
    duplicate(lm);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLmdbRecord::operator==(const CLmdbRecord& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLmdbRecord& v1, const CLmdbRecord& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLmdbRecord> CLmdbRecordArray;
extern CArchive& operator>>(CArchive& archive, CLmdbRecordArray& array);
extern CArchive& operator<<(CArchive& archive, const CLmdbRecordArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CLmdbRecord& lmd);
extern CArchive& operator>>(CArchive& archive, CLmdbRecord& lmd);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_LMDBRECORD;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
#endif
