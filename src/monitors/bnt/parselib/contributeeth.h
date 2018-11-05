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
class CContributeETH : public CTransaction {
public:
    CContributeETH(void);
    CContributeETH(const CContributeETH& co);
    virtual ~CContributeETH(void);
    CContributeETH& operator=(const CContributeETH& co);

    DECLARE_NODE(CContributeETH);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CContributeETH& item) const;
    bool operator!=(const CContributeETH& item) const { return !operator==(item); }
    friend bool operator<(const CContributeETH& v1, const CContributeETH& v2);
    friend ostream& operator<<(ostream& os, const CContributeETH& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CContributeETH& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CContributeETH::CContributeETH(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CContributeETH::CContributeETH(const CContributeETH& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CContributeETH::~CContributeETH(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributeETH::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributeETH::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributeETH::duplicate(const CContributeETH& co) {
    clear();
    CTransaction::duplicate(co);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CContributeETH& CContributeETH::operator=(const CContributeETH& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CContributeETH::operator==(const CContributeETH& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CContributeETH& v1, const CContributeETH& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CContributeETH> CContributeETHArray;
extern CArchive& operator>>(CArchive& archive, CContributeETHArray& array);
extern CArchive& operator<<(CArchive& archive, const CContributeETHArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

