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
class CStartAuctions : public CTransaction {
public:
    CStringArray _hashes;

public:
    CStartAuctions(void);
    CStartAuctions(const CStartAuctions& st);
    virtual ~CStartAuctions(void);
    CStartAuctions& operator=(const CStartAuctions& st);

    DECLARE_NODE(CStartAuctions);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStartAuctions& item) const;
    bool operator!=(const CStartAuctions& item) const { return !operator==(item); }
    friend bool operator<(const CStartAuctions& v1, const CStartAuctions& v2);
    friend ostream& operator<<(ostream& os, const CStartAuctions& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStartAuctions& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStartAuctions::CStartAuctions(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStartAuctions::CStartAuctions(const CStartAuctions& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStartAuctions::~CStartAuctions(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuctions::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuctions::initialize(void) {
    CTransaction::initialize();

    _hashes.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuctions::duplicate(const CStartAuctions& st) {
    clear();
    CTransaction::duplicate(st);

    _hashes = st._hashes;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStartAuctions& CStartAuctions::operator=(const CStartAuctions& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStartAuctions::operator==(const CStartAuctions& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStartAuctions& v1, const CStartAuctions& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStartAuctions> CStartAuctionsArray;
extern CArchive& operator>>(CArchive& archive, CStartAuctionsArray& array);
extern CArchive& operator<<(CArchive& archive, const CStartAuctionsArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

