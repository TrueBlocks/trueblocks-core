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
class CChangeUnicorn : public CTransaction {
public:
    biguint_t newUnicornPriceInFinney;
    address_t newUnicornAddress;

public:
    CChangeUnicorn(void);
    CChangeUnicorn(const CChangeUnicorn& ch);
    virtual ~CChangeUnicorn(void);
    CChangeUnicorn& operator=(const CChangeUnicorn& ch);

    DECLARE_NODE(CChangeUnicorn);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeUnicorn& item) const;
    bool operator!=(const CChangeUnicorn& item) const { return !operator==(item); }
    friend bool operator<(const CChangeUnicorn& v1, const CChangeUnicorn& v2);
    friend ostream& operator<<(ostream& os, const CChangeUnicorn& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeUnicorn& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeUnicorn::CChangeUnicorn(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeUnicorn::CChangeUnicorn(const CChangeUnicorn& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeUnicorn::~CChangeUnicorn(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeUnicorn::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeUnicorn::initialize(void) {
    CTransaction::initialize();

    newUnicornPriceInFinney = 0;
    newUnicornAddress = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeUnicorn::duplicate(const CChangeUnicorn& ch) {
    clear();
    CTransaction::duplicate(ch);

    newUnicornPriceInFinney = ch.newUnicornPriceInFinney;
    newUnicornAddress = ch.newUnicornAddress;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeUnicorn& CChangeUnicorn::operator=(const CChangeUnicorn& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeUnicorn::operator==(const CChangeUnicorn& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeUnicorn& v1, const CChangeUnicorn& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeUnicorn> CChangeUnicornArray;
extern CArchive& operator>>(CArchive& archive, CChangeUnicornArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeUnicornArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

