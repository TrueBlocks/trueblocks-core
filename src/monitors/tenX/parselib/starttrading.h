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
class CStartTrading : public CTransaction {
public:
    CStartTrading(void);
    CStartTrading(const CStartTrading& st);
    virtual ~CStartTrading(void);
    CStartTrading& operator=(const CStartTrading& st);

    DECLARE_NODE(CStartTrading);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStartTrading& item) const;
    bool operator!=(const CStartTrading& item) const { return !operator==(item); }
    friend bool operator<(const CStartTrading& v1, const CStartTrading& v2);
    friend ostream& operator<<(ostream& os, const CStartTrading& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStartTrading& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStartTrading::CStartTrading(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStartTrading::CStartTrading(const CStartTrading& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStartTrading::~CStartTrading(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartTrading::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartTrading::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartTrading::duplicate(const CStartTrading& st) {
    clear();
    CTransaction::duplicate(st);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStartTrading& CStartTrading::operator=(const CStartTrading& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStartTrading::operator==(const CStartTrading& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStartTrading& v1, const CStartTrading& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStartTrading> CStartTradingArray;
extern CArchive& operator>>(CArchive& archive, CStartTradingArray& array);
extern CArchive& operator<<(CArchive& archive, const CStartTradingArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

