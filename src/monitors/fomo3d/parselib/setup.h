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
class CSetup : public CTransaction {
public:
    address_t _firstCorpBank;

public:
    CSetup(void);
    CSetup(const CSetup& se);
    virtual ~CSetup(void);
    CSetup& operator=(const CSetup& se);

    DECLARE_NODE(CSetup);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetup& item) const;
    bool operator!=(const CSetup& item) const { return !operator==(item); }
    friend bool operator<(const CSetup& v1, const CSetup& v2);
    friend ostream& operator<<(ostream& os, const CSetup& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetup& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetup::CSetup(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetup::CSetup(const CSetup& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetup::~CSetup(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetup::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetup::initialize(void) {
    CTransaction::initialize();

    _firstCorpBank = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetup::duplicate(const CSetup& se) {
    clear();
    CTransaction::duplicate(se);

    _firstCorpBank = se._firstCorpBank;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetup& CSetup::operator=(const CSetup& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetup::operator==(const CSetup& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetup& v1, const CSetup& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetup> CSetupArray;
extern CArchive& operator>>(CArchive& archive, CSetupArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetupArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

