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
class CPinnedItem : public CBaseNode {
  public:
    string_q fileName;
    timestamp_t uploadTs;
    string_q bloomHash;
    string_q indexHash;

  public:
    CPinnedItem(void);
    CPinnedItem(const CPinnedItem& pi);
    virtual ~CPinnedItem(void);
    CPinnedItem& operator=(const CPinnedItem& pi);

    DECLARE_NODE(CPinnedItem);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinnedItem& item) const;
    bool operator!=(const CPinnedItem& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CPinnedItem& v1, const CPinnedItem& v2);
    friend ostream& operator<<(ostream& os, const CPinnedItem& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinnedItem& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinnedItem::CPinnedItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinnedItem::CPinnedItem(const CPinnedItem& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinnedItem::~CPinnedItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinnedItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinnedItem::initialize(void) {
    CBaseNode::initialize();

    fileName = "";
    uploadTs = 0;
    bloomHash = "";
    indexHash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinnedItem::duplicate(const CPinnedItem& pi) {
    clear();
    CBaseNode::duplicate(pi);

    fileName = pi.fileName;
    uploadTs = pi.uploadTs;
    bloomHash = pi.bloomHash;
    indexHash = pi.indexHash;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinnedItem& CPinnedItem::operator=(const CPinnedItem& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinnedItem::operator==(const CPinnedItem& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return fileName == item.fileName;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinnedItem& v1, const CPinnedItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.fileName < v2.fileName;
}

//---------------------------------------------------------------------------
typedef vector<CPinnedItem> CPinnedItemArray;
extern CArchive& operator>>(CArchive& archive, CPinnedItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinnedItemArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinnedItem& pin);
extern CArchive& operator>>(CArchive& archive, CPinnedItem& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINNEDITEM;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
