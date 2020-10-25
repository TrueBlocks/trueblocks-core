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
#include "etherlib.h"
#include "commandoption.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CApiRoute : public CBaseNode {
  public:
    string_q route;
    CCommandOptionArray commands;

  public:
    CApiRoute(void);
    CApiRoute(const CApiRoute& ap);
    virtual ~CApiRoute(void);
    CApiRoute& operator=(const CApiRoute& ap);

    DECLARE_NODE(CApiRoute);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CApiRoute& it) const;
    bool operator!=(const CApiRoute& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CApiRoute& v1, const CApiRoute& v2);
    friend ostream& operator<<(ostream& os, const CApiRoute& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CApiRoute& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CApiRoute::CApiRoute(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CApiRoute::CApiRoute(const CApiRoute& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CApiRoute::~CApiRoute(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiRoute::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiRoute::initialize(void) {
    CBaseNode::initialize();

    route = "";
    commands.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApiRoute::duplicate(const CApiRoute& ap) {
    clear();
    CBaseNode::duplicate(ap);

    route = ap.route;
    commands = ap.commands;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CApiRoute& CApiRoute::operator=(const CApiRoute& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CApiRoute::operator==(const CApiRoute& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CApiRoute& v1, const CApiRoute& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CApiRoute> CApiRouteArray;
extern CArchive& operator>>(CArchive& archive, CApiRouteArray& array);
extern CArchive& operator<<(CArchive& archive, const CApiRouteArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_APIROUTE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
