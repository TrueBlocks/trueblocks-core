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
class CCacheBase : public CAccountName {
  public:
    blknum_t nApps;
    blknum_t firstApp;
    blknum_t latestApp;
    string_q path;
    uint64_t sizeInBytes;

  public:
    CCacheBase(void);
    CCacheBase(const CCacheBase& ca);
    virtual ~CCacheBase(void);
    CCacheBase& operator=(const CCacheBase& ca);

    DECLARE_NODE(CCacheBase);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCacheBase& it) const;
    bool operator!=(const CCacheBase& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CCacheBase& v1, const CCacheBase& v2);
    friend ostream& operator<<(ostream& os, const CCacheBase& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCacheBase& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCacheBase::CCacheBase(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCacheBase::CCacheBase(const CCacheBase& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCacheBase::~CCacheBase(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCacheBase::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCacheBase::initialize(void) {
    CAccountName::initialize();

    nApps = 0;
    firstApp = 0;
    latestApp = 0;
    path = "";
    sizeInBytes = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCacheBase::duplicate(const CCacheBase& ca) {
    clear();
    CAccountName::duplicate(ca);

    nApps = ca.nApps;
    firstApp = ca.firstApp;
    latestApp = ca.latestApp;
    path = ca.path;
    sizeInBytes = ca.sizeInBytes;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCacheBase& CCacheBase::operator=(const CCacheBase& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCacheBase::operator==(const CCacheBase& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCacheBase& v1, const CCacheBase& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCacheBase> CCacheBaseArray;
extern CArchive& operator>>(CArchive& archive, CCacheBaseArray& array);
extern CArchive& operator<<(CArchive& archive, const CCacheBaseArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CCacheBase& cac);
extern CArchive& operator>>(CArchive& archive, CCacheBase& cac);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CACHEBASE;

//---------------------------------------------------------------------------
extern string_q nextAccountnameChunk(const string_q& fieldIn, const void* data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
