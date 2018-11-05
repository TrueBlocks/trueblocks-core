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
class CDrain : public CTransaction {
public:
    CDrain(void);
    CDrain(const CDrain& dr);
    virtual ~CDrain(void);
    CDrain& operator=(const CDrain& dr);

    DECLARE_NODE(CDrain);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDrain& item) const;
    bool operator!=(const CDrain& item) const { return !operator==(item); }
    friend bool operator<(const CDrain& v1, const CDrain& v2);
    friend ostream& operator<<(ostream& os, const CDrain& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDrain& dr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDrain::CDrain(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDrain::CDrain(const CDrain& dr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(dr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDrain::~CDrain(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDrain::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDrain::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDrain::duplicate(const CDrain& dr) {
    clear();
    CTransaction::duplicate(dr);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDrain& CDrain::operator=(const CDrain& dr) {
    duplicate(dr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDrain::operator==(const CDrain& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDrain& v1, const CDrain& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDrain> CDrainArray;
extern CArchive& operator>>(CArchive& archive, CDrainArray& array);
extern CArchive& operator<<(CArchive& archive, const CDrainArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

