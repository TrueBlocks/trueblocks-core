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
class CSetDelegate : public CTransaction {
public:
    address_t _new;

public:
    CSetDelegate(void);
    CSetDelegate(const CSetDelegate& se);
    virtual ~CSetDelegate(void);
    CSetDelegate& operator=(const CSetDelegate& se);

    DECLARE_NODE(CSetDelegate);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetDelegate& item) const;
    bool operator!=(const CSetDelegate& item) const { return !operator==(item); }
    friend bool operator<(const CSetDelegate& v1, const CSetDelegate& v2);
    friend ostream& operator<<(ostream& os, const CSetDelegate& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetDelegate& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetDelegate::CSetDelegate(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetDelegate::CSetDelegate(const CSetDelegate& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetDelegate::~CSetDelegate(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetDelegate::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetDelegate::initialize(void) {
    CTransaction::initialize();

    _new = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetDelegate::duplicate(const CSetDelegate& se) {
    clear();
    CTransaction::duplicate(se);

    _new = se._new;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetDelegate& CSetDelegate::operator=(const CSetDelegate& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetDelegate::operator==(const CSetDelegate& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetDelegate& v1, const CSetDelegate& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetDelegate> CSetDelegateArray;
extern CArchive& operator>>(CArchive& archive, CSetDelegateArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetDelegateArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

