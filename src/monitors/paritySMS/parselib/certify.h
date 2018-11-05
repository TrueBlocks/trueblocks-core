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
class CCertify : public CTransaction {
public:
    address_t _who;

public:
    CCertify(void);
    CCertify(const CCertify& ce);
    virtual ~CCertify(void);
    CCertify& operator=(const CCertify& ce);

    DECLARE_NODE(CCertify);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCertify& item) const;
    bool operator!=(const CCertify& item) const { return !operator==(item); }
    friend bool operator<(const CCertify& v1, const CCertify& v2);
    friend ostream& operator<<(ostream& os, const CCertify& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCertify& ce);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCertify::CCertify(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCertify::CCertify(const CCertify& ce) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ce);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCertify::~CCertify(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCertify::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCertify::initialize(void) {
    CTransaction::initialize();

    _who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCertify::duplicate(const CCertify& ce) {
    clear();
    CTransaction::duplicate(ce);

    _who = ce._who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCertify& CCertify::operator=(const CCertify& ce) {
    duplicate(ce);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCertify::operator==(const CCertify& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCertify& v1, const CCertify& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCertify> CCertifyArray;
extern CArchive& operator>>(CArchive& archive, CCertifyArray& array);
extern CArchive& operator<<(CArchive& archive, const CCertifyArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

