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
class CReceivePlayerNameList : public CTransaction {
public:
    biguint_t _pID;
    string_q _name;

public:
    CReceivePlayerNameList(void);
    CReceivePlayerNameList(const CReceivePlayerNameList& re);
    virtual ~CReceivePlayerNameList(void);
    CReceivePlayerNameList& operator=(const CReceivePlayerNameList& re);

    DECLARE_NODE(CReceivePlayerNameList);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReceivePlayerNameList& item) const;
    bool operator!=(const CReceivePlayerNameList& item) const { return !operator==(item); }
    friend bool operator<(const CReceivePlayerNameList& v1, const CReceivePlayerNameList& v2);
    friend ostream& operator<<(ostream& os, const CReceivePlayerNameList& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReceivePlayerNameList& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReceivePlayerNameList::CReceivePlayerNameList(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReceivePlayerNameList::CReceivePlayerNameList(const CReceivePlayerNameList& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReceivePlayerNameList::~CReceivePlayerNameList(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceivePlayerNameList::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceivePlayerNameList::initialize(void) {
    CTransaction::initialize();

    _pID = 0;
    _name = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceivePlayerNameList::duplicate(const CReceivePlayerNameList& re) {
    clear();
    CTransaction::duplicate(re);

    _pID = re._pID;
    _name = re._name;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CReceivePlayerNameList& CReceivePlayerNameList::operator=(const CReceivePlayerNameList& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReceivePlayerNameList::operator==(const CReceivePlayerNameList& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReceivePlayerNameList& v1, const CReceivePlayerNameList& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReceivePlayerNameList> CReceivePlayerNameListArray;
extern CArchive& operator>>(CArchive& archive, CReceivePlayerNameListArray& array);
extern CArchive& operator<<(CArchive& archive, const CReceivePlayerNameListArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

