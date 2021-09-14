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
class CRoute : public CBaseNode {
  public:
    string_q path;
    string_q helpText;
    string_q hotKey;

  public:
    CRoute(void);
    CRoute(const CRoute& ro);
    virtual ~CRoute(void);
    CRoute& operator=(const CRoute& ro);

    DECLARE_NODE(CRoute);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRoute& it) const;
    bool operator!=(const CRoute& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CRoute& v1, const CRoute& v2);
    friend ostream& operator<<(ostream& os, const CRoute& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRoute& ro);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRoute::CRoute(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRoute::CRoute(const CRoute& ro) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ro);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRoute::~CRoute(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRoute::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRoute::initialize(void) {
    CBaseNode::initialize();

    path = "";
    helpText = "";
    hotKey = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRoute::duplicate(const CRoute& ro) {
    clear();
    CBaseNode::duplicate(ro);

    path = ro.path;
    helpText = ro.helpText;
    hotKey = ro.hotKey;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRoute& CRoute::operator=(const CRoute& ro) {
    duplicate(ro);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRoute::operator==(const CRoute& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRoute& v1, const CRoute& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRoute> CRouteArray;
extern CArchive& operator>>(CArchive& archive, CRouteArray& array);
extern CArchive& operator<<(CArchive& archive, const CRouteArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CRoute& rou);
extern CArchive& operator>>(CArchive& archive, CRoute& rou);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ROUTE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
