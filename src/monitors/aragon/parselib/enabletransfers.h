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
class CEnableTransfers : public CTransaction {
public:
    bool _transfersEnabled;

public:
    CEnableTransfers(void);
    CEnableTransfers(const CEnableTransfers& en);
    virtual ~CEnableTransfers(void);
    CEnableTransfers& operator=(const CEnableTransfers& en);

    DECLARE_NODE(CEnableTransfers);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEnableTransfers& item) const;
    bool operator!=(const CEnableTransfers& item) const { return !operator==(item); }
    friend bool operator<(const CEnableTransfers& v1, const CEnableTransfers& v2);
    friend ostream& operator<<(ostream& os, const CEnableTransfers& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEnableTransfers& en);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEnableTransfers::CEnableTransfers(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEnableTransfers::CEnableTransfers(const CEnableTransfers& en) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(en);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEnableTransfers::~CEnableTransfers(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableTransfers::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableTransfers::initialize(void) {
    CTransaction::initialize();

    _transfersEnabled = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableTransfers::duplicate(const CEnableTransfers& en) {
    clear();
    CTransaction::duplicate(en);

    _transfersEnabled = en._transfersEnabled;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CEnableTransfers& CEnableTransfers::operator=(const CEnableTransfers& en) {
    duplicate(en);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEnableTransfers::operator==(const CEnableTransfers& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEnableTransfers& v1, const CEnableTransfers& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEnableTransfers> CEnableTransfersArray;
extern CArchive& operator>>(CArchive& archive, CEnableTransfersArray& array);
extern CArchive& operator<<(CArchive& archive, const CEnableTransfersArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

