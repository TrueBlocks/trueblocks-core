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
#include "pinatapin.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPinataList : public CBaseNode {
  public:
    string_q count;
    CPinataPinArray rows;

  public:
    CPinataList(void);
    CPinataList(const CPinataList& pi);
    virtual ~CPinataList(void);
    CPinataList& operator=(const CPinataList& pi);

    DECLARE_NODE(CPinataList);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinataList& item) const;
    bool operator!=(const CPinataList& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CPinataList& v1, const CPinataList& v2);
    friend ostream& operator<<(ostream& os, const CPinataList& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinataList& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinataList::CPinataList(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataList::CPinataList(const CPinataList& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinataList::~CPinataList(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataList::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataList::initialize(void) {
    CBaseNode::initialize();

    count = "";
    rows.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataList::duplicate(const CPinataList& pi) {
    clear();
    CBaseNode::duplicate(pi);

    count = pi.count;
    rows = pi.rows;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataList& CPinataList::operator=(const CPinataList& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinataList::operator==(const CPinataList& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinataList& v1, const CPinataList& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinataList> CPinataListArray;
extern CArchive& operator>>(CArchive& archive, CPinataListArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinataListArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINATALIST;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
