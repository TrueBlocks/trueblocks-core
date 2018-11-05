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
class CEnableTransfer : public CTransaction {
public:
    CEnableTransfer(void);
    CEnableTransfer(const CEnableTransfer& en);
    virtual ~CEnableTransfer(void);
    CEnableTransfer& operator=(const CEnableTransfer& en);

    DECLARE_NODE(CEnableTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEnableTransfer& item) const;
    bool operator!=(const CEnableTransfer& item) const { return !operator==(item); }
    friend bool operator<(const CEnableTransfer& v1, const CEnableTransfer& v2);
    friend ostream& operator<<(ostream& os, const CEnableTransfer& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEnableTransfer& en);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEnableTransfer::CEnableTransfer(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEnableTransfer::CEnableTransfer(const CEnableTransfer& en) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(en);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEnableTransfer::~CEnableTransfer(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableTransfer::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableTransfer::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableTransfer::duplicate(const CEnableTransfer& en) {
    clear();
    CTransaction::duplicate(en);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CEnableTransfer& CEnableTransfer::operator=(const CEnableTransfer& en) {
    duplicate(en);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEnableTransfer::operator==(const CEnableTransfer& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEnableTransfer& v1, const CEnableTransfer& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEnableTransfer> CEnableTransferArray;
extern CArchive& operator>>(CArchive& archive, CEnableTransferArray& array);
extern CArchive& operator<<(CArchive& archive, const CEnableTransferArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

