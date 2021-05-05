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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CSkin : public CBaseNode {
  public:
    string_q name;
    string_q bgPrimary;
    string_q bgSecondary;
    string_q textPrimary;
    string_q borderPrimary;
    string_q bgHover;
    string_q textHover;
    string_q tableBgPrimary;
    string_q tableBgSecondary;
    string_q tableTextPrimary;
    string_q tableBorderPrimary;

  public:
    CSkin(void);
    CSkin(const CSkin& sk);
    virtual ~CSkin(void);
    CSkin& operator=(const CSkin& sk);

    DECLARE_NODE(CSkin);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSkin& it) const;
    bool operator!=(const CSkin& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CSkin& v1, const CSkin& v2);
    friend ostream& operator<<(ostream& os, const CSkin& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSkin& sk);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSkin::CSkin(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSkin::CSkin(const CSkin& sk) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(sk);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSkin::~CSkin(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSkin::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSkin::initialize(void) {
    CBaseNode::initialize();

    name = "";
    bgPrimary = "";
    bgSecondary = "";
    textPrimary = "";
    borderPrimary = "";
    bgHover = "";
    textHover = "";
    tableBgPrimary = "";
    tableBgSecondary = "";
    tableTextPrimary = "";
    tableBorderPrimary = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSkin::duplicate(const CSkin& sk) {
    clear();
    CBaseNode::duplicate(sk);

    name = sk.name;
    bgPrimary = sk.bgPrimary;
    bgSecondary = sk.bgSecondary;
    textPrimary = sk.textPrimary;
    borderPrimary = sk.borderPrimary;
    bgHover = sk.bgHover;
    textHover = sk.textHover;
    tableBgPrimary = sk.tableBgPrimary;
    tableBgSecondary = sk.tableBgSecondary;
    tableTextPrimary = sk.tableTextPrimary;
    tableBorderPrimary = sk.tableBorderPrimary;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSkin& CSkin::operator=(const CSkin& sk) {
    duplicate(sk);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSkin::operator==(const CSkin& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSkin& v1, const CSkin& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSkin> CSkinArray;
extern CArchive& operator>>(CArchive& archive, CSkinArray& array);
extern CArchive& operator<<(CArchive& archive, const CSkinArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_SKIN;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
