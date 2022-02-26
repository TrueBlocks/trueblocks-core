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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMeasure : public CBaseNode {
  public:
    string_q git_hash;
    string_q date;
    string_q machine;
    string_q node;
    string_q chain;
    string_q epoch;
    string_q group;
    string_q cmd;
    string_q type;
    uint64_t nTests;
    uint64_t nPassed;
    double totSecs;

  public:
    CMeasure(void);
    CMeasure(const CMeasure& me);
    virtual ~CMeasure(void);
    CMeasure& operator=(const CMeasure& me);

    DECLARE_NODE(CMeasure);

    // EXISTING_CODE
    bool allPassed;
    CMeasure(const string_q& g, const string_q& c, const string_q& t);
    CMeasure& operator+=(const CMeasure& m) {
        nTests += m.nTests;
        nPassed += m.nPassed;
        totSecs += m.totSecs;
        return *this;
    }
    double avgSecs(void) const {
        return totSecs / (double)nTests;
    }
    // EXISTING_CODE
    bool operator==(const CMeasure& it) const;
    bool operator!=(const CMeasure& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CMeasure& v1, const CMeasure& v2);
    friend ostream& operator<<(ostream& os, const CMeasure& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMeasure& me);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMeasure::CMeasure(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMeasure::CMeasure(const CMeasure& me) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(me);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMeasure::~CMeasure(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMeasure::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMeasure::initialize(void) {
    CBaseNode::initialize();

    git_hash = "";
    date = "";
    machine = "";
    node = "";
    chain = "";
    epoch = "";
    group = "";
    cmd = "";
    type = "";
    nTests = 0;
    nPassed = 0;
    totSecs = 0.0;

    // EXISTING_CODE
    chain = getGlobalConfig("testRunner")->getConfigStr("settings", "defaultChain", "mainnet");
    allPassed = false;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMeasure::duplicate(const CMeasure& me) {
    clear();
    CBaseNode::duplicate(me);

    git_hash = me.git_hash;
    date = me.date;
    machine = me.machine;
    node = me.node;
    chain = me.chain;
    epoch = me.epoch;
    group = me.group;
    cmd = me.cmd;
    type = me.type;
    nTests = me.nTests;
    nPassed = me.nPassed;
    totSecs = me.totSecs;

    // EXISTING_CODE
    allPassed = me.allPassed;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMeasure& CMeasure::operator=(const CMeasure& me) {
    duplicate(me);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMeasure::operator==(const CMeasure& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMeasure& v1, const CMeasure& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMeasure> CMeasureArray;
extern CArchive& operator>>(CArchive& archive, CMeasureArray& array);
extern CArchive& operator<<(CArchive& archive, const CMeasureArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CMeasure& mea);
extern CArchive& operator>>(CArchive& archive, CMeasure& mea);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MEASURE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
