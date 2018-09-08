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
class CAcceptRegistrarTransfer : public CTransaction {
public:
    string_q hash;
    address_t deed;
    biguint_t registrationDate;

public:
    CAcceptRegistrarTransfer(void);
    CAcceptRegistrarTransfer(const CAcceptRegistrarTransfer& ac);
    virtual ~CAcceptRegistrarTransfer(void);
    CAcceptRegistrarTransfer& operator=(const CAcceptRegistrarTransfer& ac);

    DECLARE_NODE(CAcceptRegistrarTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAcceptRegistrarTransfer& item) const;
    bool operator!=(const CAcceptRegistrarTransfer& item) const { return !operator==(item); }
    friend bool operator<(const CAcceptRegistrarTransfer& v1, const CAcceptRegistrarTransfer& v2);
    friend ostream& operator<<(ostream& os, const CAcceptRegistrarTransfer& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAcceptRegistrarTransfer& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcceptRegistrarTransfer::CAcceptRegistrarTransfer(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcceptRegistrarTransfer::CAcceptRegistrarTransfer(const CAcceptRegistrarTransfer& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcceptRegistrarTransfer::~CAcceptRegistrarTransfer(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptRegistrarTransfer::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptRegistrarTransfer::initialize(void) {
    CTransaction::initialize();

    hash = "";
    deed = "";
    registrationDate = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptRegistrarTransfer::duplicate(const CAcceptRegistrarTransfer& ac) {
    clear();
    CTransaction::duplicate(ac);

    hash = ac.hash;
    deed = ac.deed;
    registrationDate = ac.registrationDate;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcceptRegistrarTransfer& CAcceptRegistrarTransfer::operator=(const CAcceptRegistrarTransfer& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAcceptRegistrarTransfer::operator==(const CAcceptRegistrarTransfer& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAcceptRegistrarTransfer& v1, const CAcceptRegistrarTransfer& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAcceptRegistrarTransfer> CAcceptRegistrarTransferArray;
extern CArchive& operator>>(CArchive& archive, CAcceptRegistrarTransferArray& array);
extern CArchive& operator<<(CArchive& archive, const CAcceptRegistrarTransferArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

