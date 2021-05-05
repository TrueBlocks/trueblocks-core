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
class CApiKey : public CBaseNode {
  public:
    string_q key;
    string_q secret;

  public:
    CApiKey(void);
    CApiKey(const CApiKey& ap);
    virtual ~CApiKey(void);
    CApiKey& operator=(const CApiKey& ap);

    DECLARE_NODE(CApiKey);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CApiKey& it) const;
    bool operator!=(const CApiKey& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CApiKey& v1, const CApiKey& v2);
    friend ostream& operator<<(ostream& os, const CApiKey& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CApiKey& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CApiKey::CApiKey(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CApiKey::CApiKey(const CApiKey& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CApiKey::~CApiKey(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiKey::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiKey::initialize(void) {
    CBaseNode::initialize();

    key = "";
    secret = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiKey::duplicate(const CApiKey& ap) {
    clear();
    CBaseNode::duplicate(ap);

    key = ap.key;
    secret = ap.secret;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CApiKey& CApiKey::operator=(const CApiKey& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CApiKey::operator==(const CApiKey& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (key == it.key);
}

//-------------------------------------------------------------------------
inline bool operator<(const CApiKey& v1, const CApiKey& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CApiKey> CApiKeyArray;
extern CArchive& operator>>(CArchive& archive, CApiKeyArray& array);
extern CArchive& operator<<(CArchive& archive, const CApiKeyArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CApiKey& api);
extern CArchive& operator>>(CArchive& archive, CApiKey& api);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_APIKEY;

//---------------------------------------------------------------------------
// EXISTING_CODE
//----------------------------------------------------------------
extern bool getApiKey(CApiKey& lic);
extern string_q getApiKey(const string_q& apiName, const string_q& signup);
// EXISTING_CODE
}  // namespace qblocks
