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
class CFinalizeFunding : public CTransaction {
public:
    CFinalizeFunding(void);
    CFinalizeFunding(const CFinalizeFunding& fi);
    virtual ~CFinalizeFunding(void);
    CFinalizeFunding& operator=(const CFinalizeFunding& fi);

    DECLARE_NODE(CFinalizeFunding);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFinalizeFunding& item) const;
    bool operator!=(const CFinalizeFunding& item) const { return !operator==(item); }
    friend bool operator<(const CFinalizeFunding& v1, const CFinalizeFunding& v2);
    friend ostream& operator<<(ostream& os, const CFinalizeFunding& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFinalizeFunding& fi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFinalizeFunding::CFinalizeFunding(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFinalizeFunding::CFinalizeFunding(const CFinalizeFunding& fi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFinalizeFunding::~CFinalizeFunding(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinalizeFunding::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinalizeFunding::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinalizeFunding::duplicate(const CFinalizeFunding& fi) {
    clear();
    CTransaction::duplicate(fi);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFinalizeFunding& CFinalizeFunding::operator=(const CFinalizeFunding& fi) {
    duplicate(fi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFinalizeFunding::operator==(const CFinalizeFunding& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFinalizeFunding& v1, const CFinalizeFunding& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFinalizeFunding> CFinalizeFundingArray;
extern CArchive& operator>>(CArchive& archive, CFinalizeFundingArray& array);
extern CArchive& operator<<(CArchive& archive, const CFinalizeFundingArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

