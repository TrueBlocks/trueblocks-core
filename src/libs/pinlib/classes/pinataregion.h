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
class CPinataRegion : public CBaseNode {
  public:
    string_q regionId;
    string_q currentReplicationCount;
    string_q desiredReplicationCount;

  public:
    CPinataRegion(void);
    CPinataRegion(const CPinataRegion& pi);
    virtual ~CPinataRegion(void);
    CPinataRegion& operator=(const CPinataRegion& pi);

    DECLARE_NODE(CPinataRegion);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinataRegion& it) const;
    bool operator!=(const CPinataRegion& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPinataRegion& v1, const CPinataRegion& v2);
    friend ostream& operator<<(ostream& os, const CPinataRegion& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinataRegion& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinataRegion::CPinataRegion(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataRegion::CPinataRegion(const CPinataRegion& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinataRegion::~CPinataRegion(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataRegion::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataRegion::initialize(void) {
    CBaseNode::initialize();

    regionId = "";
    currentReplicationCount = "";
    desiredReplicationCount = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataRegion::duplicate(const CPinataRegion& pi) {
    clear();
    CBaseNode::duplicate(pi);

    regionId = pi.regionId;
    currentReplicationCount = pi.currentReplicationCount;
    desiredReplicationCount = pi.desiredReplicationCount;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataRegion& CPinataRegion::operator=(const CPinataRegion& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinataRegion::operator==(const CPinataRegion& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinataRegion& v1, const CPinataRegion& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinataRegion> CPinataRegionArray;
extern CArchive& operator>>(CArchive& archive, CPinataRegionArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinataRegionArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinataRegion& pin);
extern CArchive& operator>>(CArchive& archive, CPinataRegion& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINATAREGION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
