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
class CPinApiLicense : public CBaseNode {
  public:
    string_q apiKey;
    string_q secretKey;

  public:
    CPinApiLicense(void);
    CPinApiLicense(const CPinApiLicense& pi);
    virtual ~CPinApiLicense(void);
    CPinApiLicense& operator=(const CPinApiLicense& pi);

    DECLARE_NODE(CPinApiLicense);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinApiLicense& it) const;
    bool operator!=(const CPinApiLicense& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPinApiLicense& v1, const CPinApiLicense& v2);
    friend ostream& operator<<(ostream& os, const CPinApiLicense& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinApiLicense& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinApiLicense::CPinApiLicense(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinApiLicense::CPinApiLicense(const CPinApiLicense& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinApiLicense::~CPinApiLicense(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinApiLicense::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinApiLicense::initialize(void) {
    CBaseNode::initialize();

    apiKey = "";
    secretKey = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinApiLicense::duplicate(const CPinApiLicense& pi) {
    clear();
    CBaseNode::duplicate(pi);

    apiKey = pi.apiKey;
    secretKey = pi.secretKey;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinApiLicense& CPinApiLicense::operator=(const CPinApiLicense& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinApiLicense::operator==(const CPinApiLicense& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (apiKey == it.apiKey);
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinApiLicense& v1, const CPinApiLicense& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinApiLicense> CPinApiLicenseArray;
extern CArchive& operator>>(CArchive& archive, CPinApiLicenseArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinApiLicenseArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinApiLicense& pin);
extern CArchive& operator>>(CArchive& archive, CPinApiLicense& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINAPILICENSE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
