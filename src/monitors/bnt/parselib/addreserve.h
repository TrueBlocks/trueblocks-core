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
class CAddReserve : public CTransaction {
public:
    address_t _token;
    uint32_t _ratio;
    bool _enableVirtualBalance;

public:
    CAddReserve(void);
    CAddReserve(const CAddReserve& ad);
    virtual ~CAddReserve(void);
    CAddReserve& operator=(const CAddReserve& ad);

    DECLARE_NODE(CAddReserve);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAddReserve& item) const;
    bool operator!=(const CAddReserve& item) const { return !operator==(item); }
    friend bool operator<(const CAddReserve& v1, const CAddReserve& v2);
    friend ostream& operator<<(ostream& os, const CAddReserve& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAddReserve& ad);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAddReserve::CAddReserve(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAddReserve::CAddReserve(const CAddReserve& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAddReserve::~CAddReserve(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddReserve::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddReserve::initialize(void) {
    CTransaction::initialize();

    _token = "";
    _ratio = 0;
    _enableVirtualBalance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddReserve::duplicate(const CAddReserve& ad) {
    clear();
    CTransaction::duplicate(ad);

    _token = ad._token;
    _ratio = ad._ratio;
    _enableVirtualBalance = ad._enableVirtualBalance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAddReserve& CAddReserve::operator=(const CAddReserve& ad) {
    duplicate(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAddReserve::operator==(const CAddReserve& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAddReserve& v1, const CAddReserve& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAddReserve> CAddReserveArray;
extern CArchive& operator>>(CArchive& archive, CAddReserveArray& array);
extern CArchive& operator<<(CArchive& archive, const CAddReserveArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

