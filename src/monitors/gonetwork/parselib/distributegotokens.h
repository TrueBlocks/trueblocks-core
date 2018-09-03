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
class CDistributeGoTokens : public CTransaction {
public:
    address_t receiver_address;

public:
    CDistributeGoTokens(void);
    CDistributeGoTokens(const CDistributeGoTokens& di);
    virtual ~CDistributeGoTokens(void);
    CDistributeGoTokens& operator=(const CDistributeGoTokens& di);

    DECLARE_NODE(CDistributeGoTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDistributeGoTokens& item) const;
    bool operator!=(const CDistributeGoTokens& item) const { return !operator==(item); }
    friend bool operator<(const CDistributeGoTokens& v1, const CDistributeGoTokens& v2);
    friend ostream& operator<<(ostream& os, const CDistributeGoTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDistributeGoTokens& di);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDistributeGoTokens::CDistributeGoTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDistributeGoTokens::CDistributeGoTokens(const CDistributeGoTokens& di) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(di);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDistributeGoTokens::~CDistributeGoTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDistributeGoTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDistributeGoTokens::initialize(void) {
    CTransaction::initialize();

    receiver_address = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDistributeGoTokens::duplicate(const CDistributeGoTokens& di) {
    clear();
    CTransaction::duplicate(di);

    receiver_address = di.receiver_address;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDistributeGoTokens& CDistributeGoTokens::operator=(const CDistributeGoTokens& di) {
    duplicate(di);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDistributeGoTokens::operator==(const CDistributeGoTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDistributeGoTokens& v1, const CDistributeGoTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDistributeGoTokens> CDistributeGoTokensArray;
extern CArchive& operator>>(CArchive& archive, CDistributeGoTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CDistributeGoTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

