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
class CReceivePlayerInfo : public CTransaction {
public:
    biguint_t _pID;
    address_t _addr;
    string_q _name;
    biguint_t _laff;

public:
    CReceivePlayerInfo(void);
    CReceivePlayerInfo(const CReceivePlayerInfo& re);
    virtual ~CReceivePlayerInfo(void);
    CReceivePlayerInfo& operator=(const CReceivePlayerInfo& re);

    DECLARE_NODE(CReceivePlayerInfo);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReceivePlayerInfo& item) const;
    bool operator!=(const CReceivePlayerInfo& item) const { return !operator==(item); }
    friend bool operator<(const CReceivePlayerInfo& v1, const CReceivePlayerInfo& v2);
    friend ostream& operator<<(ostream& os, const CReceivePlayerInfo& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReceivePlayerInfo& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReceivePlayerInfo::CReceivePlayerInfo(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReceivePlayerInfo::CReceivePlayerInfo(const CReceivePlayerInfo& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReceivePlayerInfo::~CReceivePlayerInfo(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceivePlayerInfo::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceivePlayerInfo::initialize(void) {
    CTransaction::initialize();

    _pID = 0;
    _addr = "";
    _name = "";
    _laff = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceivePlayerInfo::duplicate(const CReceivePlayerInfo& re) {
    clear();
    CTransaction::duplicate(re);

    _pID = re._pID;
    _addr = re._addr;
    _name = re._name;
    _laff = re._laff;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CReceivePlayerInfo& CReceivePlayerInfo::operator=(const CReceivePlayerInfo& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReceivePlayerInfo::operator==(const CReceivePlayerInfo& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReceivePlayerInfo& v1, const CReceivePlayerInfo& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReceivePlayerInfo> CReceivePlayerInfoArray;
extern CArchive& operator>>(CArchive& archive, CReceivePlayerInfoArray& array);
extern CArchive& operator<<(CArchive& archive, const CReceivePlayerInfoArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

