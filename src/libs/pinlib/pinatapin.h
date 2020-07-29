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
#include "pinatametadata.h"
#include "pinataregion.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPinataPin : public CBaseNode {
  public:
    string_q id;
    string_q ipfs_pin_hash;
    uint64_t size;
    string_q user_id;
    string_q date_pinned;
    string_q date_unpinned;
    CPinataMetadata metadata;
    CPinataRegionArray regions;

  public:
    CPinataPin(void);
    CPinataPin(const CPinataPin& pi);
    virtual ~CPinataPin(void);
    CPinataPin& operator=(const CPinataPin& pi);

    DECLARE_NODE(CPinataPin);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinataPin& item) const;
    bool operator!=(const CPinataPin& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CPinataPin& v1, const CPinataPin& v2);
    friend ostream& operator<<(ostream& os, const CPinataPin& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinataPin& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinataPin::CPinataPin(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataPin::CPinataPin(const CPinataPin& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinataPin::~CPinataPin(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataPin::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataPin::initialize(void) {
    CBaseNode::initialize();

    id = "";
    ipfs_pin_hash = "";
    size = 0;
    user_id = "";
    date_pinned = "";
    date_unpinned = "";
    metadata = CPinataMetadata();
    regions.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataPin::duplicate(const CPinataPin& pi) {
    clear();
    CBaseNode::duplicate(pi);

    id = pi.id;
    ipfs_pin_hash = pi.ipfs_pin_hash;
    size = pi.size;
    user_id = pi.user_id;
    date_pinned = pi.date_pinned;
    date_unpinned = pi.date_unpinned;
    metadata = pi.metadata;
    regions = pi.regions;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataPin& CPinataPin::operator=(const CPinataPin& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinataPin::operator==(const CPinataPin& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinataPin& v1, const CPinataPin& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinataPin> CPinataPinArray;
extern CArchive& operator>>(CArchive& archive, CPinataPinArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinataPinArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinataPin& pin);
extern CArchive& operator>>(CArchive& archive, CPinataPin& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINATAPIN;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
