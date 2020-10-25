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
class CApiSpec : public CBaseNode {
  public:
    string_q method;
    string_q uri;
    string_q headers;

  public:
    CApiSpec(void);
    CApiSpec(const CApiSpec& ap);
    virtual ~CApiSpec(void);
    CApiSpec& operator=(const CApiSpec& ap);

    DECLARE_NODE(CApiSpec);

    // EXISTING_CODE
    bool sendData(const string_q& data);
    string_q getData(const string_q& params);
    // EXISTING_CODE
    bool operator==(const CApiSpec& it) const;
    bool operator!=(const CApiSpec& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CApiSpec& v1, const CApiSpec& v2);
    friend ostream& operator<<(ostream& os, const CApiSpec& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CApiSpec& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CApiSpec::CApiSpec(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CApiSpec::CApiSpec(const CApiSpec& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CApiSpec::~CApiSpec(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiSpec::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiSpec::initialize(void) {
    CBaseNode::initialize();

    method = "";
    uri = "";
    headers = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiSpec::duplicate(const CApiSpec& ap) {
    clear();
    CBaseNode::duplicate(ap);

    method = ap.method;
    uri = ap.uri;
    headers = ap.headers;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CApiSpec& CApiSpec::operator=(const CApiSpec& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CApiSpec::operator==(const CApiSpec& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CApiSpec& v1, const CApiSpec& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CApiSpec> CApiSpecArray;
extern CArchive& operator>>(CArchive& archive, CApiSpecArray& array);
extern CArchive& operator<<(CArchive& archive, const CApiSpecArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CApiSpec& api);
extern CArchive& operator>>(CArchive& archive, CApiSpec& api);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_APISPEC;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
