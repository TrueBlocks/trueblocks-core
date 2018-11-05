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
class CCreateTokenProxy : public CTransaction {
public:
    address_t _tokenHolder;

public:
    CCreateTokenProxy(void);
    CCreateTokenProxy(const CCreateTokenProxy& cr);
    virtual ~CCreateTokenProxy(void);
    CCreateTokenProxy& operator=(const CCreateTokenProxy& cr);

    DECLARE_NODE(CCreateTokenProxy);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCreateTokenProxy& item) const;
    bool operator!=(const CCreateTokenProxy& item) const { return !operator==(item); }
    friend bool operator<(const CCreateTokenProxy& v1, const CCreateTokenProxy& v2);
    friend ostream& operator<<(ostream& os, const CCreateTokenProxy& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCreateTokenProxy& cr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCreateTokenProxy::CCreateTokenProxy(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCreateTokenProxy::CCreateTokenProxy(const CCreateTokenProxy& cr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCreateTokenProxy::~CCreateTokenProxy(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateTokenProxy::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateTokenProxy::initialize(void) {
    CTransaction::initialize();

    _tokenHolder = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateTokenProxy::duplicate(const CCreateTokenProxy& cr) {
    clear();
    CTransaction::duplicate(cr);

    _tokenHolder = cr._tokenHolder;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCreateTokenProxy& CCreateTokenProxy::operator=(const CCreateTokenProxy& cr) {
    duplicate(cr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCreateTokenProxy::operator==(const CCreateTokenProxy& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCreateTokenProxy& v1, const CCreateTokenProxy& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCreateTokenProxy> CCreateTokenProxyArray;
extern CArchive& operator>>(CArchive& archive, CCreateTokenProxyArray& array);
extern CArchive& operator<<(CArchive& archive, const CCreateTokenProxyArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

