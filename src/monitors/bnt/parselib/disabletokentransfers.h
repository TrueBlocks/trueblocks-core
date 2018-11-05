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
class CDisableTokenTransfers : public CTransaction {
public:
    bool _disable;

public:
    CDisableTokenTransfers(void);
    CDisableTokenTransfers(const CDisableTokenTransfers& di);
    virtual ~CDisableTokenTransfers(void);
    CDisableTokenTransfers& operator=(const CDisableTokenTransfers& di);

    DECLARE_NODE(CDisableTokenTransfers);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDisableTokenTransfers& item) const;
    bool operator!=(const CDisableTokenTransfers& item) const { return !operator==(item); }
    friend bool operator<(const CDisableTokenTransfers& v1, const CDisableTokenTransfers& v2);
    friend ostream& operator<<(ostream& os, const CDisableTokenTransfers& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDisableTokenTransfers& di);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDisableTokenTransfers::CDisableTokenTransfers(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDisableTokenTransfers::CDisableTokenTransfers(const CDisableTokenTransfers& di) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(di);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDisableTokenTransfers::~CDisableTokenTransfers(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTokenTransfers::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTokenTransfers::initialize(void) {
    CTransaction::initialize();

    _disable = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTokenTransfers::duplicate(const CDisableTokenTransfers& di) {
    clear();
    CTransaction::duplicate(di);

    _disable = di._disable;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDisableTokenTransfers& CDisableTokenTransfers::operator=(const CDisableTokenTransfers& di) {
    duplicate(di);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDisableTokenTransfers::operator==(const CDisableTokenTransfers& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDisableTokenTransfers& v1, const CDisableTokenTransfers& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDisableTokenTransfers> CDisableTokenTransfersArray;
extern CArchive& operator>>(CArchive& archive, CDisableTokenTransfersArray& array);
extern CArchive& operator<<(CArchive& archive, const CDisableTokenTransfersArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

