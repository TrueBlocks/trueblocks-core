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
class CCreateCloneTokena1c9 : public CTransaction {
public:
    address_t _parentToken;
    biguint_t _snapshotBlock;
    string_q _tokenName;
    uint32_t _decimalUnits;
    string_q _tokenSymbol;
    bool _transfersEnabled;

public:
    CCreateCloneTokena1c9(void);
    CCreateCloneTokena1c9(const CCreateCloneTokena1c9& cr);
    virtual ~CCreateCloneTokena1c9(void);
    CCreateCloneTokena1c9& operator=(const CCreateCloneTokena1c9& cr);

    DECLARE_NODE(CCreateCloneTokena1c9);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCreateCloneTokena1c9& item) const;
    bool operator!=(const CCreateCloneTokena1c9& item) const { return !operator==(item); }
    friend bool operator<(const CCreateCloneTokena1c9& v1, const CCreateCloneTokena1c9& v2);
    friend ostream& operator<<(ostream& os, const CCreateCloneTokena1c9& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCreateCloneTokena1c9& cr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCreateCloneTokena1c9::CCreateCloneTokena1c9(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCreateCloneTokena1c9::CCreateCloneTokena1c9(const CCreateCloneTokena1c9& cr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCreateCloneTokena1c9::~CCreateCloneTokena1c9(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateCloneTokena1c9::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateCloneTokena1c9::initialize(void) {
    CTransaction::initialize();

    _parentToken = "";
    _snapshotBlock = 0;
    _tokenName = "";
    _decimalUnits = 0;
    _tokenSymbol = "";
    _transfersEnabled = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateCloneTokena1c9::duplicate(const CCreateCloneTokena1c9& cr) {
    clear();
    CTransaction::duplicate(cr);

    _parentToken = cr._parentToken;
    _snapshotBlock = cr._snapshotBlock;
    _tokenName = cr._tokenName;
    _decimalUnits = cr._decimalUnits;
    _tokenSymbol = cr._tokenSymbol;
    _transfersEnabled = cr._transfersEnabled;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCreateCloneTokena1c9& CCreateCloneTokena1c9::operator=(const CCreateCloneTokena1c9& cr) {
    duplicate(cr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCreateCloneTokena1c9::operator==(const CCreateCloneTokena1c9& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCreateCloneTokena1c9& v1, const CCreateCloneTokena1c9& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCreateCloneTokena1c9> CCreateCloneTokena1c9Array;
extern CArchive& operator>>(CArchive& archive, CCreateCloneTokena1c9Array& array);
extern CArchive& operator<<(CArchive& archive, const CCreateCloneTokena1c9Array& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

