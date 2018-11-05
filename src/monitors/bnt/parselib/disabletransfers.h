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
class CDisableTransfers : public CTransaction {
public:
    bool _disable;

public:
    CDisableTransfers(void);
    CDisableTransfers(const CDisableTransfers& di);
    virtual ~CDisableTransfers(void);
    CDisableTransfers& operator=(const CDisableTransfers& di);

    DECLARE_NODE(CDisableTransfers);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDisableTransfers& item) const;
    bool operator!=(const CDisableTransfers& item) const { return !operator==(item); }
    friend bool operator<(const CDisableTransfers& v1, const CDisableTransfers& v2);
    friend ostream& operator<<(ostream& os, const CDisableTransfers& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDisableTransfers& di);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDisableTransfers::CDisableTransfers(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDisableTransfers::CDisableTransfers(const CDisableTransfers& di) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(di);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDisableTransfers::~CDisableTransfers(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTransfers::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTransfers::initialize(void) {
    CTransaction::initialize();

    _disable = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableTransfers::duplicate(const CDisableTransfers& di) {
    clear();
    CTransaction::duplicate(di);

    _disable = di._disable;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDisableTransfers& CDisableTransfers::operator=(const CDisableTransfers& di) {
    duplicate(di);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDisableTransfers::operator==(const CDisableTransfers& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDisableTransfers& v1, const CDisableTransfers& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDisableTransfers> CDisableTransfersArray;
extern CArchive& operator>>(CArchive& archive, CDisableTransfersArray& array);
extern CArchive& operator<<(CArchive& archive, const CDisableTransfersArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

