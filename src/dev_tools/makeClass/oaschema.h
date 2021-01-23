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
class COASchema : public CBaseNode {
  public:
    string_q noop;

  public:
    COASchema(void);
    COASchema(const COASchema& oa);
    virtual ~COASchema(void);
    COASchema& operator=(const COASchema& oa);

    DECLARE_NODE(COASchema);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COASchema& it) const;
    bool operator!=(const COASchema& it) const {
        return !operator==(it);
    }
    friend bool operator<(const COASchema& v1, const COASchema& v2);
    friend ostream& operator<<(ostream& os, const COASchema& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COASchema& oa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COASchema::COASchema(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COASchema::COASchema(const COASchema& oa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(oa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COASchema::~COASchema(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COASchema::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COASchema::initialize(void) {
    CBaseNode::initialize();

    noop = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COASchema::duplicate(const COASchema& oa) {
    clear();
    CBaseNode::duplicate(oa);

    noop = oa.noop;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COASchema& COASchema::operator=(const COASchema& oa) {
    duplicate(oa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COASchema::operator==(const COASchema& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COASchema& v1, const COASchema& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COASchema> COASchemaArray;
extern CArchive& operator>>(CArchive& archive, COASchemaArray& array);
extern CArchive& operator<<(CArchive& archive, const COASchemaArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const COASchema& oas);
extern CArchive& operator>>(CArchive& archive, COASchema& oas);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_OASCHEMA;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
