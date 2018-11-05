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
class CApprove : public CTransaction {
public:
    address_t guy;

public:
    CApprove(void);
    CApprove(const CApprove& ap);
    virtual ~CApprove(void);
    CApprove& operator=(const CApprove& ap);

    DECLARE_NODE(CApprove);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CApprove& item) const;
    bool operator!=(const CApprove& item) const { return !operator==(item); }
    friend bool operator<(const CApprove& v1, const CApprove& v2);
    friend ostream& operator<<(ostream& os, const CApprove& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CApprove& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CApprove::CApprove(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CApprove::CApprove(const CApprove& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CApprove::~CApprove(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApprove::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApprove::initialize(void) {
    CTransaction::initialize();

    guy = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CApprove::duplicate(const CApprove& ap) {
    clear();
    CTransaction::duplicate(ap);

    guy = ap.guy;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CApprove& CApprove::operator=(const CApprove& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CApprove::operator==(const CApprove& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CApprove& v1, const CApprove& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CApprove> CApproveArray;
extern CArchive& operator>>(CArchive& archive, CApproveArray& array);
extern CArchive& operator<<(CArchive& archive, const CApproveArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

