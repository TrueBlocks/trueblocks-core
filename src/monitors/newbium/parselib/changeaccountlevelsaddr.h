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
class CChangeAccountLevelsAddr : public CTransaction {
public:
    address_t accountLevelsAddr_;

public:
    CChangeAccountLevelsAddr(void);
    CChangeAccountLevelsAddr(const CChangeAccountLevelsAddr& ch);
    virtual ~CChangeAccountLevelsAddr(void);
    CChangeAccountLevelsAddr& operator=(const CChangeAccountLevelsAddr& ch);

    DECLARE_NODE(CChangeAccountLevelsAddr);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeAccountLevelsAddr& item) const;
    bool operator!=(const CChangeAccountLevelsAddr& item) const { return !operator==(item); }
    friend bool operator<(const CChangeAccountLevelsAddr& v1, const CChangeAccountLevelsAddr& v2);
    friend ostream& operator<<(ostream& os, const CChangeAccountLevelsAddr& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeAccountLevelsAddr& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeAccountLevelsAddr::CChangeAccountLevelsAddr(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeAccountLevelsAddr::CChangeAccountLevelsAddr(const CChangeAccountLevelsAddr& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeAccountLevelsAddr::~CChangeAccountLevelsAddr(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAccountLevelsAddr::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAccountLevelsAddr::initialize(void) {
    CTransaction::initialize();

    accountLevelsAddr_ = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAccountLevelsAddr::duplicate(const CChangeAccountLevelsAddr& ch) {
    clear();
    CTransaction::duplicate(ch);

    accountLevelsAddr_ = ch.accountLevelsAddr_;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeAccountLevelsAddr& CChangeAccountLevelsAddr::operator=(const CChangeAccountLevelsAddr& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeAccountLevelsAddr::operator==(const CChangeAccountLevelsAddr& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeAccountLevelsAddr& v1, const CChangeAccountLevelsAddr& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeAccountLevelsAddr> CChangeAccountLevelsAddrArray;
extern CArchive& operator>>(CArchive& archive, CChangeAccountLevelsAddrArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeAccountLevelsAddrArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

