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
class CMintTokens : public CTransaction {
public:
    biguint_t _amount;

public:
    CMintTokens(void);
    CMintTokens(const CMintTokens& mi);
    virtual ~CMintTokens(void);
    CMintTokens& operator=(const CMintTokens& mi);

    DECLARE_NODE(CMintTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMintTokens& item) const;
    bool operator!=(const CMintTokens& item) const { return !operator==(item); }
    friend bool operator<(const CMintTokens& v1, const CMintTokens& v2);
    friend ostream& operator<<(ostream& os, const CMintTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMintTokens& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMintTokens::CMintTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMintTokens::CMintTokens(const CMintTokens& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMintTokens::~CMintTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTokens::initialize(void) {
    CTransaction::initialize();

    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintTokens::duplicate(const CMintTokens& mi) {
    clear();
    CTransaction::duplicate(mi);

    _amount = mi._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMintTokens& CMintTokens::operator=(const CMintTokens& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMintTokens::operator==(const CMintTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMintTokens& v1, const CMintTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMintTokens> CMintTokensArray;
extern CArchive& operator>>(CArchive& archive, CMintTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CMintTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

