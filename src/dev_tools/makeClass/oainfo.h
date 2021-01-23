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
class COAInfo : public CBaseNode {
  public:
    string_q description;
    string_q title;
    string_q version;

  public:
    COAInfo(void);
    COAInfo(const COAInfo& oa);
    virtual ~COAInfo(void);
    COAInfo& operator=(const COAInfo& oa);

    DECLARE_NODE(COAInfo);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COAInfo& it) const;
    bool operator!=(const COAInfo& it) const {
        return !operator==(it);
    }
    friend bool operator<(const COAInfo& v1, const COAInfo& v2);
    friend ostream& operator<<(ostream& os, const COAInfo& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COAInfo& oa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COAInfo::COAInfo(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COAInfo::COAInfo(const COAInfo& oa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(oa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COAInfo::~COAInfo(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COAInfo::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COAInfo::initialize(void) {
    CBaseNode::initialize();

    description = "";
    title = "";
    version = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COAInfo::duplicate(const COAInfo& oa) {
    clear();
    CBaseNode::duplicate(oa);

    description = oa.description;
    title = oa.title;
    version = oa.version;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COAInfo& COAInfo::operator=(const COAInfo& oa) {
    duplicate(oa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COAInfo::operator==(const COAInfo& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COAInfo& v1, const COAInfo& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COAInfo> COAInfoArray;
extern CArchive& operator>>(CArchive& archive, COAInfoArray& array);
extern CArchive& operator<<(CArchive& archive, const COAInfoArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const COAInfo& oai);
extern CArchive& operator>>(CArchive& archive, COAInfo& oai);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_OAINFO;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
