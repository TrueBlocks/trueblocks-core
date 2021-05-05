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
#include "utillib.h"
#include "pinatapin.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPinataPinlist : public CBaseNode {
  public:
    string_q count;
    CPinataPinArray rows;

  public:
    CPinataPinlist(void);
    CPinataPinlist(const CPinataPinlist& pi);
    virtual ~CPinataPinlist(void);
    CPinataPinlist& operator=(const CPinataPinlist& pi);

    DECLARE_NODE(CPinataPinlist);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinataPinlist& it) const;
    bool operator!=(const CPinataPinlist& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPinataPinlist& v1, const CPinataPinlist& v2);
    friend ostream& operator<<(ostream& os, const CPinataPinlist& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinataPinlist& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinataPinlist::CPinataPinlist(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataPinlist::CPinataPinlist(const CPinataPinlist& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinataPinlist::~CPinataPinlist(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataPinlist::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataPinlist::initialize(void) {
    CBaseNode::initialize();

    count = "";
    rows.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataPinlist::duplicate(const CPinataPinlist& pi) {
    clear();
    CBaseNode::duplicate(pi);

    count = pi.count;
    rows = pi.rows;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataPinlist& CPinataPinlist::operator=(const CPinataPinlist& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinataPinlist::operator==(const CPinataPinlist& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinataPinlist& v1, const CPinataPinlist& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinataPinlist> CPinataPinlistArray;
extern CArchive& operator>>(CArchive& archive, CPinataPinlistArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinataPinlistArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinataPinlist& pin);
extern CArchive& operator>>(CArchive& archive, CPinataPinlist& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINATAPINLIST;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
