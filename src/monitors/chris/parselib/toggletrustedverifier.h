#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include <vector>
#include <map>
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CToggleTrustedVerifier : public CTransaction {
public:
    address_t _verifier;

public:
    CToggleTrustedVerifier(void);
    CToggleTrustedVerifier(const CToggleTrustedVerifier& to);
    virtual ~CToggleTrustedVerifier(void);
    CToggleTrustedVerifier& operator=(const CToggleTrustedVerifier& to);

    DECLARE_NODE(CToggleTrustedVerifier);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CToggleTrustedVerifier& item) const;
    bool operator!=(const CToggleTrustedVerifier& item) const { return !operator==(item); }
    friend bool operator<(const CToggleTrustedVerifier& v1, const CToggleTrustedVerifier& v2);
    friend ostream& operator<<(ostream& os, const CToggleTrustedVerifier& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CToggleTrustedVerifier& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CToggleTrustedVerifier::CToggleTrustedVerifier(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CToggleTrustedVerifier::CToggleTrustedVerifier(const CToggleTrustedVerifier& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CToggleTrustedVerifier::~CToggleTrustedVerifier(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CToggleTrustedVerifier::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CToggleTrustedVerifier::initialize(void) {
    CTransaction::initialize();

    _verifier = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CToggleTrustedVerifier::duplicate(const CToggleTrustedVerifier& to) {
    clear();
    CTransaction::duplicate(to);

    _verifier = to._verifier;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CToggleTrustedVerifier& CToggleTrustedVerifier::operator=(const CToggleTrustedVerifier& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CToggleTrustedVerifier::operator==(const CToggleTrustedVerifier& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CToggleTrustedVerifier& v1, const CToggleTrustedVerifier& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CToggleTrustedVerifier> CToggleTrustedVerifierArray;
extern CArchive& operator>>(CArchive& archive, CToggleTrustedVerifierArray& array);
extern CArchive& operator<<(CArchive& archive, const CToggleTrustedVerifierArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

