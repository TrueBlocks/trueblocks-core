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
class CCreateCloneToken : public CTransaction {
public:
    string_q _cloneTokenName;
    uint32_t _cloneDecimalUnits;
    string_q _cloneTokenSymbol;
    biguint_t _snapshotBlock;
    bool _transfersEnabled;

public:
    CCreateCloneToken(void);
    CCreateCloneToken(const CCreateCloneToken& cr);
    virtual ~CCreateCloneToken(void);
    CCreateCloneToken& operator=(const CCreateCloneToken& cr);

    DECLARE_NODE(CCreateCloneToken);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCreateCloneToken& item) const;
    bool operator!=(const CCreateCloneToken& item) const { return !operator==(item); }
    friend bool operator<(const CCreateCloneToken& v1, const CCreateCloneToken& v2);
    friend ostream& operator<<(ostream& os, const CCreateCloneToken& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCreateCloneToken& cr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCreateCloneToken::CCreateCloneToken(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCreateCloneToken::CCreateCloneToken(const CCreateCloneToken& cr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCreateCloneToken::~CCreateCloneToken(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateCloneToken::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateCloneToken::initialize(void) {
    CTransaction::initialize();

    _cloneTokenName = "";
    _cloneDecimalUnits = 0;
    _cloneTokenSymbol = "";
    _snapshotBlock = 0;
    _transfersEnabled = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateCloneToken::duplicate(const CCreateCloneToken& cr) {
    clear();
    CTransaction::duplicate(cr);

    _cloneTokenName = cr._cloneTokenName;
    _cloneDecimalUnits = cr._cloneDecimalUnits;
    _cloneTokenSymbol = cr._cloneTokenSymbol;
    _snapshotBlock = cr._snapshotBlock;
    _transfersEnabled = cr._transfersEnabled;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCreateCloneToken& CCreateCloneToken::operator=(const CCreateCloneToken& cr) {
    duplicate(cr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCreateCloneToken::operator==(const CCreateCloneToken& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCreateCloneToken& v1, const CCreateCloneToken& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCreateCloneToken> CCreateCloneTokenArray;
extern CArchive& operator>>(CArchive& archive, CCreateCloneTokenArray& array);
extern CArchive& operator<<(CArchive& archive, const CCreateCloneTokenArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

