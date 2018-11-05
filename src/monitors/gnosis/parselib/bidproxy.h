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
class CBidProxy : public CTransaction {
public:
    CBidProxy(void);
    CBidProxy(const CBidProxy& bi);
    virtual ~CBidProxy(void);
    CBidProxy& operator=(const CBidProxy& bi);

    DECLARE_NODE(CBidProxy);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBidProxy& item) const;
    bool operator!=(const CBidProxy& item) const { return !operator==(item); }
    friend bool operator<(const CBidProxy& v1, const CBidProxy& v2);
    friend ostream& operator<<(ostream& os, const CBidProxy& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBidProxy& bi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBidProxy::CBidProxy(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBidProxy::CBidProxy(const CBidProxy& bi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBidProxy::~CBidProxy(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBidProxy::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBidProxy::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBidProxy::duplicate(const CBidProxy& bi) {
    clear();
    CTransaction::duplicate(bi);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBidProxy& CBidProxy::operator=(const CBidProxy& bi) {
    duplicate(bi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBidProxy::operator==(const CBidProxy& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBidProxy& v1, const CBidProxy& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBidProxy> CBidProxyArray;
extern CArchive& operator>>(CArchive& archive, CBidProxyArray& array);
extern CArchive& operator<<(CArchive& archive, const CBidProxyArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

