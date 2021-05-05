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
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPinataMetadata : public CBaseNode {
  public:
    string_q name;
    CKeyValueArray keyvalues;

  public:
    CPinataMetadata(void);
    CPinataMetadata(const CPinataMetadata& pi);
    virtual ~CPinataMetadata(void);
    CPinataMetadata& operator=(const CPinataMetadata& pi);

    DECLARE_NODE(CPinataMetadata);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinataMetadata& it) const;
    bool operator!=(const CPinataMetadata& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPinataMetadata& v1, const CPinataMetadata& v2);
    friend ostream& operator<<(ostream& os, const CPinataMetadata& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinataMetadata& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinataMetadata::CPinataMetadata(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataMetadata::CPinataMetadata(const CPinataMetadata& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinataMetadata::~CPinataMetadata(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataMetadata::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataMetadata::initialize(void) {
    CBaseNode::initialize();

    name = "";
    keyvalues.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataMetadata::duplicate(const CPinataMetadata& pi) {
    clear();
    CBaseNode::duplicate(pi);

    name = pi.name;
    keyvalues = pi.keyvalues;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataMetadata& CPinataMetadata::operator=(const CPinataMetadata& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinataMetadata::operator==(const CPinataMetadata& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (name == it.name);
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinataMetadata& v1, const CPinataMetadata& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinataMetadata> CPinataMetadataArray;
extern CArchive& operator>>(CArchive& archive, CPinataMetadataArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinataMetadataArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinataMetadata& pin);
extern CArchive& operator>>(CArchive& archive, CPinataMetadata& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINATAMETADATA;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
