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
typedef map<blknum_t, uint64_t> CHeatmapMap;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CHeatmap : public CBaseNode {
  public:
    CHeatmapMap heatmap;

  public:
    CHeatmap(void);
    CHeatmap(const CHeatmap& he);
    virtual ~CHeatmap(void);
    CHeatmap& operator=(const CHeatmap& he);

    DECLARE_NODE(CHeatmap);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CHeatmap& item) const;
    bool operator!=(const CHeatmap& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CHeatmap& v1, const CHeatmap& v2);
    friend ostream& operator<<(ostream& os, const CHeatmap& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CHeatmap& he);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CHeatmap::CHeatmap(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CHeatmap::CHeatmap(const CHeatmap& he) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(he);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CHeatmap::~CHeatmap(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHeatmap::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHeatmap::initialize(void) {
    CBaseNode::initialize();

    heatmap.clear();

    // EXISTING_CODE
    heatmap.clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHeatmap::duplicate(const CHeatmap& he) {
    clear();
    CBaseNode::duplicate(he);

    heatmap = he.heatmap;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CHeatmap& CHeatmap::operator=(const CHeatmap& he) {
    duplicate(he);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CHeatmap::operator==(const CHeatmap& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CHeatmap& v1, const CHeatmap& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CHeatmap> CHeatmapArray;
extern CArchive& operator>>(CArchive& archive, CHeatmapArray& array);
extern CArchive& operator<<(CArchive& archive, const CHeatmapArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_HEATMAP;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
