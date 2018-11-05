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
class CTransfer : public CTransaction {
public:
    CTransfer(void);
    CTransfer(const CTransfer& tr);
    virtual ~CTransfer(void);
    CTransfer& operator=(const CTransfer& tr);

    DECLARE_NODE(CTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransfer& item) const;
    bool operator!=(const CTransfer& item) const { return !operator==(item); }
    friend bool operator<(const CTransfer& v1, const CTransfer& v2);
    friend ostream& operator<<(ostream& os, const CTransfer& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransfer& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransfer::CTransfer(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransfer::CTransfer(const CTransfer& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransfer::~CTransfer(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransfer::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransfer::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransfer::duplicate(const CTransfer& tr) {
    clear();
    CTransaction::duplicate(tr);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransfer& CTransfer::operator=(const CTransfer& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransfer::operator==(const CTransfer& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransfer& v1, const CTransfer& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransfer> CTransferArray;
extern CArchive& operator>>(CArchive& archive, CTransferArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

