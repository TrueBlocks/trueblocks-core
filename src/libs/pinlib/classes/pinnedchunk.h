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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPinnedChunk : public CBaseNode {
  public:
    string_q fileName;
    ipfshash_t bloomHash;
    ipfshash_t indexHash;

  public:
    CPinnedChunk(void);
    CPinnedChunk(const CPinnedChunk& pi);
    virtual ~CPinnedChunk(void);
    CPinnedChunk& operator=(const CPinnedChunk& pi);

    DECLARE_NODE(CPinnedChunk);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinnedChunk& it) const;
    bool operator!=(const CPinnedChunk& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPinnedChunk& v1, const CPinnedChunk& v2);
    friend ostream& operator<<(ostream& os, const CPinnedChunk& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinnedChunk& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinnedChunk::CPinnedChunk(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinnedChunk::CPinnedChunk(const CPinnedChunk& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinnedChunk::~CPinnedChunk(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinnedChunk::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinnedChunk::initialize(void) {
    CBaseNode::initialize();

    fileName = "";
    bloomHash = "";
    indexHash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinnedChunk::duplicate(const CPinnedChunk& pi) {
    clear();
    CBaseNode::duplicate(pi);

    fileName = pi.fileName;
    bloomHash = pi.bloomHash;
    indexHash = pi.indexHash;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinnedChunk& CPinnedChunk::operator=(const CPinnedChunk& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinnedChunk::operator==(const CPinnedChunk& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return fileName == it.fileName;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinnedChunk& v1, const CPinnedChunk& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.fileName < v2.fileName;
}

//---------------------------------------------------------------------------
typedef vector<CPinnedChunk> CPinnedChunkArray;
extern CArchive& operator>>(CArchive& archive, CPinnedChunkArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinnedChunkArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinnedChunk& pin);
extern CArchive& operator>>(CArchive& archive, CPinnedChunk& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINNEDCHUNK;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
