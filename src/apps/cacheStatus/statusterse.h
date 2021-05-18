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
#include "status.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CStatusTerse : public CStatus {
  public:
    string_q modes1;
    string_q modes2;

  public:
    CStatusTerse(void);
    CStatusTerse(const CStatusTerse& st);
    virtual ~CStatusTerse(void);
    CStatusTerse& operator=(const CStatusTerse& st);

    DECLARE_NODE(CStatusTerse);

    // EXISTING_CODE
    CStatusTerse(const CStatus& it);
    // EXISTING_CODE
    bool operator==(const CStatusTerse& it) const;
    bool operator!=(const CStatusTerse& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CStatusTerse& v1, const CStatusTerse& v2);
    friend ostream& operator<<(ostream& os, const CStatusTerse& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStatusTerse& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStatusTerse::CStatusTerse(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStatusTerse::CStatusTerse(const CStatusTerse& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStatusTerse::~CStatusTerse(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStatusTerse::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStatusTerse::initialize(void) {
    CStatus::initialize();

    modes1 = "";
    modes2 = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStatusTerse::duplicate(const CStatusTerse& st) {
    clear();
    CStatus::duplicate(st);

    modes1 = st.modes1;
    modes2 = st.modes2;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStatusTerse& CStatusTerse::operator=(const CStatusTerse& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStatusTerse::operator==(const CStatusTerse& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStatusTerse& v1, const CStatusTerse& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStatusTerse> CStatusTerseArray;
extern CArchive& operator>>(CArchive& archive, CStatusTerseArray& array);
extern CArchive& operator<<(CArchive& archive, const CStatusTerseArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_STATUSTERSE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
