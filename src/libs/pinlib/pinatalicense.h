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
#include "pinlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPinataLicense : public CBaseNode {
  public:
    string_q apiKey;
    string_q secretKey;

  public:
    CPinataLicense(void);
    CPinataLicense(const CPinataLicense& pi);
    virtual ~CPinataLicense(void);
    CPinataLicense& operator=(const CPinataLicense& pi);

    DECLARE_NODE(CPinataLicense);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPinataLicense& it) const;
    bool operator!=(const CPinataLicense& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CPinataLicense& v1, const CPinataLicense& v2);
    friend ostream& operator<<(ostream& os, const CPinataLicense& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPinataLicense& pi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPinataLicense::CPinataLicense(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataLicense::CPinataLicense(const CPinataLicense& pi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPinataLicense::~CPinataLicense(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataLicense::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataLicense::initialize(void) {
    CBaseNode::initialize();

    apiKey = "";
    secretKey = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPinataLicense::duplicate(const CPinataLicense& pi) {
    clear();
    CBaseNode::duplicate(pi);

    apiKey = pi.apiKey;
    secretKey = pi.secretKey;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPinataLicense& CPinataLicense::operator=(const CPinataLicense& pi) {
    duplicate(pi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPinataLicense::operator==(const CPinataLicense& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (apiKey == it.apiKey);
}

//-------------------------------------------------------------------------
inline bool operator<(const CPinataLicense& v1, const CPinataLicense& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPinataLicense> CPinataLicenseArray;
extern CArchive& operator>>(CArchive& archive, CPinataLicenseArray& array);
extern CArchive& operator<<(CArchive& archive, const CPinataLicenseArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CPinataLicense& pin);
extern CArchive& operator>>(CArchive& archive, CPinataLicense& pin);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PINATALICENSE;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool pinlib_pinataListOfPins(const CPinataLicense& lic, string_q& result);
extern bool pinlib_getPinataKeys(CPinataLicense& lic);
inline bool pinlib_hasPinataKeys(void) {
    CPinataLicense lic;
    return pinlib_getPinataKeys(lic);
}
// EXISTING_CODE
}  // namespace qblocks
