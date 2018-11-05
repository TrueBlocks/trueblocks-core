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
class CSell : public CTransaction {
public:
    address_t _reserveToken;
    biguint_t _sellAmount;
    biguint_t _minReturn;

public:
    CSell(void);
    CSell(const CSell& se);
    virtual ~CSell(void);
    CSell& operator=(const CSell& se);

    DECLARE_NODE(CSell);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSell& item) const;
    bool operator!=(const CSell& item) const { return !operator==(item); }
    friend bool operator<(const CSell& v1, const CSell& v2);
    friend ostream& operator<<(ostream& os, const CSell& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSell& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSell::CSell(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSell::CSell(const CSell& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSell::~CSell(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSell::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSell::initialize(void) {
    CTransaction::initialize();

    _reserveToken = "";
    _sellAmount = 0;
    _minReturn = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSell::duplicate(const CSell& se) {
    clear();
    CTransaction::duplicate(se);

    _reserveToken = se._reserveToken;
    _sellAmount = se._sellAmount;
    _minReturn = se._minReturn;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSell& CSell::operator=(const CSell& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSell::operator==(const CSell& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSell& v1, const CSell& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSell> CSellArray;
extern CArchive& operator>>(CArchive& archive, CSellArray& array);
extern CArchive& operator<<(CArchive& archive, const CSellArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

