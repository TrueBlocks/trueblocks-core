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
class CKey : public CBaseNode {
  public:
    string_q provider;
    string_q apiKey;
    string_q jwt;
    string_q secret;

  public:
    CKey(void);
    CKey(const CKey& ke);
    virtual ~CKey(void);
    CKey& operator=(const CKey& ke);

    DECLARE_NODE(CKey);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CKey& it) const;
    bool operator!=(const CKey& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CKey& v1, const CKey& v2);
    friend ostream& operator<<(ostream& os, const CKey& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CKey& ke);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CKey::CKey(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CKey::CKey(const CKey& ke) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ke);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CKey::~CKey(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CKey::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CKey::initialize(void) {
    CBaseNode::initialize();

    provider = "";
    apiKey = "";
    jwt = "";
    secret = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CKey::duplicate(const CKey& ke) {
    clear();
    CBaseNode::duplicate(ke);

    provider = ke.provider;
    apiKey = ke.apiKey;
    jwt = ke.jwt;
    secret = ke.secret;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CKey& CKey::operator=(const CKey& ke) {
    duplicate(ke);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CKey::operator==(const CKey& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CKey& v1, const CKey& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CKey> CKeyArray;
extern CArchive& operator>>(CArchive& archive, CKeyArray& array);
extern CArchive& operator<<(CArchive& archive, const CKeyArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CKey& key);
extern CArchive& operator>>(CArchive& archive, CKey& key);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_KEY;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
