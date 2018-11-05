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
class CUpdateReserve : public CTransaction {
public:
    address_t _reserveToken;
    uint32_t _ratio;
    bool _enableVirtualBalance;
    biguint_t _virtualBalance;

public:
    CUpdateReserve(void);
    CUpdateReserve(const CUpdateReserve& up);
    virtual ~CUpdateReserve(void);
    CUpdateReserve& operator=(const CUpdateReserve& up);

    DECLARE_NODE(CUpdateReserve);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUpdateReserve& item) const;
    bool operator!=(const CUpdateReserve& item) const { return !operator==(item); }
    friend bool operator<(const CUpdateReserve& v1, const CUpdateReserve& v2);
    friend ostream& operator<<(ostream& os, const CUpdateReserve& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUpdateReserve& up);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUpdateReserve::CUpdateReserve(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUpdateReserve::CUpdateReserve(const CUpdateReserve& up) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(up);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUpdateReserve::~CUpdateReserve(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUpdateReserve::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUpdateReserve::initialize(void) {
    CTransaction::initialize();

    _reserveToken = "";
    _ratio = 0;
    _enableVirtualBalance = 0;
    _virtualBalance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUpdateReserve::duplicate(const CUpdateReserve& up) {
    clear();
    CTransaction::duplicate(up);

    _reserveToken = up._reserveToken;
    _ratio = up._ratio;
    _enableVirtualBalance = up._enableVirtualBalance;
    _virtualBalance = up._virtualBalance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUpdateReserve& CUpdateReserve::operator=(const CUpdateReserve& up) {
    duplicate(up);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUpdateReserve::operator==(const CUpdateReserve& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUpdateReserve& v1, const CUpdateReserve& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUpdateReserve> CUpdateReserveArray;
extern CArchive& operator>>(CArchive& archive, CUpdateReserveArray& array);
extern CArchive& operator<<(CArchive& archive, const CUpdateReserveArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

