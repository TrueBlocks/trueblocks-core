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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CCounts : public CBaseNode {
  public:
    address_t address;
    uint64_t count;

  public:
    CCounts(void);
    CCounts(const CCounts& co);
    virtual ~CCounts(void);
    CCounts& operator=(const CCounts& co);

    DECLARE_NODE(CCounts);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCounts& item) const;
    bool operator!=(const CCounts& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CCounts& v1, const CCounts& v2);
    friend ostream& operator<<(ostream& os, const CCounts& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCounts& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCounts::CCounts(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCounts::CCounts(const CCounts& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCounts::~CCounts(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCounts::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCounts::initialize(void) {
    CBaseNode::initialize();

    address = "";
    count = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCounts::duplicate(const CCounts& co) {
    clear();
    CBaseNode::duplicate(co);

    address = co.address;
    count = co.count;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCounts& CCounts::operator=(const CCounts& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCounts::operator==(const CCounts& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCounts& v1, const CCounts& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCounts> CCountsArray;
extern CArchive& operator>>(CArchive& archive, CCountsArray& array);
extern CArchive& operator<<(CArchive& archive, const CCountsArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_COUNTS;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
