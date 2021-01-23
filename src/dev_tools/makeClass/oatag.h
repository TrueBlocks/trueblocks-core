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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class COATag : public CBaseNode {
  public:
    string_q name;

  public:
    COATag(void);
    COATag(const COATag& oa);
    virtual ~COATag(void);
    COATag& operator=(const COATag& oa);

    DECLARE_NODE(COATag);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COATag& it) const;
    bool operator!=(const COATag& it) const {
        return !operator==(it);
    }
    friend bool operator<(const COATag& v1, const COATag& v2);
    friend ostream& operator<<(ostream& os, const COATag& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COATag& oa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COATag::COATag(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COATag::COATag(const COATag& oa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(oa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COATag::~COATag(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COATag::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COATag::initialize(void) {
    CBaseNode::initialize();

    name = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COATag::duplicate(const COATag& oa) {
    clear();
    CBaseNode::duplicate(oa);

    name = oa.name;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COATag& COATag::operator=(const COATag& oa) {
    duplicate(oa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COATag::operator==(const COATag& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COATag& v1, const COATag& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COATag> COATagArray;
extern CArchive& operator>>(CArchive& archive, COATagArray& array);
extern CArchive& operator<<(CArchive& archive, const COATagArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const COATag& oat);
extern CArchive& operator>>(CArchive& archive, COATag& oat);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_OATAG;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
