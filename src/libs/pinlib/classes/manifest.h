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
#include "etherlib.h"
#include "pinnedchunk.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CManifest : public CBaseNode {
  public:
    string_q version;
    string_q chain;
    ipfshash_t schemas;
    ipfshash_t databases;
    CPinnedChunkArray chunks;

  public:
    CManifest(void);
    CManifest(const CManifest& ma);
    virtual ~CManifest(void);
    CManifest& operator=(const CManifest& ma);

    DECLARE_NODE(CManifest);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CManifest& it) const;
    bool operator!=(const CManifest& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CManifest& v1, const CManifest& v2);
    friend ostream& operator<<(ostream& os, const CManifest& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CManifest& ma);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CManifest::CManifest(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CManifest::CManifest(const CManifest& ma) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ma);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CManifest::~CManifest(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CManifest::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CManifest::initialize(void) {
    CBaseNode::initialize();

    version = "";
    chain = "";
    schemas = "";
    databases = "";
    chunks.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CManifest::duplicate(const CManifest& ma) {
    clear();
    CBaseNode::duplicate(ma);

    version = ma.version;
    chain = ma.chain;
    schemas = ma.schemas;
    databases = ma.databases;
    chunks = ma.chunks;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CManifest& CManifest::operator=(const CManifest& ma) {
    duplicate(ma);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CManifest::operator==(const CManifest& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CManifest& v1, const CManifest& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CManifest> CManifestArray;
extern CArchive& operator>>(CArchive& archive, CManifestArray& array);
extern CArchive& operator<<(CArchive& archive, const CManifestArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CManifest& man);
extern CArchive& operator>>(CArchive& archive, CManifest& man);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MANIFEST;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
