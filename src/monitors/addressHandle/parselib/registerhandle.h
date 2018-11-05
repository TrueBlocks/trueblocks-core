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
class CRegisterHandle : public CTransaction {
public:
    string_q _base;
    string_q _handle;
    address_t _addr;

public:
    CRegisterHandle(void);
    CRegisterHandle(const CRegisterHandle& re);
    virtual ~CRegisterHandle(void);
    CRegisterHandle& operator=(const CRegisterHandle& re);

    DECLARE_NODE(CRegisterHandle);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRegisterHandle& item) const;
    bool operator!=(const CRegisterHandle& item) const { return !operator==(item); }
    friend bool operator<(const CRegisterHandle& v1, const CRegisterHandle& v2);
    friend ostream& operator<<(ostream& os, const CRegisterHandle& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRegisterHandle& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRegisterHandle::CRegisterHandle(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRegisterHandle::CRegisterHandle(const CRegisterHandle& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRegisterHandle::~CRegisterHandle(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterHandle::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterHandle::initialize(void) {
    CTransaction::initialize();

    _base = "";
    _handle = "";
    _addr = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegisterHandle::duplicate(const CRegisterHandle& re) {
    clear();
    CTransaction::duplicate(re);

    _base = re._base;
    _handle = re._handle;
    _addr = re._addr;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRegisterHandle& CRegisterHandle::operator=(const CRegisterHandle& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRegisterHandle::operator==(const CRegisterHandle& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRegisterHandle& v1, const CRegisterHandle& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRegisterHandle> CRegisterHandleArray;
extern CArchive& operator>>(CArchive& archive, CRegisterHandleArray& array);
extern CArchive& operator<<(CArchive& archive, const CRegisterHandleArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

