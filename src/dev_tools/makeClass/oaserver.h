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
class COAServer : public CBaseNode {
  public:
    string_q url;

  public:
    COAServer(void);
    COAServer(const COAServer& oa);
    virtual ~COAServer(void);
    COAServer& operator=(const COAServer& oa);

    DECLARE_NODE(COAServer);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COAServer& it) const;
    bool operator!=(const COAServer& it) const {
        return !operator==(it);
    }
    friend bool operator<(const COAServer& v1, const COAServer& v2);
    friend ostream& operator<<(ostream& os, const COAServer& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COAServer& oa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COAServer::COAServer(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COAServer::COAServer(const COAServer& oa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(oa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COAServer::~COAServer(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COAServer::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COAServer::initialize(void) {
    CBaseNode::initialize();

    url = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COAServer::duplicate(const COAServer& oa) {
    clear();
    CBaseNode::duplicate(oa);

    url = oa.url;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COAServer& COAServer::operator=(const COAServer& oa) {
    duplicate(oa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COAServer::operator==(const COAServer& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COAServer& v1, const COAServer& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COAServer> COAServerArray;
extern CArchive& operator>>(CArchive& archive, COAServerArray& array);
extern CArchive& operator<<(CArchive& archive, const COAServerArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const COAServer& oas);
extern CArchive& operator>>(CArchive& archive, COAServer& oas);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_OASERVER;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
