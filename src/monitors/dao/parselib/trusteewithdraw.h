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
class CTrusteeWithdraw : public CTransaction {
public:
    CTrusteeWithdraw(void);
    CTrusteeWithdraw(const CTrusteeWithdraw& tr);
    virtual ~CTrusteeWithdraw(void);
    CTrusteeWithdraw& operator=(const CTrusteeWithdraw& tr);

    DECLARE_NODE(CTrusteeWithdraw);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTrusteeWithdraw& item) const;
    bool operator!=(const CTrusteeWithdraw& item) const { return !operator==(item); }
    friend bool operator<(const CTrusteeWithdraw& v1, const CTrusteeWithdraw& v2);
    friend ostream& operator<<(ostream& os, const CTrusteeWithdraw& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTrusteeWithdraw& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTrusteeWithdraw::CTrusteeWithdraw(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTrusteeWithdraw::CTrusteeWithdraw(const CTrusteeWithdraw& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTrusteeWithdraw::~CTrusteeWithdraw(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrusteeWithdraw::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrusteeWithdraw::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrusteeWithdraw::duplicate(const CTrusteeWithdraw& tr) {
    clear();
    CTransaction::duplicate(tr);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTrusteeWithdraw& CTrusteeWithdraw::operator=(const CTrusteeWithdraw& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTrusteeWithdraw::operator==(const CTrusteeWithdraw& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTrusteeWithdraw& v1, const CTrusteeWithdraw& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTrusteeWithdraw> CTrusteeWithdrawArray;
extern CArchive& operator>>(CArchive& archive, CTrusteeWithdrawArray& array);
extern CArchive& operator<<(CArchive& archive, const CTrusteeWithdrawArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

